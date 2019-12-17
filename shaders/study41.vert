/*** MAIN ***/

#include "common.vert"

vec3 effects(vec3 newcoords)
{

    return vec3(coords[0], coords[1]+3.0*snoise(vec4(coords/10.0, time/2.0)),coords[2]);
}

void main() 
{  
    globals();
    vec3 smooth_normal = vec3(0.0);
    vec4 position = vec4(0.0);

    if (true)
    {
        vec3 vertex_data = effects(vec3(texelFetch(tex_ver_data, gl_VertexID)));
        vec3 adj_vertex_data[6];
        
        int adj_vertex_id[6] = int[](adjacents0[0],adjacents0[1],adjacents0[2], adjacents1[0],adjacents1[1],adjacents1[2]);

        adj_vertex_data[0] = effects(vec3(texelFetch(tex_ver_data, adj_vertex_id[0])));
        globals();
        adj_vertex_data[1] = effects(vec3(texelFetch(tex_ver_data, adj_vertex_id[1])));
        globals();
        adj_vertex_data[2] = effects(vec3(texelFetch(tex_ver_data, adj_vertex_id[2])));
        globals();
        adj_vertex_data[3] = effects(vec3(texelFetch(tex_ver_data, adj_vertex_id[3])));
        globals();
        adj_vertex_data[4] = effects(vec3(texelFetch(tex_ver_data, adj_vertex_id[4])));
        globals();
        adj_vertex_data[5] = effects(vec3(texelFetch(tex_ver_data, adj_vertex_id[5])));
        globals();

        int adj_vertex_seq[12] = int[](adjacents_seq0[0],adjacents_seq0[1],adjacents_seq0[2],adjacents_seq0[3],
                                        adjacents_seq1[0],adjacents_seq1[1],adjacents_seq1[2],adjacents_seq1[3],
                                        adjacents_seq2[0],adjacents_seq2[1],adjacents_seq2[2],adjacents_seq2[3]);

        smooth_normal = vec3(0.0,0.0,0.0);
        for (int i = 0; i < 12; i+=2)
        {
            if (adj_vertex_seq[i] < 12 && adj_vertex_seq[i] >= 0)
            {
                smooth_normal += cross(adj_vertex_data[adj_vertex_seq[i]] - vertex_data, adj_vertex_data[adj_vertex_seq[i+1]] - vertex_data);   
            }
        }
        position = vec4(vertex_data, 1.0);
        // position = vec4(adj_vertex_data[5], 1.0);
        // position = vec4(adjacents0, 1.0);
    }
    else
    {
        smooth_normal = normal;

        normalize(smooth_normal);
        position = vec4(effects(coords), 1.0);
    }

    gl_Position = aspect * proj * view * model * position;

    lighting(position);

    frag_data.vert_normal_view = mat3(transpose(inverse(view * model))) * smooth_normal;
}
