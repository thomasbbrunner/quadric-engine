/*** MAIN ***/

void main() 
{  
    globals();

    vec3 vertex_data = vec3(texelFetch(tex_ver_data, ivec2(gl_VertexID,0), 0));
    vec4 position;

    // if (length(vertex_data-vec4(coords,1.0)) > 0.1)
    // {
    //     position = vec4((((coords))), 1.0);
    //     position.y += 10.0;
    // }
    // else
    // {
        position = vec4(noise((bring(vertex_data))), 1.0);
    // }
    // position = vertex_data;

    gl_Position = aspect * proj * view * model * position;

    lighting(position);
}