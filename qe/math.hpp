
#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace quad
{
namespace math
{
void add_to_each(std::vector<unsigned int> *vec, unsigned int value);

float random(float min, float max);

void normalize(std::vector<glm::vec3> *vertices);

void multiply_by_scalar(std::vector<glm::vec3> *vertices, float d);

float linear_interpolation(float value, const std::vector<float> *range, const std::vector<float> *data);

} // namespace math
} // namespace quad
