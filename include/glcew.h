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

/* Dynamic functions. */

/* Functions with wrappers. */
typedef void (*tglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (*tglClear) (GLbitfield mask);
typedef void (*tglBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void (*tglPolygonMode) (GLenum face, GLenum mode);
typedef void (*tglScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*tglDrawBuffer) (GLenum mode);
typedef void (*tglReadBuffer) (GLenum mode);
typedef void (*tglEnable) (GLenum cap);
typedef void (*tglDisable) (GLenum cap);
typedef GLboolean (*tglIsEnabled) (GLenum cap);
typedef void (*tglGetBooleanv) (GLenum pname, GLboolean* params);
typedef void (*tglGetDoublev) (GLenum pname, GLdouble* params);
typedef void (*tglGetFloatv) (GLenum pname, GLfloat* params);
typedef void (*tglGetIntegerv) (GLenum pname, GLint* params);
typedef const GLubyte* (*tglGetString) (GLenum name);
typedef void (*tglFinish) ();
typedef void (*tglFlush) ();
typedef void (*tglDepthFunc) (GLenum func);
typedef void (*tglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*tglDrawArrays) (GLenum mode, GLint first, GLsizei count);
typedef void (*tglDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
typedef void (*tglPixelStorei) (GLenum pname, GLint param);
typedef void (*tglReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
typedef void (*tglTexParameteri) (GLenum target, GLenum pname, GLint param);
typedef void (*tglGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (*tglTexImage2D) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (*tglGetTexImage) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels);
typedef void (*tglGenTextures) (GLsizei n, GLuint* textures);
typedef void (*tglDeleteTextures) (GLsizei n, const GLuint* textures);
typedef void (*tglBindTexture) (GLenum target, GLuint texture);
typedef XVisualInfo* (*tglXChooseVisual) (Display* dpy, int screen, int* attribList);
typedef GLXContext (*tglXCreateContext) (Display* dpy, XVisualInfo* vis, GLXContext shareList, int direct);
typedef void (*tglXDestroyContext) (Display* dpy, GLXContext ctx);
typedef int (*tglXMakeCurrent) (Display* dpy, GLXDrawable drawable, GLXContext ctx);
typedef void (*tglXSwapBuffers) (Display* dpy, GLXDrawable drawable);
typedef int (*tglXQueryExtension) (Display* dpy, int* errorb, int* event);
typedef int (*tglXQueryVersion) (Display* dpy, int* maj, int* min);
typedef GLXContext (*tglXGetCurrentContext) ();
typedef GLXDrawable (*tglXGetCurrentDrawable) ();
typedef void (*tglXWaitGL) ();
typedef void (*tglXWaitX) ();
typedef const char* (*tglXQueryExtensionsString) (Display* dpy, int screen);
typedef const char* (*tglXGetClientString) (Display* dpy, int name);
typedef __GLXextFuncPtr (*tglXGetProcAddressARB) (const GLubyte* arg1);

/* Functions read using gl's GetProcAddr. */

/* Function pointer declarations.
 *
 * Pointer to functions which are dynamically loaded from the library.
 */

/* Dynamic functions. */

/* Functions with wrappers. */
extern tglClearColor glClearColor_impl;
extern tglClear glClear_impl;
extern tglBlendFunc glBlendFunc_impl;
extern tglPolygonMode glPolygonMode_impl;
extern tglScissor glScissor_impl;
extern tglDrawBuffer glDrawBuffer_impl;
extern tglReadBuffer glReadBuffer_impl;
extern tglEnable glEnable_impl;
extern tglDisable glDisable_impl;
extern tglIsEnabled glIsEnabled_impl;
extern tglGetBooleanv glGetBooleanv_impl;
extern tglGetDoublev glGetDoublev_impl;
extern tglGetFloatv glGetFloatv_impl;
extern tglGetIntegerv glGetIntegerv_impl;
extern tglGetString glGetString_impl;
extern tglFinish glFinish_impl;
extern tglFlush glFlush_impl;
extern tglDepthFunc glDepthFunc_impl;
extern tglViewport glViewport_impl;
extern tglDrawArrays glDrawArrays_impl;
extern tglDrawElements glDrawElements_impl;
extern tglPixelStorei glPixelStorei_impl;
extern tglReadPixels glReadPixels_impl;
extern tglTexParameteri glTexParameteri_impl;
extern tglGetTexLevelParameteriv glGetTexLevelParameteriv_impl;
extern tglTexImage2D glTexImage2D_impl;
extern tglGetTexImage glGetTexImage_impl;
extern tglGenTextures glGenTextures_impl;
extern tglDeleteTextures glDeleteTextures_impl;
extern tglBindTexture glBindTexture_impl;
extern tglXChooseVisual glXChooseVisual_impl;
extern tglXCreateContext glXCreateContext_impl;
extern tglXDestroyContext glXDestroyContext_impl;
extern tglXMakeCurrent glXMakeCurrent_impl;
extern tglXSwapBuffers glXSwapBuffers_impl;
extern tglXQueryExtension glXQueryExtension_impl;
extern tglXQueryVersion glXQueryVersion_impl;
extern tglXGetCurrentContext glXGetCurrentContext_impl;
extern tglXGetCurrentDrawable glXGetCurrentDrawable_impl;
extern tglXWaitGL glXWaitGL_impl;
extern tglXWaitX glXWaitX_impl;
extern tglXQueryExtensionsString glXQueryExtensionsString_impl;
extern tglXGetClientString glXGetClientString_impl;
extern tglXGetProcAddressARB glXGetProcAddressARB_impl;

/* Functions read using gl's GetProcAddr. */

/* Wrapper function declarations.
 *
 * Used for functions which are not really supposed to be loaded dynamically,
 * due to possible dependencies not being aware of them being a function
 * pointer.
 */

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void glClear(GLbitfield mask);
void glBlendFunc(GLenum sfactor, GLenum dfactor);
void glPolygonMode(GLenum face, GLenum mode);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glDrawBuffer(GLenum mode);
void glReadBuffer(GLenum mode);
void glEnable(GLenum cap);
void glDisable(GLenum cap);
GLboolean glIsEnabled(GLenum cap);
void glGetBooleanv(GLenum pname, GLboolean* params);
void glGetDoublev(GLenum pname, GLdouble* params);
void glGetFloatv(GLenum pname, GLfloat* params);
void glGetIntegerv(GLenum pname, GLint* params);
const GLubyte* glGetString(GLenum name);
void glFinish();
void glFlush();
void glDepthFunc(GLenum func);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glDrawArrays(GLenum mode, GLint first, GLsizei count);
void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
void glPixelStorei(GLenum pname, GLint param);
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint* params);
void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels);
void glGenTextures(GLsizei n, GLuint* textures);
void glDeleteTextures(GLsizei n, const GLuint* textures);
void glBindTexture(GLenum target, GLuint texture);
XVisualInfo* glXChooseVisual(Display* dpy, int screen, int* attribList);
GLXContext glXCreateContext(Display* dpy, XVisualInfo* vis, GLXContext shareList, int direct);
void glXDestroyContext(Display* dpy, GLXContext ctx);
int glXMakeCurrent(Display* dpy, GLXDrawable drawable, GLXContext ctx);
void glXSwapBuffers(Display* dpy, GLXDrawable drawable);
int glXQueryExtension(Display* dpy, int* errorb, int* event);
int glXQueryVersion(Display* dpy, int* maj, int* min);
GLXContext glXGetCurrentContext();
GLXDrawable glXGetCurrentDrawable();
void glXWaitGL();
void glXWaitX();
const char* glXQueryExtensionsString(Display* dpy, int screen);
const char* glXGetClientString(Display* dpy, int name);
__GLXextFuncPtr glXGetProcAddressARB(const GLubyte* arg1);

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
