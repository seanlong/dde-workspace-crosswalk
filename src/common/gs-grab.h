/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2004-2006 William Jon McCann <mccann@jhu.edu>
 * Copyright (C) 2013      Linux Deepin Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Authors: William Jon McCann <mccann@jhu.edu>
 *
 */
#ifndef __GS_GRAB_H
#define __GS_GRAB_H

#include <glib.h>
#include <gdk/gdk.h>

G_BEGIN_DECLS
#define GS_TYPE_GRAB         (gs_grab_get_type ())
#define GS_GRAB(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GS_TYPE_GRAB, GSGrab))
#define GS_GRAB_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GS_TYPE_GRAB, GSGrabClass))
#define GS_IS_GRAB(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GS_TYPE_GRAB))
#define GS_IS_GRAB_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GS_TYPE_GRAB))
#define GS_GRAB_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GS_TYPE_GRAB, GSGrabClass))

typedef struct GSGrabPrivate GSGrabPrivate;

typedef struct
{
        GObject        parent;
        GSGrabPrivate *priv;
} GSGrab;

typedef struct
{
        GObjectClass   parent_class;

} GSGrabClass;

GSGrab  * gs_grab_new              (void);

void      gs_grab_move_to_window   (GSGrab    *grab,
                                    GdkWindow *window,
                                    GdkScreen *screen,
                                    gboolean   hide_cursor);
G_END_DECLS
#endif /* __GS_GRAB_H */
