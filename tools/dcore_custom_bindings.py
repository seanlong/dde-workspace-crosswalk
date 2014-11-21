{
    "signal_connect": {
        "custom_js": """
//signal_connect custom binding
var callbacks_ = {};
extension.setMessageListener(function(msg) {
  //console.log(msg);
  var obj = JSON.parse(msg);
  if (!obj.signal) {
    console.log('invalid incomming event' + msg);
    return;
  }
  if (obj.signal in callbacks_)
    callbacks_[obj.signal](obj.data);
  else
    console.warn('signal handler not found: ' + obj.signal);
});

exports.signal_connect = function(signal, callback) {
  callbacks_[signal] = callback;
  sendSyncMessage({ cmd: 'signal_connect', args: [signal] });
};
""",
        "custom_c": """
//signal_connect custom binding
extern void dcore_signal_connect(const char* type, JSCallback* value);
void handle_signal_connect(XW_Instance instance, json_object* msg) {
  struct json_object* args;
  json_object_object_get_ex(msg, "args", &args);
  struct json_object* arg_obj_0 = json_object_array_get_idx(args, 0);
  const char* arg_0 = json_object_get_string(arg_obj_0);

  JSCallback* callback = malloc(sizeof(JSCallback));
  callback->xw_instance = instance;
  dcore_signal_connect(arg_0, callback);
  struct json_object* ret = json_object_new_object();
  sync_messaging_interface->SetSyncReply(instance, json_object_to_json_string(ret));
  json_object_put(ret);
}
""",
    }
}
