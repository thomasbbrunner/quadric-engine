/*** MAIN ***/

#include "common.vert"

void main() 
{ 
    globals();
    
    vec4 position = vec4(coords, 1.0);

    gl_Position = aspect * proj * view * model * position;

    lighting(position);
}