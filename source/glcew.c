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

#ifdef _MSC_VER
#  if _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#  define popen _popen
#  define pclose _pclose
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <glcew.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define VC_EXTRALEAN
#  include <windows.h>

/* Utility macros. */

typedef HMODULE DynamicLibrary;

#  define dynamic_library_open(path)         LoadLibraryA(path)
#  define dynamic_library_close(lib)         FreeLibrary(lib)
#  define dynamic_library_find(lib, symbol)  GetProcAddress(lib, symbol)
#else
#  include <dlfcn.h>

typedef void* DynamicLibrary;

#  define dynamic_library_open(path)         dlopen(path, RTLD_NOW)
#  define dynamic_library_close(lib)         dlclose(lib)
#  define dynamic_library_find(lib, symbol)  dlsym(lib, symbol)
#endif

#define GLUE_IMPL(A, B) A ## B
#define GLUE(A, B) GLUE_IMPL(A, B)

#define IMPL_SUFFIX _impl

#define _LIBRARY_FIND_CHECKED(lib, name)                     \
        do {                                                 \
          name = (t##name)dynamic_library_find(lib, #name);  \
          assert(name);                                      \
        } while (0)

#define _LIBRARY_FIND(lib, name)                             \
        do {                                                 \
          name = (t##name)dynamic_library_find(lib, #name);  \
        } while (0)

#define _LIBRARY_FIND_IMPL_CHECKED(lib, name)                                  \
        do {                                                                   \
          GLUE(name, IMPL_SUFFIX) = (t##name)dynamic_library_find(lib, #name); \
          assert(name);                                                        \
        } while (0)

#define _LIBRARY_FIND_IMPL(lib, name)                                          \
        do {                                                                   \
          GLUE(name, IMPL_SUFFIX) = (t##name)dynamic_library_find(lib, #name); \
        } while (0)

#define GL_LIBRARY_FIND_CHECKED(name) _LIBRARY_FIND_CHECKED(gl_lib, name)
#define GL_LIBRARY_FIND(name) _LIBRARY_FIND(gl_lib, name)

#define GL_LIBRARY_FIND_IMPL_CHECKED(name) \
        _LIBRARY_FIND_IMPL_CHECKED(gl_lib, name)
#define GL_LIBRARY_FIND_IMPL(name) _LIBRARY_FIND_IMPL(gl_lib, name)

static DynamicLibrary gl_lib;

/* ************************ Function definitions. ************************ */

/* Dynamic functions. */

/* Functions with wrappers. */
tglClearColor glClearColor_impl;
tglClear glClear_impl;
tglBlendFunc glBlendFunc_impl;
tglPolygonMode glPolygonMode_impl;
tglScissor glScissor_impl;
tglDrawBuffer glDrawBuffer_impl;
tglReadBuffer glReadBuffer_impl;
tglEnable glEnable_impl;
tglDisable glDisable_impl;
tglIsEnabled glIsEnabled_impl;
tglGetBooleanv glGetBooleanv_impl;
tglGetDoublev glGetDoublev_impl;
tglGetFloatv glGetFloatv_impl;
tglGetIntegerv glGetIntegerv_impl;
tglGetString glGetString_impl;
tglFinish glFinish_impl;
tglFlush glFlush_impl;
tglDepthFunc glDepthFunc_impl;
tglViewport glViewport_impl;
tglDrawArrays glDrawArrays_impl;
tglDrawElements glDrawElements_impl;
tglPixelStorei glPixelStorei_impl;
tglReadPixels glReadPixels_impl;
tglTexParameteri glTexParameteri_impl;
tglGetTexLevelParameteriv glGetTexLevelParameteriv_impl;
tglTexImage2D glTexImage2D_impl;
tglGetTexImage glGetTexImage_impl;
tglGenTextures glGenTextures_impl;
tglDeleteTextures glDeleteTextures_impl;
tglBindTexture glBindTexture_impl;
tglXChooseVisual glXChooseVisual_impl;
tglXCreateContext glXCreateContext_impl;
tglXDestroyContext glXDestroyContext_impl;
tglXMakeCurrent glXMakeCurrent_impl;
tglXSwapBuffers glXSwapBuffers_impl;
tglXQueryExtension glXQueryExtension_impl;
tglXQueryVersion glXQueryVersion_impl;
tglXGetCurrentContext glXGetCurrentContext_impl;
tglXGetCurrentDrawable glXGetCurrentDrawable_impl;
tglXWaitGL glXWaitGL_impl;
tglXWaitX glXWaitX_impl;
tglXQueryExtensionsString glXQueryExtensionsString_impl;
tglXGetClientString glXGetClientString_impl;
tglXGetProcAddressARB glXGetProcAddressARB_impl;

/* Functions read using gl's GetProcAddr. */

/* ************************** Function wrappers. ************************* */

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
  return glClearColor_impl(red, green, blue, alpha);
}

void glClear(GLbitfield mask) {
  return glClear_impl(mask);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
  return glBlendFunc_impl(sfactor, dfactor);
}

void glPolygonMode(GLenum face, GLenum mode) {
  return glPolygonMode_impl(face, mode);
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
  return glScissor_impl(x, y, width, height);
}

void glDrawBuffer(GLenum mode) {
  return glDrawBuffer_impl(mode);
}

void glReadBuffer(GLenum mode) {
  return glReadBuffer_impl(mode);
}

void glEnable(GLenum cap) {
  return glEnable_impl(cap);
}

void glDisable(GLenum cap) {
  return glDisable_impl(cap);
}

GLboolean glIsEnabled(GLenum cap) {
  return glIsEnabled_impl(cap);
}

void glGetBooleanv(GLenum pname, GLboolean* params) {
  return glGetBooleanv_impl(pname, params);
}

void glGetDoublev(GLenum pname, GLdouble* params) {
  return glGetDoublev_impl(pname, params);
}

void glGetFloatv(GLenum pname, GLfloat* params) {
  return glGetFloatv_impl(pname, params);
}

void glGetIntegerv(GLenum pname, GLint* params) {
  return glGetIntegerv_impl(pname, params);
}

const GLubyte* glGetString(GLenum name) {
  return glGetString_impl(name);
}

void glFinish() {
  return glFinish_impl();
}

void glFlush() {
  return glFlush_impl();
}

void glDepthFunc(GLenum func) {
  return glDepthFunc_impl(func);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  return glViewport_impl(x, y, width, height);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
  return glDrawArrays_impl(mode, first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {
  return glDrawElements_impl(mode, count, type, indices);
}

void glPixelStorei(GLenum pname, GLint param) {
  return glPixelStorei_impl(pname, param);
}

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) {
  return glReadPixels_impl(x, y, width, height, format, type, pixels);
}

void glTexParameteri(GLenum target, GLenum pname, GLint param) {
  return glTexParameteri_impl(target, pname, param);
}

void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint* params) {
  return glGetTexLevelParameteriv_impl(target, level, pname, params);
}

void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) {
  return glTexImage2D_impl(target, level, internalFormat, width, height, border, format, type, pixels);
}

void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels) {
  return glGetTexImage_impl(target, level, format, type, pixels);
}

void glGenTextures(GLsizei n, GLuint* textures) {
  return glGenTextures_impl(n, textures);
}

void glDeleteTextures(GLsizei n, const GLuint* textures) {
  return glDeleteTextures_impl(n, textures);
}

void glBindTexture(GLenum target, GLuint texture) {
  return glBindTexture_impl(target, texture);
}

XVisualInfo* glXChooseVisual(Display* dpy, int screen, int* attribList) {
  return glXChooseVisual_impl(dpy, screen, attribList);
}

GLXContext glXCreateContext(Display* dpy, XVisualInfo* vis, GLXContext shareList, int direct) {
  return glXCreateContext_impl(dpy, vis, shareList, direct);
}

void glXDestroyContext(Display* dpy, GLXContext ctx) {
  return glXDestroyContext_impl(dpy, ctx);
}

int glXMakeCurrent(Display* dpy, GLXDrawable drawable, GLXContext ctx) {
  return glXMakeCurrent_impl(dpy, drawable, ctx);
}

void glXSwapBuffers(Display* dpy, GLXDrawable drawable) {
  return glXSwapBuffers_impl(dpy, drawable);
}

int glXQueryExtension(Display* dpy, int* errorb, int* event) {
  return glXQueryExtension_impl(dpy, errorb, event);
}

int glXQueryVersion(Display* dpy, int* maj, int* min) {
  return glXQueryVersion_impl(dpy, maj, min);
}

GLXContext glXGetCurrentContext() {
  return glXGetCurrentContext_impl();
}

GLXDrawable glXGetCurrentDrawable() {
  return glXGetCurrentDrawable_impl();
}

void glXWaitGL() {
  return glXWaitGL_impl();
}

void glXWaitX() {
  return glXWaitX_impl();
}

const char* glXQueryExtensionsString(Display* dpy, int screen) {
  return glXQueryExtensionsString_impl(dpy, screen);
}

const char* glXGetClientString(Display* dpy, int name) {
  return glXGetClientString_impl(dpy, name);
}

__GLXextFuncPtr glXGetProcAddressARB(const GLubyte* arg1) {
  return glXGetProcAddressARB_impl(arg1);
}

/* ************************ Main wrangling logic. ************************ */

static DynamicLibrary dynamic_library_open_find(const char** paths) {
  int i = 0;
  while (paths[i] != NULL) {
      DynamicLibrary lib = dynamic_library_open(paths[i]);
      if (lib != NULL) {
        return lib;
      }
      ++i;
  }
  return NULL;
}

static void glcewExit(void) {
  if (gl_lib != NULL) {
    /*  Ignore errors. */
    dynamic_library_close(gl_lib);
    gl_lib = NULL;
  }
}

int glcewInit(void) {
  /* Library paths. */
#ifdef _WIN32
  const char* gl_paths[] = {"opengl32.dll"};
#elif defined(__APPLE__)
  const char* gl_paths[] = {NULL};
#else
  /* TODO(sergey): Check on an order. Angular does other way around. */
  const char* gl_paths[] = {"libGL.so",
                            "libGL.so.1",
                            NULL};
#endif
  static int initialized = 0;
  static int result = 0;
  int error;

  if (initialized) {
    return result;
  }

  initialized = 1;

  error = atexit(glcewExit);
  if (error) {
    result = GLCEW_ERROR_ATEXIT_FAILED;
    return result;
  }

  /* Load library. */
  gl_lib = dynamic_library_open_find(gl_paths);
  if (gl_lib == NULL) {
    result = GLCEW_ERROR_OPEN_FAILED;
    return result;
  }

  /* Fetch all function pointers. */

  /* Dynamic functions. */

  /* Functions with wrappers. */
  GL_LIBRARY_FIND_IMPL(glClearColor);
  GL_LIBRARY_FIND_IMPL(glClear);
  GL_LIBRARY_FIND_IMPL(glBlendFunc);
  GL_LIBRARY_FIND_IMPL(glPolygonMode);
  GL_LIBRARY_FIND_IMPL(glScissor);
  GL_LIBRARY_FIND_IMPL(glDrawBuffer);
  GL_LIBRARY_FIND_IMPL(glReadBuffer);
  GL_LIBRARY_FIND_IMPL(glEnable);
  GL_LIBRARY_FIND_IMPL(glDisable);
  GL_LIBRARY_FIND_IMPL(glIsEnabled);
  GL_LIBRARY_FIND_IMPL(glGetBooleanv);
  GL_LIBRARY_FIND_IMPL(glGetDoublev);
  GL_LIBRARY_FIND_IMPL(glGetFloatv);
  GL_LIBRARY_FIND_IMPL(glGetIntegerv);
  GL_LIBRARY_FIND_IMPL(glGetString);
  GL_LIBRARY_FIND_IMPL(glFinish);
  GL_LIBRARY_FIND_IMPL(glFlush);
  GL_LIBRARY_FIND_IMPL(glDepthFunc);
  GL_LIBRARY_FIND_IMPL(glViewport);
  GL_LIBRARY_FIND_IMPL(glDrawArrays);
  GL_LIBRARY_FIND_IMPL(glDrawElements);
  GL_LIBRARY_FIND_IMPL(glPixelStorei);
  GL_LIBRARY_FIND_IMPL(glReadPixels);
  GL_LIBRARY_FIND_IMPL(glTexParameteri);
  GL_LIBRARY_FIND_IMPL(glGetTexLevelParameteriv);
  GL_LIBRARY_FIND_IMPL(glTexImage2D);
  GL_LIBRARY_FIND_IMPL(glGetTexImage);
  GL_LIBRARY_FIND_IMPL(glGenTextures);
  GL_LIBRARY_FIND_IMPL(glDeleteTextures);
  GL_LIBRARY_FIND_IMPL(glBindTexture);
  GL_LIBRARY_FIND_IMPL(glXChooseVisual);
  GL_LIBRARY_FIND_IMPL(glXCreateContext);
  GL_LIBRARY_FIND_IMPL(glXDestroyContext);
  GL_LIBRARY_FIND_IMPL(glXMakeCurrent);
  GL_LIBRARY_FIND_IMPL(glXSwapBuffers);
  GL_LIBRARY_FIND_IMPL(glXQueryExtension);
  GL_LIBRARY_FIND_IMPL(glXQueryVersion);
  GL_LIBRARY_FIND_IMPL(glXGetCurrentContext);
  GL_LIBRARY_FIND_IMPL(glXGetCurrentDrawable);
  GL_LIBRARY_FIND_IMPL(glXWaitGL);
  GL_LIBRARY_FIND_IMPL(glXWaitX);
  GL_LIBRARY_FIND_IMPL(glXQueryExtensionsString);
  GL_LIBRARY_FIND_IMPL(glXGetClientString);
  GL_LIBRARY_FIND_IMPL(glXGetProcAddressARB);

  result = GLCEW_SUCCESS;
  return result;
}

const char* glcewErrorString(int error) {
  switch (error) {
    case GLCEW_SUCCESS: return "SUCCESS";
    case GLCEW_ERROR_OPEN_FAILED: return "OPEN_FAILED";
    case GLCEW_ERROR_ATEXIT_FAILED: return "ATEXIT_FAILED";
  }
  return "UNKNOWN";
}
