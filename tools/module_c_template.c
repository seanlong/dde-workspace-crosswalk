#include <assert.h>
#include <stdio.h>

#include "XW_Extension.h"
#include "XW_Extension_SyncMessage.h"

#include "glib.h"
#include "glib-object.h"

#include "json-c/json.h"

#include "common/config.h" //FIXME

static XW_Extension xw_extension = 0;

const XW_CoreInterface* core_interface = NULL;
const XW_MessagingInterface* async_messaging_interface = NULL;
const XW_Internal_SyncMessagingInterface* sync_messaging_interface = NULL;

extern const char kSource_{{module.name}}_js_api[];

//FIXME GC corresponding C object when JS object is free.
ArrayContainer json_native_object_array_to_container(json_object* json_array) {
  int i;
  ArrayContainer ret;
  int array_length = json_object_array_length(json_array);
  void** array = g_new0(void*, array_length); // The C implementation should be responsible for free it.
  ret.num = array_length;
  ret.data = (void*)array;

  for (i = 0; i < array_length; ++i) {
    json_object *obj = json_object_array_get_idx(json_array, i);
    void* obj_ptr = (void*)json_object_get_int64(obj);
    array[i] = obj_ptr;
  }
  return ret;
}

json_object* container_to_json_native_object_array(ArrayContainer container) {
  int i;
  json_object* ret = json_object_new_array();
  void** data = (void**)container.data;

  for (i = 0; i < container.num; ++i)
    json_object_array_add(ret, json_object_new_int64((int64_t)data[i]));
  g_free(container.data);
  return ret;
}

{% for func in module.functions %}
{%- if is_custom_function(func) %}
{{module.custom_binding[func.name]['custom_c']}}
{% else %}
extern {{func.ret.c_type}} {{module.name.lower()}}_{{func.name}}(
  {%- for param in func.params -%}
    {%- if loop.index0 == func.params|count - 1 -%}
    {{param.c_type}}
    {%- else -%}
    {{param.c_type}},
    {%- endif -%}
  {%- endfor -%}
);
void handle_{{func.name}}(XW_Instance instance, json_object* msg) {
  {% if func.params|count -%}
  struct json_object* args; 
  json_object_object_get_ex(msg, "args", &args);
  {%- endif -%}
  {%- for param in func.params %}
  struct json_object* arg_obj_{{loop.index0}} = json_object_array_get_idx(args, {{loop.index0}});
    {%- if is_native_object_array(param) %}
  ArrayContainer arg_{{loop.index0}} = json_native_object_array_to_container(arg_obj_{{loop.index0}});
    {%- elif is_native_object(param) %}
  void* arg_{{loop.index0}} = (void*)json_object_get_int64(arg_obj_{{loop.index0}});
    {%- else %}
  {{param.c_type}} arg_{{loop.index0}} = json_object_get_{{param.json_type}}(arg_obj_{{loop.index0}});
    {%- endif -%}
  {%- endfor %}

  {% if is_null(func.ret) -%}
  {%- else -%}
  {{func.ret.c_type}} data = {% endif -%}
  {{module.name.lower()}}_{{func.name}}(
      {%- for i in range(func.params|count) -%}
        {%- if i == func.params|count - 1 -%}
      arg_{{i}}
        {%- else -%}
      arg_{{i}},
        {%- endif -%}
      {%- endfor -%});
  struct json_object* ret = json_object_new_object();
  {%- if is_object(func.ret) %}
  json_object_object_add(ret, "data", data);
  {%- elif is_native_object(func.ret) %}
  json_object_object_add(ret, "data", json_object_new_int64((int64_t)data));
  {%- elif is_native_object_array(func.ret) %}
  json_object_object_add(ret, "data", container_to_json_native_object_array(data));
  {%- elif is_null(func.ret) %}
  json_object_object_add(ret, "data", json_object_new_object());
  {%- else %}
  json_object_object_add(ret, "data", json_object_new_{{func.ret.json_type}}(data));
  {%- endif %}
  sync_messaging_interface->SetSyncReply(instance, json_object_to_json_string(ret));
  json_object_put(ret);
}
{% endif -%}
{% endfor %}

void handle_sync_message(XW_Instance instance, const char* msg) {
  //printf("%s====> %s\n", __FILE__, msg);
  json_object* obj = json_tokener_parse(msg);
  assert(obj && json_object_is_type(obj, json_type_object));

  json_object* cmd_obj;
  json_object_object_get_ex(obj, "cmd", &cmd_obj);
  const char* cmd = json_object_get_string(cmd_obj);
  {% for func in module.functions %}
    {%- if loop.index == 1 %}
  if (!strcmp(cmd, "{{func.name}}"))
    {%- else %}
  else if (!strcmp(cmd, "{{func.name}}"))
    {%- endif %}
    handle_{{func.name}}(instance, obj);
  {%- endfor %}
  else
    fprintf(stderr, "ASSERT NOT REACHED.\n");
  
  json_object_put(obj);
}

int32_t XW_Initialize(XW_Extension extension, XW_GetInterface get_interface) {
  xw_extension = extension;
  core_interface = get_interface(XW_CORE_INTERFACE);
  {%- if module.name == "DCore" %}
  core_interface->SetExtensionName(extension, "DCore");
  {% else %}
  core_interface->SetExtensionName(extension, "DCore.{{module.name}}");
  {% endif -%}
  core_interface->SetJavaScriptAPI(extension, kSource_{{module.name}}_js_api);

  async_messaging_interface = get_interface(XW_MESSAGING_INTERFACE);
  sync_messaging_interface = get_interface(XW_INTERNAL_SYNC_MESSAGING_INTERFACE);
  sync_messaging_interface->Register(extension, handle_sync_message);

  return XW_OK;
}
