
#pragma once

#include "geometry.hpp"

class ModelBuffer
{
  public:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int TBO = 0;
    unsigned int VBO_texture = 0;
    unsigned int numelements = 0;
};

class Buffers
{
  public:
    ModelBuffer mesh;
    ModelBuffer contour;
    ModelBuffer dots;
};

class BufferGeneration
{
  public:
    static ModelBuffer generate_from_mesh(Model);

    static ModelBuffer generate_from_contour(Model);

    static ModelBuffer generate_from_dots(Model);
};
