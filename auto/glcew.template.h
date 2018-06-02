/*
 * Copyright 2018 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */

#ifndef __GLCEW_H__
#define __GLCEW_H__

#include <stdint.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Defines. */
#define GLCEW_VERSION_MAJOR 1
#define GLCEW_VERSION_MINOR 0

/* Types.
 *
 * "Hardcode" some types to decouple wrangler from any system wide header.
 */

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;  /* 1-byte signed */
typedef short GLshort;  /* 2-byte signed */
typedef int  GLint;  /* 4-byte signed */
typedef unsigned char GLubyte; /* 1-byte unsigned */
typedef unsigned short GLushort;  /* 2-byte unsigned */
typedef unsigned int GLuint;  /* 4-byte unsigned */
typedef int GLsizei;  /* 4-byte signed */
typedef float GLfloat;  /* single precision float */
typedef float GLclampf;  /* single precision float in [0,1] */
typedef double GLdouble;  /* double precision float */
typedef double GLclampd;  /* double precision float in [0,1] */

/* GLX specific types.
 *
 * TODO(sergey): Consider using glxew.h.
 */
typedef void (*__GLXextFuncPtr)(void);
typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXDrawable;
typedef XID GLXPixmap;

/* Function pointer types.
 *
 * Those are types of functions which are read from the library by either
 * system-specific dynamic library lookup (such as glsym), or by OpenGL
 * context specific pointer lookup (such as glXGetProcAddress).
 */

%functions_pointer_typedefs%

/* Function pointer declarations.
 *
 * Pointer to functions which are dynamically loaded from the library.
 */

%functions_pointer_declarations%

/* Wrapper function declarations.
 *
 * Used for functions which are not really supposed to be loaded dynamically,
 * due to possible dependencies not being aware of them being a function
 * pointer.
 */

%functions_wrapper_declarations%

/* ****************************************************************************
 * * GLCEW related API
 * */

enum {
  GLCEW_SUCCESS = 0,
  GLCEW_ERROR_OPEN_FAILED = -1,
  GLCEW_ERROR_ATEXIT_FAILED = -2,
};

int glcewInit(void);
const char* glcewErrorString(int error);

#ifdef __cplusplus
}
#endif

#endif  /* __GLCEW_H__ */
