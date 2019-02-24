#version 330 core
// #define OPENGL_ES

/*** COMPATIBILITY FUNCTIONS ***/
#ifdef OPENGL_ES
// #extension GL_ARB_texture_buffer_object : enable
precision mediump float;
#else

#endif

float modulo(float x, float y)
{
    return x - y * floor(x/y);
}

/*** INPUTS ***/

float bring_vel = 5.0;

float bring_timecoeff = 1.0; // Mess around! (coefficient for relative velocity between wave and mesh)

int tube_dx = 50; // Size of mesh (dx)
float tube_timeq = 10.0; // Time coefficient for how fast to do transition
float tube_timed = 1.0; // Time delay until start of animation

#define M_PI 3.1415926535897932384626433832795

// Globals
float tube_r;

void globals()
{
    tube_r = tube_dx/(4.0*M_PI); 
}

/*** VARIABLES ***/

layout (location = 0) in vec3 coords;
layout (location = 1) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 aspect;
uniform float time;

struct Vertex_Data
{
    vec3 vert_normal_view;
    vec3 vert_pos_view;
};

out Vertex_Data frag_data;

// Texture data test
// uniform sampler2D tex_ver_data;
uniform samplerBuffer tex_ver_data;

/*** LIGHTING ***/

void lighting(vec4 newcoords)
{
//     frag_data.vert_normal_view = mat3(transpose(inverse(view * model))) * normal;
    // vertex_data.vert_normal_view = normal;
    frag_data.vert_pos_view = vec3(view * model * newcoords);
}

/*** BRING ***/

vec3 bring(vec3 newcoords) 
{
    vec3 newcoords1 = newcoords;
    newcoords1.z = newcoords.z+bring_vel*(modulo(time,1.0)); // bring!

    return newcoords1;
}

/*** TUBE ***/

vec3 tube_transform(vec3 newcoords)
{
    if (time <= tube_timed)
    {
        return newcoords;
    }
    if (time >= tube_timed && time < tube_timed+tube_timeq*0.5)
    {
        tube_r = tube_dx/(4.0*M_PI); 
        tube_r /= mod((time-tube_timed)/tube_timeq,0.5);
        float q = 4.0*M_PI*mod((time-tube_timed)/(tube_timeq),0.5);

        // A' (x',y',z') = ( R*cos(x*(2Pi/L)) , R*sin(x*(2Pi/L)) , z*(H'/H))
        return vec3(tube_r*cos(newcoords.x*(q/tube_dx) - M_PI/2.0), tube_r*sin(newcoords.x*(q/tube_dx) - M_PI/2.0) + tube_r, newcoords.z);
    }   
    if(time >= tube_timed+tube_timeq*0.5)
    {
        tube_r = tube_dx/(4.0*M_PI); 
        tube_r /= 0.5;
        float q = 2.0*M_PI;
        // A' (x',y',z') = ( R*cos(x*(2Pi/L)) , R*sin(x*(2Pi/L)) , z*(H'/H))
        return vec3(tube_r*cos(newcoords.x*(q/tube_dx) - M_PI/2.0), tube_r*sin(newcoords.x*(q/tube_dx) - M_PI/2.0) + tube_r, newcoords.z);
    }
}
vec3 tube(vec3 newcoords) 
{    
    tube_r = tube_dx/(4.0*M_PI) / 0.5;
    float q = 2.0*M_PI;
    // A' (x',y',z') = ( R*cos(x*(2Pi/L)) , R*sin(x*(2Pi/L)) , z*(H'/H))
    return vec3(tube_r*cos(newcoords.x*(q/tube_dx) - M_PI/2.0), tube_r*sin(newcoords.x*(q/tube_dx) - M_PI/2.0) + tube_r, newcoords.z);
}

/*** NOISE ***/

float hash(vec2 p) 
{ 
    return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); 
}

float perlin(vec2 x) 
{
    vec2 i = floor(x);
    vec2 f = fract(x);

	// Four corners in 2D of a tile
	float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

	// Same code, with the clamps in smoothstep and common subexpressions
	// optimized away.
    vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

vec3 noise(vec3 newcoords)
{
    newcoords.y += perlin(vec2(newcoords.x, newcoords.z - bring_vel * bring_timecoeff * time) / 2.0);

    return newcoords;
}

/*** WAVE ***/

vec3 wave(vec3 newcoords)
{
    newcoords.y += 0.2 * sin(time) * sin(newcoords.x + M_PI/2.0) + 0.15 * sin(time) * cos(newcoords.z-bring_vel*bring_timecoeff*time);

    return newcoords;
}
