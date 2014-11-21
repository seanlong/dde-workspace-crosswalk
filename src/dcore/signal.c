/**
 * Copyright (c) 2011 ~ 2012 Deepin, Inc.
 *               2011 ~ 2012 snyh
 *
 * Author:      snyh <snyh@snyh.org>
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
#include "signal.h"

#include <glib.h>

#include "common/config.h"
#include "XW_Extension.h"

#include <stdio.h>

static GHashTable* signals = NULL;

extern const XW_MessagingInterface* async_messaging_interface;

void js_post_message(const char* name, json_object* json)
{
    if (signals == NULL) {
        g_warning("signals %s has not init!\n", name);
        return;
    }

    //JSContextRef ctx = get_global_context();
    //g_return_if_fail(ctx != NULL);
    //JSObjectRef cb = g_hash_table_lookup(signals, name);
    JSCallback* cb = g_hash_table_lookup(signals, name);

    if (cb != NULL) {
        //Call* call = g_new0(Call, 1);
        //call->cb = cb;
        //call->arg = json;
	//JSValueProtect(ctx, json);
        //g_main_context_invoke(NULL, (GSourceFunc)_interal_call, call);
      struct json_object* ret = json_object_new_object();
      json_object_object_add(ret, "signal", json_object_new_string(name));
      json_object_object_add(ret, "data", json);
      async_messaging_interface->PostMessage(cb->xw_instance, json_object_to_json_string(ret));
    } else {
        g_warning("signal %s has not connected!\n", name);
    }
}

static
void js_post_message_simply(const char* name, const char* format, ...)
{
    if (format == NULL) {
        js_post_message(name, NULL);
    } else {
        va_list args;
        va_start(args, format);
        char* json_str = g_strdup_vprintf(format, args);
        va_end(args);

        js_post_message(name, json_tokener_parse(json_str));
        g_free(json_str);
    }
}

void js_post_signal(const char* signal)
{
    js_post_message_simply(signal, NULL);
}


void unprotect(gpointer data)
{
    //GRAB_CTX();
    //JSContextRef ctx = get_global_context();
    //JSValueUnprotect(ctx, (JSValueRef)data);
    //UNGRAB_CTX();
}

JS_EXPORT_API
void dcore_signal_connect(const char* type, JSCallback* value)
{
    if (signals == NULL) {
        signals = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, unprotect);
    }
    //JSContextRef ctx = get_global_context();
    //JSObjectRef cb = JSValueToObject(ctx, value, js->exception);
    //if (cb != NULL || !JSObjectIsFunction(ctx, cb)) {
        //JSValueProtect(ctx, cb);
        g_hash_table_insert(signals, g_strdup(type), (gpointer)value);
        /*g_message("signal connect %s \n", type);*/
    //} else {
    //    g_warning("signal_connect's second parameter must be an function object");
    //}
}

