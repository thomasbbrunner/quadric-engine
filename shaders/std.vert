/*** MAIN ***/

#include "common.vert"

void main() 
{ 
    globals();
    
    vec4 position = vec4(coords[0], coords[1]+3.0*snoise(vec3(vec2(coords/3.0), time)),coords[2], 1.0);

    gl_Position = aspect * proj * view * model * position;

    lighting(position);
}