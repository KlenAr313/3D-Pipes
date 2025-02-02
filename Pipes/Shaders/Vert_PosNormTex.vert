#version 430

// VBO-ból érkező változók
layout( location = 0 ) in vec3 vs_in_pos;
//layout( location = 1 ) in vec3 vs_in_norm;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 viewProj;

float r = 0.5;

//SPHERE
void main()
{
	float u = vs_in_pos.x * 3.1415 * 2;
	float v = vs_in_pos.y * 3.1415;

	vs_out_pos = vec3( 
			r * sin(v) * cos(u),
			r * cos(v),
			r * sin(v) * sin(u));

	vs_out_norm = vec3(
				sin(v) * cos(u),
				cos(v),
				sin(v) * sin(u));

	gl_Position = viewProj * world * vec4( vs_out_pos, 1 );
	vs_out_pos  = (world   * vec4(vs_out_pos,  1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_out_norm, 0)).xyz;

}