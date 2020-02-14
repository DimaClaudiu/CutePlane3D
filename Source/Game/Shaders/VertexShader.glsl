#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform float Time;

// Output value to fragment shader
out vec3 color;

float random( vec2 p )
{
    vec2 K1 = vec2(
        23.14069263277926, // e^pi (Gelfond's constant)
         2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return fract( cos( dot(p,K1) ) * 12345.6789 );
}

void main()
{
	// TODO: compute world space vectors
	//v_position += Time*vec3(1);
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
    vec3 world_normal = normalize( mat3(Model) * normalize(v_normal) );



	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = 0.55;

	// TODO: compute diffuse light component
	//float diffuse_light = 0;
	
	float diffuse_light = material_kd * max (dot(N, L), 0);
	vec3 diffuse_light_vec3 = vec3(diffuse_light / 10, diffuse_light / 10 , diffuse_light + sin(Time) * 10);

	// TODO: compute specular light component
	float specular_light = 0;
	vec3 specular_light_vec3;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess);
		specular_light_vec3 = vec3(specular_light / 10, specular_light / 10, specular_light  + sin(Time) * 10);
	}

	// TODO: compute light
	float light = ambient_light + specular_light + diffuse_light;

	//vec3 light_vec3 = diffuse_light_vec3 + vec3(1) * (ambient_light + specular_light);
	vec3 light_vec3 = diffuse_light_vec3 +  specular_light_vec3 + vec3(1) * (ambient_light);

	// TODO: send color light output to fragment shader
	//color = vec3(1);
	color = object_color * light_vec3;

	vec3 deviation = v_position + vec3(0, sin(Time + random(vec2(v_position.x, v_position.y))) / 1.8f, 0);
	gl_Position = Projection * View * Model * vec4(deviation, 1.0);
}
