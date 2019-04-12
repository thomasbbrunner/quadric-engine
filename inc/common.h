#ifndef COMMON_H
#define COMMON_H

#ifdef OPENGL_ES // Including files for OpenGL ES
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#else // Including files for OpenGL
#include <glad/glad.h>
#endif

#include <cstdlib>
#include <stdio.h>
#include <signal.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>
#include <print.h>
#include <time.h>
#include <camera.h>
#include <opengl.h>
#include <video.h>
#include <geometry.h>
#include <raw.h>
#include <buffer.h>
#include <geometry2D.h>
#include <geometry3D.h>
#include <shader.h>
#include <draw.h>
#include <thing.h>
#include <draw.h>
#include <light.h>


#endif