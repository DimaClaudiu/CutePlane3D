#version 330

// get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform int lighting_type;
uniform float cut_off; // cut_off position

layout(location = 0) out vec4 out_color;

void main()
{	
	if(lighting_type == 0) // Normal
	{
		//float ambient_light = 0.146534785687;
		float ambient_light = 0.45;
	
		vec3 L = normalize(light_position - world_position);
		float diffuse_light = max(dot(normalize(world_normal), normalize(L)), 0);

		vec3 R = reflect(normalize(-L), normalize(world_normal));
		vec3 H = normalize(normalize(world_normal) + normalize(R) / 2);

		float specular_light = pow(max(dot(normalize(world_normal), H), 0), material_shininess);

		float dist = distance(light_position, world_position);
		float attenuation = 1 / (1.1 * dist * dist + 0.002 * dist + 1);
		vec3 light = (ambient_light + material_kd * diffuse_light + material_ks * specular_light) * object_color * attenuation;

		out_color = vec4(light, 1);
	}
	else //Spot light
	{
		vec3 L = normalize(light_position - world_position);
		float spot_light = dot(normalize(-L), normalize(light_direction));
		float spot_light_limit = cos(cut_off);
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);
		float ambient_light = 0.08;

		if(spot_light > cos(cut_off))
		{
			// phong
			float diffuse_light = max(dot(normalize(world_normal), normalize(L)), 0);

			vec3 R = reflect(normalize(-L), normalize(world_normal));
			vec3 H = normalize(normalize(world_normal) + normalize(R) / 2);

			float specular_light = pow(max(dot(normalize(world_normal), H), 0), material_shininess);

			float dist = distance(light_position, world_position);
			float attenuation = 1 / (0.001 * dist * dist + 0.02 * dist + 1);
			vec3 light = (ambient_light + (material_kd * diffuse_light + material_ks * specular_light) * attenuation * light_att_factor) * object_color;

			out_color = vec4( light, 1);
		}
		else
		{	
			out_color = vec4(ambient_light * object_color, 1);
		}
		
	}
}