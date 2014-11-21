function sendSyncMessage(msg) {
  return JSON.parse(extension.internal.sendSyncMessage(JSON.stringify(msg)));
}
{% for func in module.functions %}
{%- if is_custom_function(func) %}
{{module.custom_binding[func.name]['custom_js']}}
{% else %}
exports.{{func.name}} = function() {
  var ret = sendSyncMessage({ cmd: '{{func.name}}', args: Array.prototype.slice.call(arguments, 0) });
  return ret.data;
};
{% endif -%}
{% endfor %}
