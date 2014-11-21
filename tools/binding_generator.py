#!/usr/bin/python2

import os
import sys
import jinja2

class Description:
    def __init__(self, string):
        pass

class Param:
    c_type = "undefined"
    json_type = "undefined"

    def __init__(self, name=None, desc=None):
        self.name = name
        self.desc = desc

    def is_array(self):
        return False

class Array(Param):
    def is_array(self):
        return True 

class Boolean(Param):
    c_type = "gboolean"
    json_type = "boolean"

class Number(Param):
    c_type = "double"
    json_type = "double"

class String(Param):
    c_type = "const char*"
    json_type = "string"

class CString(String):
    c_type = "const char*"
    json_type = "string"

class Null(Param):
    c_type = "void"
    json_type = "object"

class NativeObject(Param):
    c_type = "void*"
    json_type = "int64"

class Object(Param):
    c_type = "json_object*"
    json_type = "object"

class Callback(Param):
    c_type = "int64_t"
    json_type = "int64"

class ABoolean(Array):
    c_type = "boolean*"
    json_type = "array"

class ANumber(Array):
    c_type = "double*"
    json_type = "array"

class AString(Array):
    c_type = "char**"
    json_type = "array"

class ANativeObject(Array):
    c_type = "ArrayContainer"
    json_type = "array"

class Function:
    def __init__(self, name, ret, *params):
        self.name = name
        self.ret = ret
        self.params = params

class CustomFunction(Function):
    pass

class Class:
    def __init__(self, name, desc=None, *args):
        self.name = name
        self.description = desc
        self.properties = args
        self.functions = [ f for f in self.properties if isinstance(f, Function) ]
        custom_file_path = os.path.join(
                os.path.dirname(os.path.realpath(__file__)), name.lower() + '_custom_bindings.py')
        if os.path.exists(custom_file_path):
            with open(custom_file_path) as custom_binding_file:
                custom_binding = custom_binding_file.read()
                self.custom_binding = eval(custom_binding)

    def generate_js_wrapper(self):
        template = env.get_template('module_js_template.js')
        return template.render(module = self)

    def generate_c_wrapper(self):
        template = env.get_template('module_c_template.c')
        return template.render(module = self)


env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(os.path.dirname(os.path.realpath(__file__))))
env.globals['is_custom_function'] = lambda v: isinstance(v, CustomFunction)
env.globals['is_native_object'] = lambda v: isinstance(v, NativeObject)
env.globals['is_native_object_array'] = lambda v: isinstance(v, ANativeObject) 
env.globals['is_null'] = lambda v: isinstance(v, Null)
env.globals['is_object'] = lambda v: isinstance(v, Object)

def generate_c_string_for_js(js_file_path, module):
    template = """\
extern const char %s[];
const char %s[] = { %s, 0 };
"""
    with open(js_file_path) as js_file:
        lines = js_file.read()
        c_code = ', '.join(str(ord(c)) for c in lines)
        file_path = js_file_path.split('.')[0] + '.c'
        with open(file_path, 'w') as output:
            var_name = 'kSource_' + module.name + '_js_api'
            output.write(template % (var_name, var_name, c_code))

def generate_bindings(cfg_path, output_dir):
    cfg = os.path.basename(cfg_path)
    assert(cfg.split('.')[1] == 'cfg')
    module_name = cfg.split('.')[0]
    with open(cfg_path) as f:
        content = f.read()
        module = eval(content)
        js_file_path = os.path.join(output_dir, module_name + '_js_api.js')
        c_file_path = os.path.join(output_dir, module_name + '_c_api.c')
        with open(js_file_path, 'w') as js_wrapper:
            js_wrapper.write(module.generate_js_wrapper())
        with open(c_file_path, 'w') as c_wrapper:
            c_wrapper.write(module.generate_c_wrapper())
        generate_c_string_for_js(js_file_path, module)


#usage: ./binding_generator.py test.cfg ../output/
if __name__ == "__main__":
    cfg_file = sys.argv[1]
    output_dir = sys.argv[2]
    print output_dir
    generate_bindings(cfg_file, output_dir)

