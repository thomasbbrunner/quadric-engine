#version 330 core
// #define OPENGL_ES

/*** COMPATIBILITY FUNCTIONS ***/
#ifdef OPENGL_ES
precision mediump float;
#else

#endif

float modulo(float x, float y)
{
    return x - y * floor(x/y);
}

/*** INPUTS ***/

float ambient_strength = 0.5;
float specular_strength = 0.5;
int specular_shininess = 256; // powers of two (2,4,8,16...256)
float attenuation_constant = 1.0;
float attenuation_linear = 0.02;
float attenuation_quadratic = 0.002;

#define M_PI 3.1415926535897932384626433832795

/*** VARIABLES ***/
flat in ivec3 DEBUG0;
flat in int DEBUG1;
out vec4 frag_color;
uniform vec4 color;
uniform float time;

struct Vertex_Data
{
    vec3 vert_normal_view;
    vec3 vert_pos_view;
};

in Vertex_Data frag_data;

/*** RAINBOW ***/

vec4 rainbow(vec4 newcolor)
{
    float color0 = time;
    return vec4(sin(color0 - 2.0) / 2.0 + 0.5, sin(color0) / 2.0 + 0.5, sin(color0 + 2.0) / 2.0 + 0.5, 1.0);
}

/*** LIGHTING ***/

struct Light 
{
    vec4 color;
    vec3 pos;
    int type;
};

uniform Light light[10];
uniform mat4 view;
uniform mat4 model;

vec4 lighting(vec4 newcolor)
{
    // Calculating normal
    // vec3 fdx = vec3(dFdx(frag_data.vert_pos_view.x),dFdx(frag_data.vert_pos_view.y),dFdx(frag_data.vert_pos_view.z));    
    // vec3 fdy = vec3(dFdy(frag_data.vert_pos_view.x),dFdy(frag_data.vert_pos_view.y),dFdy(frag_data.vert_pos_view.z));
    // vec3 N = normalize(cross(fdx,fdy));
    // N = normalize(mat3(transpose(inverse(view * model))) * N);
    vec3 N = frag_data.vert_normal_view;

    // Calculating lighting
    vec3 lightasdf = vec3(0.0,0.0,0.0);

    for (int i = 0; i < 10; i++)
    {
        vec4 light_pos_view = vec4(0.0,0.0,0.0,0.0);

        if (light[i].type == 0) // direction vector -> directional light
        {
            light_pos_view = view * vec4(light[i].pos, 0.0);
        }
        else if(light[i].type == 1) // position vector -> positional light
        {
            light_pos_view = view * vec4(light[i].pos, 1.0);
        }
        else
        {
            return vec4(1.0,0.0,0.0,0.0);
        }

        // Ambient
        float ambient_amount = ambient_strength;

        // Diffuse
        vec3 frag_light_dir = vec3(0.0,0.0,0.0);
        float diffuse_amount = 0.0;
        if (light[i].type == 0) // direction vector -> directional light
        {
            frag_light_dir = normalize(vec3(-light_pos_view));  
            diffuse_amount = max(dot(normalize(N), frag_light_dir), 0.0);
        }
        else if(light[i].type == 1) // position vector -> positional light
        {
            frag_light_dir = normalize(vec3(light_pos_view) - frag_data.vert_pos_view);  
            diffuse_amount = max(dot(normalize(N), frag_light_dir), 0.0);
            // diffuse_amount = abs(dot(normalize(frag_data.vert_normal_view), frag_light_dir)); // for light source inside geometry
        }
        else
        {
            return vec4(1.0,0.0,0.0,0.0);
        }

        // Specular
        vec3 viewDir = normalize(-frag_data.vert_pos_view);
        vec3 reflectDir = reflect(-frag_light_dir, normalize(frag_data.vert_normal_view));

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), float(specular_shininess));
        float specular_amount = specular_strength * spec;

        // Resulting lighting
        if (light[i].type == 0) // direction vector -> directional light
        {
            lightasdf += (ambient_amount + diffuse_amount + specular_amount) * vec3(light[i].color); 
        }
        else if (light[i].type == 1) // position vector -> positional light
        {
            float distance    = length(vec3(light_pos_view) - frag_data.vert_pos_view);
            float attenuation = 1.0 / (attenuation_constant + attenuation_linear * distance + attenuation_quadratic * (distance * distance)); 
            lightasdf += attenuation*(ambient_amount + diffuse_amount + specular_amount) * vec3(light[i].color); 
        }
    }

    return vec4(lightasdf * vec3(newcolor), 1.0);
}
