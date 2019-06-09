#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include "print.h"

int main()
{
    glm::vec3 vec(25.1f, 11.0f, -22.0f);

    Print::vec(vec);

    auto res = glm::perlin(vec);

    std::cout << res << std::endl;

    return 0;
}