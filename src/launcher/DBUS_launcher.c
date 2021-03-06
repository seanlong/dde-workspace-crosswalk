
/*
* THIS FILE WAS AUTOMATICALLY GENERATED, DO NOT EDIT.
*
* This file was generated by the make_dbus.go script.
*/
#include <glib.h>
#include <gio/gio.h>

static int _service_owner_id = 0;
static GDBusInterfaceInfo * interface_info = NULL;

void launcher_show() __attribute__((weak));

void launcher_hide() __attribute__((weak));

void launcher_toggle() __attribute__((weak));

void launcher_quit() __attribute__((weak));

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
        } else if (&launcher_show != 0 && g_strcmp0(method, "Show") == 0) {
    
    
     launcher_show();

    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
        } else if (&launcher_hide != 0 && g_strcmp0(method, "Hide") == 0) {
    
    
     launcher_hide();

    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
        } else if (&launcher_toggle != 0 && g_strcmp0(method, "Toggle") == 0) {
    
    
     launcher_toggle();

    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
        } else if (&launcher_quit != 0 && g_strcmp0(method, "Exit") == 0) {
    
    
     launcher_quit();

    
        g_dbus_method_invocation_return_value (invocation, retval);
        return;
    
    } else {
        g_dbus_method_invocation_return_dbus_error (invocation,
                "com.deepin.dde.launcher.Error",
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
            "/com/deepin/dde/launcher",
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
setup_launcher_dbus_service()
{
    const char* xml = "<?xml version=\"1.0\"?>"
"<node>"
"<interface name=\"com.deepin.dde.launcher\">"
"       <method name=\"Show\">"
"       </method>" 
"       <method name=\"Hide\">"
"       </method>" 
"       <method name=\"Toggle\">"
"       </method>" 
"       <method name=\"Exit\">"
"       </method>" 
"       <signal name=\"Shown\">"
"       </signal>"
"       <signal name=\"Closed\">"
"       </signal>"
"</interface>"
"</node>";

    GError* error = NULL;
    GDBusNodeInfo * node_info = g_dbus_node_info_new_for_xml (xml, &error);
    if (error != NULL) {
        g_critical ("Unable to parse interface xml: %s", error->message);
        g_error_free (error);
    }

    interface_info = g_dbus_node_info_lookup_interface (node_info, "com.deepin.dde.launcher");
    /*g_dbus_node_info_unref(node_info);*/
    if (interface_info == NULL) {
        g_critical ("Unable to find interface \"com.deepin.dde.launcher\"");
    }

    _service_owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
            "com.deepin.dde.launcher",
            G_BUS_NAME_OWNER_FLAGS_NONE,
            _on_bus_acquired,
            NULL,
            NULL,
            NULL,
            NULL);
}


void dbus_launcher_show()
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
        g_warning ("call dbus_launcher_show on com.deepin.dde.launcher failed: %s",
        error->message);
        g_error_free(error);
    }
    if (proxy != NULL) {
        GVariant* params = NULL;
        
        GVariant* retval = g_dbus_proxy_call_sync(proxy, "Show",
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

void dbus_launcher_toggle()
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
        g_warning ("call dbus_launcher_toggle on com.deepin.dde.launcher failed: %s",
        error->message);
        g_error_free(error);
    }
    if (proxy != NULL) {
        GVariant* params = NULL;
        
        GVariant* retval = g_dbus_proxy_call_sync(proxy, "Toggle",
                                               params,
                                               G_DBUS_CALL_FLAGS_NONE,
                                               -1, NULL, NULL);
        if (retval != NULL) {
    
            g_variant_unref(retval);
        }

        g_object_unref(proxy);
    }
    
}

