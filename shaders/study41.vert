/*** MAIN ***/

vec3 effects(vec3 newcoords)
{
    return ((((newcoords))));
}

void main() 
{  
    globals();

    vec3 vertex_data = coords;//effects(vec3(texelFetch(tex_ver_data, gl_VertexID)));
    // vec3 adj_vertex_data[7];

    // int mesh_dim = (tube_dx) * 8; // Size of mesh times resolution and something else? if lighting is strange, change here

    // adj_vertex_data[0] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID - 1)));
    // globals();
    // adj_vertex_data[6] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID - 1)));
    // globals();
    // adj_vertex_data[1] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID - 1 - mesh_dim - 1)));
    // globals();
    // adj_vertex_data[2] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID - mesh_dim - 1)));
    // globals();
    // adj_vertex_data[3] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID + 1)));
    // globals();
    // adj_vertex_data[4] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID + 1 + mesh_dim + 1)));
    // globals();
    // adj_vertex_data[5] = effects(vec3(texelFetch(tex_ver_data, gl_VertexID + mesh_dim + 1)));
    // globals();
    
    // vec3 smooth_normal = vec3(0.0);
    // for (int i = 0; i < 6; i++)
    // {
    //     smooth_normal += cross(adj_vertex_data[i] - vertex_data, adj_vertex_data[i+1] - vertex_data);
    // }

    vec3 smooth_normal = normal;

    normalize(smooth_normal);

    // vec3 vertex_data = noise(coords);
    vec4 position = vec4(coords, 1.0);

    gl_Position = aspect * proj * view * model * position;

    lighting(position);

    frag_data.vert_normal_view = mat3(transpose(inverse(view * model))) * smooth_normal;
}