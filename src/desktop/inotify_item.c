/**
 * Copyright (c) 2011 ~ 2014 Deepin, Inc.
 *               2011 ~ 2014 snyh
 *
 * Author:      snyh <snyh@snyh.org>
 *              bluth <yuanchenglu001@gmail.com>
 * Maintainer:  snyh <snyh@snyh.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 **/
#include "inotify_item.h"
#include <gio/gio.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <glib-object.h>

#include "common/utils.h"
#include "common/xdg_misc.h"
#include "dentry/entry.h"
#include "json-c/json.h"

extern void desktop_item_update();
PRIVATE gboolean _inotify_poll();
PRIVATE void _remove_monitor_directory(GFile*);
PRIVATE void _add_monitor_directory(GFile*);
void handle_delete(GFile* f);

static GHashTable* _monitor_table = NULL;
static GFile* _desktop_file = NULL;
static GFile* _trash_can = NULL;
static int _inotify_fd = -1;

extern void js_post_message(const char* name, json_object* json);
extern void js_post_signal(const char* signal);

void trash_changed()
{
    GFileInfo* info = g_file_query_info(_trash_can, G_FILE_ATTRIBUTE_TRASH_ITEM_COUNT, G_FILE_QUERY_INFO_NONE, NULL, NULL);
    int count = g_file_info_get_attribute_uint32(info, G_FILE_ATTRIBUTE_TRASH_ITEM_COUNT);
    g_object_unref(info);
    //JSObjectRef value = json_create();
    //json_append_number(value, "value", count);
    json_object* value = json_object_new_object();
    json_object_object_add(value, "value", json_object_new_int(count));
    js_post_message("trash_count_changed", value);
}

PRIVATE
void _add_monitor_directory(GFile* f)
{
    g_assert(_inotify_fd != -1);

    GFileInfo* info = g_file_query_info(f, "standard::type", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, NULL);
    if (info == NULL) return; //temp file may cause this like downloading file or compressing file
    GFileType type = g_file_info_get_attribute_uint32(info, G_FILE_ATTRIBUTE_STANDARD_TYPE);
    g_assert(info != NULL);
    if (g_file_info_get_is_symlink(info)) {
        GFile* maybe_real_target = g_file_new_for_uri(g_file_info_get_symlink_target(info));
        _add_monitor_directory(maybe_real_target);
        g_object_unref(maybe_real_target);
    } else if (type == G_FILE_TYPE_DIRECTORY) {
        char* path = g_file_get_path(f);
        int watch = inotify_add_watch(_inotify_fd, path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO | IN_ATTRIB);
        g_free(path);
        g_hash_table_insert(_monitor_table, GINT_TO_POINTER(watch), g_object_ref(f));
    }
    g_object_unref(info);
}

void install_monitor()
{
    if (_inotify_fd == -1) {
        _inotify_fd = inotify_init1(IN_NONBLOCK|IN_CLOEXEC);
        _monitor_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)g_object_unref);
        g_timeout_add(50, (GSourceFunc)_inotify_poll, NULL);

        _desktop_file = g_file_new_for_commandline_arg(DESKTOP_DIR());
        _trash_can = g_file_new_for_uri("trash:///");
        GFileMonitor* m = g_file_monitor(_trash_can, G_FILE_MONITOR_NONE, NULL, NULL);
        g_signal_connect(m, "changed", G_CALLBACK(trash_changed), NULL);

        _add_monitor_directory(_desktop_file);

        GDir *dir =  g_dir_open(DESKTOP_DIR(), 0, NULL);

        if (dir != NULL) {
            const char* filename = NULL;
            while ((filename = g_dir_read_name(dir)) != NULL) {
                GFile* f = g_file_get_child(_desktop_file, filename);
                _add_monitor_directory(f);
                g_object_unref(f);
            }
            g_dir_close(dir);
        }
    }
}

void handle_rename(GFile* old_f, GFile* new_f)
{
    _add_monitor_directory(new_f);
    _remove_monitor_directory(old_f);

    char* path = g_file_get_path(new_f);
    Entry* entry = dentry_create_by_path(path);
    g_free(path);

    //JSObjectRef json = json_create();
    //json_append_nobject(json, "old", old_f, g_object_ref, g_object_unref);
    //json_append_nobject(json, "new", entry, g_object_ref, g_object_unref);
    json_object* json = json_object_new_object();
    json_object_object_add(json, "old", json_object_new_int64((int64_t)old_f));
    json_object_object_add(json, "new", json_object_new_int64((int64_t)entry));
    js_post_message("item_rename", json);

    //g_object_unref(entry);
}

