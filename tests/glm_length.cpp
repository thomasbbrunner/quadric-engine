#include <stdio.h>
#include <vector>

#include <glm/glm.hpp>

int main()
{
    glm::vec3 vec1 = {1.0f, 5.0f, 97.222f};
    glm::vec3 vec2 = {5.0, 100.55f, 200.69f};

    printf("%.5f %.5f %.5f\n", (vec1-vec2).x, (vec1-vec2).y, (vec1-vec2).z);
    printf("%.5f\n", glm::length(vec1-vec2));
    printf("%.5f\n", glm::length(vec2-vec1));

    return 0;
}