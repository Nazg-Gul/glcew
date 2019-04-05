#!/usr/bin/env python

from __future__ import print_function

from clang.cindex import *
import os
import sys

###############################################################################
# List of function to wrangle.

# Those functions will have regular warngler logic, based on simple function
# pinter read from the library.
DYNAMIC_FUNCTIONS = (
    # NOTE: glXGetProcAddress seems to be a variable of function type,
    #       which is not really supported by our Clang based parser yet.
    # "glXGetProcAddress",
)

# Those functions will be read from library into a function pointer variable,
# which name will be suffixed with _impl.
#
# The wrapper function will be created for all those functions, which will
# ensure that external libraries don't need to be aware of those functions
# being dynamically load function pointers.
WRAPPER_FUNCTIONS = (
    # NOTE: Use them as wrappers because GLEW links against them.
    "glXGetProcAddressARB",
    "glXGetClientString",
    "glXQueryVersion",
    "glXChooseVisual",

    "glBindTexture",
    "glBlendFunc",
    "glClear",
    "glClearColor",
    "glDeleteTextures",
    "glDepthFunc",
    "glDisable",
    "glDrawArrays",
    "glDrawElements",
    "glEnable",
    "glGenTextures",
    "glGetBooleanv",
    "glGetDoublev",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetString",
    "glGetTexImage",
    "glGetTexLevelParameteriv",
    "glPixelStorei",
    "glPolygonMode",
    "glScissor",
    "glTexImage2D",
    "glTexParameteri",
    "glViewport",
    "glIsEnabled",
    "glFlush",
)

# Those functions are exposed to API as a function pointers, but are read from
# library using specified function.
GETPROCADDR_FUNCTIONS = {
    "glXGetProcAddress" : (
        # GLX 1.0
        "glXCreateContext",
        "glXDestroyContext",
        "glXMakeCurrent",
        "glXSwapBuffers",
        "glXQueryExtension",
        # "glXQueryVersion",
        "glXGetCurrentContext",
        "glXGetCurrentDrawable",
        "glXWaitX",
        "glXWaitGL",
        # GLX 1.1
        # "glXGetClientString",
        "glXQueryExtensionsString",
    ),
}

###############################################################################
# Parsing


def formatAndCleanType(type):
    """
    Format data type to code style closer to what we want.
    """
    type = type.replace(" *", "* ")
    type = type.strip()
    if type.startswith("struct ") and not type.endswith("*"):
        type = type[7:]
    return type


def mergeTypeAndVariable(type, variable):
    """
    Combines type and variable to a single line of code.
    """
    dimension = ""
    type = type.strip()
    while type.endswith(']'):
        index = type.rfind('[')
        dimension = type[index:] + dimension
        type = type[:index]
    return "{} {}{}". format(type.strip(), variable, dimension)


class Argument:
    """
    Function argument.
    """
    def __init__(self, cursor):
        """
        Construct higher level function argument declaration
        from CLang's cursor.
        """
        self.name = cursor.spelling
        self.type = formatAndCleanType(cursor.type.spelling)

    def __str__(self):
        return mergeTypeAndVariable(self.type, self.name)


class Function:
    """
    Function declaration.
    """
    def __init__(self, cursor):
        """
        Construct higher level function declaration from CLang's cursor.
        """
        self.return_type = "void"
        self.name = cursor.spelling
        self.return_type = formatAndCleanType(cursor.type.get_result().spelling)
        self.arguments = []
        for child in cursor.get_arguments():
            if child.kind == CursorKind.PARM_DECL:
                argument = Argument(child)
                if not argument.name:
                    argument.name = "arg{}" . format(len(self.arguments) + 1)
                self.arguments.append(argument)

    def __str__(self):
        result = "{} {}" . format(self.return_type, self.name)
        for argument in self.arguments:
            result += "\n" + str(argument)
        return result


def isGetProcAddrFunction(name):
    """
    Check whether given function name is found in GETPROCADDR_FUNCTIONS.
    """
    for func in GETPROCADDR_FUNCTIONS:
        if name in GETPROCADDR_FUNCTIONS[func]:
            return True
    return False