void handle_delete(GFile* f)
{
    _remove_monitor_directory(f);
    //JSObjectRef json = json_create();
    //json_append_nobject(json, "entry", f, g_object_ref, g_object_unref);
    json_object* json = json_object_new_object();
    json_object_object_add(json, "entry", json_object_new_int64((int64_t)f));
    js_post_message("item_delete", json);
}

void handle_update(GFile* f)
{
    // g_message("handle_update");
    if (g_file_query_file_type(f, G_FILE_QUERY_INFO_NONE ,NULL) != G_FILE_TYPE_UNKNOWN) {
        char* path = g_file_get_path(f);
        Entry* entry = dentry_create_by_path(path);
        g_free(path);

        //JSObjectRef json = json_create();
        //json_append_nobject(json, "entry", entry, g_object_ref, g_object_unref);
        json_object* json = json_object_new_object();
        json_object_object_add(json, "entry", json_object_new_int64((int64_t)entry));
        js_post_message("item_update", json);
        desktop_item_update();

        //g_object_unref(entry);
    }
}

void handle_new(GFile* f)
{
    _add_monitor_directory(f);
    handle_update(f);
}

// test : use real fileops to test it

PRIVATE
void _remove_monitor_directory(GFile* f)
{
    int wd = -1;
    GList* _keys = g_hash_table_get_keys(_monitor_table);
    GList* keys = _keys;
    while (keys != NULL) {
        GFile* test = g_hash_table_lookup(_monitor_table, keys->data);
        if (test != NULL && g_file_equal(f, test)) {
            wd = GPOINTER_TO_INT(keys->data);
            break;
        }
        keys = g_list_next(keys);
    }
    g_list_free(_keys);
    if (wd != -1) {
        inotify_rm_watch(_inotify_fd, wd);
        g_hash_table_remove(_monitor_table, GINT_TO_POINTER(wd));
    }
}


// test important
PRIVATE
gboolean _inotify_poll()
{
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

    if (_inotify_fd != -1) {
        char buffer[EVENT_BUF_LEN];
        int length = read(_inotify_fd, buffer, EVENT_BUF_LEN);

        struct inotify_event *move_out_event = NULL;
        GFile* old = NULL;// test : use real fileops to test it

        for (int i=0; i<length; ) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            i += EVENT_SIZE+event->len;
            if(desktop_file_filter(event->name))
                continue;
            if (event->len) {
                GFile* p = g_hash_table_lookup(_monitor_table, GINT_TO_POINTER(event->wd));

                if (g_file_equal(p, _desktop_file)) {
                    /* BEGIN MOVE EVENT HANDLE */
                    if ((event->mask & IN_MOVED_FROM) && (move_out_event == NULL)) {
                        move_out_event = event;
                        old = g_file_get_child(p, event->name);
                        continue;
                    } else if ((event->mask & IN_MOVED_FROM) && (move_out_event != NULL)) {
                        GFile* f = g_file_get_child(_desktop_file, event->name);
                        handle_delete(f);
                        g_object_unref(f);
                        continue;
                    } else if ((event->mask & IN_MOVED_TO) && (move_out_event != NULL)) {
                        move_out_event = NULL;
                        GFile* f = g_file_get_child(p, event->name);

                        handle_rename(old, f);
                        g_object_unref(f);
                        g_object_unref(old);
                        old = NULL;
                        continue;
                    /* END MVOE EVENT HANDLE */
                    } else if (event->mask & IN_DELETE) {
                        GFile* f = g_file_get_child(p, event->name);
                        handle_delete(f);
                        g_object_unref(f);
                    } else if (event->mask & IN_CREATE) {
                        GFile* f = g_file_get_child(p, event->name);
                        handle_new(f);
                        g_object_unref(f);
                    } else {
                        GFile* f = g_file_get_child(p, event->name);
                        _add_monitor_directory(f);
                        handle_update(f);
                        g_object_unref(f);
                    }

                } else {
                    if (event->mask & IN_MOVED_TO) {
                        GFile* f = g_file_get_child(_desktop_file, event->name);
                        handle_delete(f);
                        g_object_unref(f);
                    }
                    handle_update(p);
                }
            }
        }
        if (move_out_event != NULL) {
            handle_delete(old);
            move_out_event = NULL;
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean desktop_file_filter(const char *file_name)
{
    g_assert(file_name != NULL);
    if((file_name[0] == '.' && !g_str_has_prefix(file_name, DEEPIN_RICH_DIR)) || g_str_has_suffix(file_name, "~"))
        return TRUE;
    else
        return FALSE;
}

