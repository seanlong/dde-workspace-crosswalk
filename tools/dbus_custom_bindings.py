{
    "sys": {
        "custom_js": """
var v8tools = requireNative('v8tools');
var sys_conn;
var session_conn;
function getObject(bus_type, bus_name, path, interface_name) {
  var bus;
  if (bus_type == 'system') {
    if (!sys_conn)
      sys_conn = v8tools.SystemBus();
    bus = sys_conn;
  } else if (bus_type == 'session') {
    if (!session_conn)
      session_conn = v8tools.SessionBus();
    bus = session_conn;
  } else {
    console.error('bad bus type: ' + bus_type);
    return;
  }

  var dbus_obj = v8tools.GetInterface(bus, bus_name, path, interface_name);
  for (var e in dbus_obj) {
    if (typeof dbus_obj[e] == 'function') {
      dbus_obj[e + '_sync'] = dbus_obj[e];
      //delete dbus_obj[e];
    }
  }

  dbus_obj.connect = function(signal_name, callback) {
    if (!dbus_obj[signal_name]) {
        console.error('signal not found: ' + signal_name);
        return;
    }
    if (!dbus_obj[signal_name].onemit) {
      dbus_obj[signal_name].callbacks = {};
      dbus_obj[signal_name].idx = 0;
      dbus_obj[signal_name].onemit = function() {
        for(var i in dbus_obj[signal_name].callbacks)
          dbus_obj[signal_name].callbacks[i].call(null, Array.prototype.slice.call(arguments, 0));
      };
      dbus_obj[signal_name].enabled = true;
    }

    dbus_obj[signal_name].idx++;
    dbus_obj[signal_name].callbacks[dbus_obj[signal_name].idx] = callback;
    //console.log('connect ' + signal_name);
    //console.log(dbus_obj[signal_name]);
    return dbus_obj[signal_name].idx;
  };

  dbus_obj.dis_connect = function(signal_name, id) {
    dbus_obj[signal_name].callbacks[id] = undefined;
    dbus_obj[signal_name].enabled = false;
    //console.log('disconnect ' + signal_name);
    //console.log(dbus_obj[signal_name]);
  };


  var dbus_prop_obj = v8tools.GetInterface(bus, bus_name, path, 'org.freedesktop.DBus.Properties');
  if (dbus_prop_obj) {
    var props = dbus_prop_obj.GetAll(interface_name);
    //for (var e in props)
        //console.log(e + props[e]);

    for (var p in props[0])
      dbus_obj[p] = props[0][p];
  }

  return dbus_obj;
};

exports.sys = function(bus_name) {
  var path = bus_name.split('.').join('/');
  return getObject('system', bus_name, path, bus_name);
};
""",
        "custom_c": """
void handle_sys(XW_Instance instance, json_object* object) {}
"""
    },

    "sys_object": {
        "custom_js": """
exports.sys = function(bus_name, object_path, interface_name) {
  return getObject('system', bus_name, object_path, interface_name);
};
""",
        "custom_c": """
void handle_sys_object(XW_Instance instance, json_object* object) {}
"""
    },

    "session": {
        "custom_js": """
exports.session = function(bus_name) {
  var path = '/' + bus_name.split('.').join('/');
  return getObject('session', bus_name, path, bus_name);
};
""",
        "custom_c": """
void handle_session(XW_Instance instance, json_object* object) {}
"""
    },

    "session_object": {
        "custom_js": """
exports.session_object = function(bus_name, object_path, interface_name) {
  return getObject('session', bus_name, object_path, interface_name);
};
""",
        "custom_c": """
void handle_session_object(XW_Instance instance, json_object* object) {}
"""
    }
}