def collect_function_prototypes(tu):
    file_name = tu.cursor.spelling
    functions = []
    for cursor in tu.cursor.walk_preorder():
        # Skip all tokens which are coming from different file.
        if cursor.location.file is None:
            continue
        if cursor.location.file.name != file_name:
            continue
        if cursor.kind == CursorKind.FUNCTION_DECL:
            function = Function(cursor)
            if function.name in WRAPPER_FUNCTIONS:
                function.type = 'WRAPPER'
                functions.append(function)
            elif function.name in DYNAMIC_FUNCTIONS:
                function.type = 'DYNAMIC'
                functions.append(function)
            elif isGetProcAddrFunction(function.name):
                # TODO(sergey): Trying to use them as GetProcAddr makes
                # code incompatible with GLEW. But for projects without
                # GLEW having this will be really handy.
                # function.type = 'GETPROCADDR'
                function.type = 'WRAPPER'
                functions.append(function)
    return functions


def parse_file(file_name):
    idx = Index.create()
    args = ('-x', 'c-header')
    tu = idx.parse(file_name, args=args)
    functions = collect_function_prototypes(tu)
    return functions


###############################################################################
# Wrangler generation

def getFunctionsWithType(functions, type):
    """
    Get list of all functions of given type
    """
    typed_functions = []
    for function in functions:
        if function.type == type:
            typed_functions.append(function)
    return typed_functions


def generate_single_type_function_pointer_typedefs(functions):
    """
    Generate typedef for function pointers:
      typedef return_type (*tMyFunction)(arguments).
    """
    lines = []
    for function in functions:
        line = "typedef {} (*t{}) ({});" . format(
                function.return_type,
                function.name,
                ", ".join(str(arg) for arg in function.arguments))
        lines.append(line)
    return lines


def generate_function_pointer_typedefs(functions):
    """
    Same as above, but groups functions based on their type.
    """
    lines = []
    lines.append("/* Dynamic functions. */")
    lines.extend(generate_single_type_function_pointer_typedefs(
            getFunctionsWithType(functions, 'DYNAMIC')))
    lines.append("\n/* Functions with wrappers. */")
    lines.extend(generate_single_type_function_pointer_typedefs(
            getFunctionsWithType(functions, 'WRAPPER')))
    lines.append("\n/* Functions read using gl's GetProcAddr. */")
    lines.extend(generate_single_type_function_pointer_typedefs(
            getFunctionsWithType(functions, 'GETPROCADDR')))
    return lines


def generate_single_type_function_pointer_declarations(functions):
    """
    Generate lines "extern tFoo foo_impl;"
    """
    if not functions:
        return []
    lines = []
    suffix = "_impl" if functions[0].type == 'WRAPPER' else ""
    for function in functions:
        line = "extern t{} {}{};" . format(function.name, function.name, suffix)
        lines.append(line)
    return lines


def generate_function_pointer_declarations(functions):
    """
    Same as above, but groups functions based on their type.
    """
    lines = []
    lines.append("/* Dynamic functions. */")
    lines.extend(generate_single_type_function_pointer_declarations(
            getFunctionsWithType(functions, 'DYNAMIC')))
    lines.append("\n/* Functions with wrappers. */")
    lines.extend(generate_single_type_function_pointer_declarations(
            getFunctionsWithType(functions, 'WRAPPER')))
    lines.append("\n/* Functions read using gl's GetProcAddr. */")
    lines.extend(generate_single_type_function_pointer_declarations(
            getFunctionsWithType(functions, 'GETPROCADDR')))
    return lines


def generate_single_type_function_pointer_definitions(functions):
    """
    Generate lines "tFoo foo_impl;"
    """
    if not functions:
        return []
    lines = []
    suffix = "_impl" if functions[0].type == 'WRAPPER' else ""
    for function in functions:
        line = "t{} {}{};" . format(function.name, function.name, suffix)
        lines.append(line)
    return lines


def generate_function_pointer_definitions(functions):
    """
    Same as above, but groups functions based on their type.
    """
    lines = []
    lines.append("/* Dynamic functions. */")
    lines.extend(generate_single_type_function_pointer_definitions(
            getFunctionsWithType(functions, 'DYNAMIC')))
    lines.append("\n/* Functions with wrappers. */")
    lines.extend(generate_single_type_function_pointer_definitions(
            getFunctionsWithType(functions, 'WRAPPER')))
    lines.append("\n/* Functions read using gl's GetProcAddr. */")
    lines.extend(generate_single_type_function_pointer_definitions(
            getFunctionsWithType(functions, 'GETPROCADDR')))
    return lines


def generate_wrapper_declarations(functions):
    """
    Generate wrapper function declarations.
    Those declarations actually matches functions from GL headers.
    """
    lines = []
    for function in functions:
        if function.type != 'WRAPPER':
            continue
        line = ""
        line += "{} {}" . format(formatAndCleanType(function.return_type),
                                 function.name)
        arguments = []
        argument_names = []
        for argument in function.arguments:
            arguments.append(str(argument))
            argument_names.append(argument.name)
        line += "({})" . format(", " . join(arguments)) + ";"
        lines.append(line)
    return lines


