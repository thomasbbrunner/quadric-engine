#ifndef API_H
#define API_H

#ifdef OPENGL_ES // Including files for OpenGL ES
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#else // Including files for OpenGL
#include <glad/glad.h>
#endif

#endif