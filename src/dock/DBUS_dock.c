
/*
* THIS FILE WAS AUTOMATICALLY GENERATED, DO NOT EDIT.
*
* This file was generated by the make_dbus.go script.
*/
#include <glib.h>
#include <gio/gio.h>

static int _service_owner_id = 0;
static GDBusInterfaceInfo * interface_info = NULL;

void dock_show_inspector() __attribute__((weak));

void dock_bus_message_notify(gchar*,gchar*) __attribute__((weak));

guint64 dock_xid() __attribute__((weak));

static void _bus_method_call (GDBusConnection * connection,
                 const gchar * sender, const gchar * object_path, const gchar * interface,
                 const gchar * method, GVariant * params,
                 GDBusMethodInvocation * invocation, gpointer user_data)
{
        (void)connection;
        (void)sender;
        (void)object_path;
        (void)interface;
        (void)user_data;
        (void)params;
        GVariant * retval = NULL;
        if (0) { 
        } else if (&dock_show_inspector != 0 && g_strcmp0(method, "ShowInspector") == 0) {
    
    
     dock_show_inspector();

    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
        } else if (&dock_bus_message_notify != 0 && g_strcmp0(method, "MessageNotify") == 0) {
    
        gchar* arg0;
    
        gchar* arg1;
    
    g_variant_get(params, "(ss)", &arg0,&arg1);

     dock_bus_message_notify(arg0,arg1);

    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
        } else if (&dock_xid != 0 && g_strcmp0(method, "Xid") == 0) {
    
    
     guint64 _c_retval = dock_xid();

    
        retval = g_variant_new("(t)", _c_retval);
        
    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
    } else {
        g_dbus_method_invocation_return_dbus_error (invocation,
                "com.deepin.dde.dock.Error",
                "Can't find this method");
        return;
    }

}
static void _on_bus_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data)
{
    (void)name;
    static GDBusInterfaceVTable interface_table = {
        method_call:   _bus_method_call,
        get_property:   NULL, /* No properties */
        set_property:   NULL  /* No properties */
    };
    GError* error = NULL;
    g_dbus_connection_register_object (connection,
            "/com/deepin/dde/dock",
            interface_info,
            &interface_table,
            user_data,
            NULL,
            &error);

    if (error != NULL) {
        g_critical ("Unable to register the object to DBus: %s", error->message);
        g_error_free (error);
        g_bus_unown_name (_service_owner_id);
    }
}

void
setup_dock_dbus_service()
{
    const char* xml = "<?xml version=\"1.0\"?>"
"<node>"
"<interface name=\"com.deepin.dde.dock\">"
"       <method name=\"ShowInspector\">"
"       </method>" 
"       <method name=\"MessageNotify\">"
"             <arg name=\"appid\" type=\"s\" direction=\"in\"></arg>"
"             <arg name=\"itemid\" type=\"s\" direction=\"in\"></arg>"
"       </method>" 
"       <method name=\"Xid\">"
"             <arg name=\"xid\" type=\"t\" direction=\"out\"></arg>"
"       </method>" 
"</interface>"
"</node>";

    GError* error = NULL;
    GDBusNodeInfo * node_info = g_dbus_node_info_new_for_xml (xml, &error);
    if (error != NULL) {
        g_critical ("Unable to parse interface xml: %s", error->message);
        g_error_free (error);
    }

    interface_info = g_dbus_node_info_lookup_interface (node_info, "com.deepin.dde.dock");
    /*g_dbus_node_info_unref(node_info);*/
    if (interface_info == NULL) {
        g_critical ("Unable to find interface \"com.deepin.dde.dock\"");
    }

    _service_owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
            "com.deepin.dde.dock",
            G_BUS_NAME_OWNER_FLAGS_NONE,
            _on_bus_acquired,
            NULL,
            NULL,
            NULL,
            NULL);
}


void dbus_set_desktop_focused(gboolean arg0)
{
    
    GError *error = NULL;
    GDBusProxy* proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                                     0,
                                                     NULL,
                                                     "com.deepin.dde.desktop",
                                                     "/com/deepin/dde/desktop",
                                                     "com.deepin.dde.desktop",
                                                     NULL,
                                                     &error);
    if (error != NULL) {
        g_warning ("call dbus_set_desktop_focused on com.deepin.dde.desktop failed: %s",
        error->message);
        g_error_free(error);
    }
    if (proxy != NULL) {
        GVariant* params = NULL;
        params = g_variant_new("(b)", arg0);
        GVariant* retval = g_dbus_proxy_call_sync(proxy, "FocusChanged",
                                               params,
                                               G_DBUS_CALL_FLAGS_NONE,
                                               -1, NULL, NULL);
        if (retval != NULL) {
    
            g_variant_unref(retval);
        }

        g_object_unref(proxy);
    }
    
}



void dbus_launcher_hide()
{
    
    GError *error = NULL;
    GDBusProxy* proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                                     0,
                                                     NULL,
                                                     "com.deepin.dde.launcher",
                                                     "/com/deepin/dde/launcher",
                                                     "com.deepin.dde.launcher",
                                                     NULL,
                                                     &error);
    if (error != NULL) {
        g_warning ("call dbus_launcher_hide on com.deepin.dde.launcher failed: %s",
        error->message);
        g_error_free(error);
    }
    if (proxy != NULL) {
        GVariant* params = NULL;
        
        GVariant* retval = g_dbus_proxy_call_sync(proxy, "Hide",
                                               params,
                                               G_DBUS_CALL_FLAGS_NONE,
                                               -1, NULL, NULL);
        if (retval != NULL) {
    
            g_variant_unref(retval);
        }

        g_object_unref(proxy);
    }
    
}



void dbus_dock_daemon_update_hide_state()
{
    
    GError *error = NULL;
    GDBusProxy* proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                                     0,
                                                     NULL,
                                                     "com.deepin.daemon.Dock",
                                                     "/dde/dock/HideStateManager",
                                                     "dde.dock.HideStateManger",
                                                     NULL,
                                                     &error);
    if (error != NULL) {
        g_warning ("call dbus_dock_daemon_update_hide_state on com.deepin.daemon.Dock failed: %s",
        error->message);
        g_error_free(error);
    }
    if (proxy != NULL) {
        GVariant* params = NULL;
        
        GVariant* retval = g_dbus_proxy_call_sync(proxy, "UpdateState",
                                               params,
                                               G_DBUS_CALL_FLAGS_NONE,
                                               -1, NULL, NULL);
        if (retval != NULL) {
    
            g_variant_unref(retval);
        }

        g_object_unref(proxy);
    }
    
}

void dbus_dock_daemon_cancel_toggle_show()
{
    
    GError *error = NULL;
    GDBusProxy* proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                                     0,
                                                     NULL,
                                                     "com.deepin.daemon.Dock",
                                                     "/dde/dock/HideStateManager",
                                                     "dde.dock.HideStateManger",
                                                     NULL,
                                                     &error);
    if (error != NULL) {
        g_warning ("call dbus_dock_daemon_cancel_toggle_show on com.deepin.daemon.Dock failed: %s",
        error->message);
        g_error_free(error);
    }
    if (proxy != NULL) {
        GVariant* params = NULL;
        
        GVariant* retval = g_dbus_proxy_call_sync(proxy, "CancelToggleShow",
                                               params,
                                               G_DBUS_CALL_FLAGS_NONE,
                                               -1, NULL, NULL);
        if (retval != NULL) {
    
            g_variant_unref(retval);
        }

        g_object_unref(proxy);
    }
    
}