def generate_wrapper_implementations(functions):
    """
    Genrate function wrappers, which passes call to a dynload symbol.
    """
    lines = []
    for function in functions:
        if function.type != 'WRAPPER':
            continue
        line = ""
        if lines:
            line += "\n"
        line += "{} {}" . format(formatAndCleanType(function.return_type),
                                 function.name)
        arguments = []
        argument_names = []
        for argument in function.arguments:
            arguments.append(str(argument))
            argument_names.append(argument.name)
        line += "({})" . format(", " . join(arguments)) + " {\n"
        line += "  return {}_impl({});\n" . format(
                function.name,
                ", " . join(argument_names))
        line += "}"
        lines.append(line)
    return lines


def generate_single_type_dynload_calls(functions):
    """
    Generate lines which reads all functions from dynamic library.
    """
    if not functions:
        return []
    lines = []
    suffix = "_IMPL" if functions[0].type == 'WRAPPER' else ""
    for function in functions:
        line = "  GL_LIBRARY_FIND{}({});" . format(suffix, function.name)
        lines.append(line)
    return lines


def generate_dynload_calls(functions):
    """
    Same as above, but groups functions based on their type.
    """
    lines = []
    lines.append("  /* Dynamic functions. */")
    lines.extend(generate_single_type_dynload_calls(
            getFunctionsWithType(functions, 'DYNAMIC')))
    lines.append("\n  /* Functions with wrappers. */")
    lines.extend(generate_single_type_dynload_calls(
            getFunctionsWithType(functions, 'WRAPPER')))
    return lines


def add_functions_to_wrangler(header, wrangler, functions):
    # Function pointers, for things which we dlsym().
    pointer_typedefs = generate_function_pointer_typedefs(functions)
    function_pointer_declarations = \
            generate_function_pointer_declarations(functions)
    function_pointer_definitions = \
            generate_function_pointer_definitions(functions)
    dynload = generate_dynload_calls(functions)
    wrangler["functions"]["pointer_typedefs"].extend(pointer_typedefs)
    wrangler["functions"]["pointer_declarations"].extend(
            function_pointer_declarations)
    wrangler["functions"]["pointer_definitions"].extend(
            function_pointer_definitions)
    # Wrappers which passes API call to dynamically loaded symbol.
    wrapper_declarations = generate_wrapper_declarations(functions)
    wrapper_implementations = generate_wrapper_implementations(functions)
    wrangler["functions"]["wrapper_declarations"].extend(wrapper_declarations)
    wrangler["functions"]["wrapper_implementations"].extend(
            wrapper_implementations)

    wrangler["functions"]["dynload"].extend(dynload)


def replace_template_variables(wrangler, data):
    """
    Replace variables like %foo% in template data with actual code.
    """
    for group in wrangler:
        for variable in wrangler[group]:
            template_variable = "%{}_{}%" . format(group, variable)
            data = data.replace(template_variable,
                                "\n" . join(wrangler[group][variable]))
    return data


def write_wrangler_to_file(wrangler, template, destination):
    """
    Write wrangler symbols to a template.
    """
    with open(template, "r") as input:
        data = input.read()
        data = replace_template_variables(wrangler, data)
        with open(destination, "w") as output:
            output.write(data)


def write_wrangler_to_files(wrangler):
    """
    Write generated data from wrangler context to actual source files.
    """
    path = os.path.dirname(os.path.realpath(__file__))
    write_wrangler_to_file(
            wrangler,
            os.path.join(path, "glcew.template.h"),
            os.path.join(path, "..", "include", "glcew.h"))
    write_wrangler_to_file(
            wrangler,
            os.path.join(path, "glcew.template.c"),
            os.path.join(path, "..", "source", "glcew.c"))

###############################################################################
# Main logic


if __name__ == "__main__":
    headers = ("/usr/include/GL/gl.h",
               "/usr/include/GL/glx.h",
              )
    if len(sys.argv) == 2:
        headers = [sys.argv[1]]
    wrangler = {
        "functions": {
            "pointer_typedefs": [],
            "pointer_declarations": [],
            "pointer_definitions": [],
            "wrapper_declarations": [],
            "wrapper_implementations": [],
            "dynload": [],
        },
    }
    functions = []
    for header in headers:
        functions.extend(parse_file(header))
    add_functions_to_wrangler(header, wrangler, functions)
    write_wrangler_to_files(wrangler)
