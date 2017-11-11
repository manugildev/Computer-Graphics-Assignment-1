#version 330

#define number_of_point_lights 3

struct DirLight {
	vec3 direction;
	vec3 light_color;
};

struct PointLight {
	vec3 position;
	vec3 light_color;
	float constant;
	float linear;
	float quadratic;
};


out vec4 frag_color;

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coords;

uniform vec3 view_pos;
uniform vec3 object_color;
uniform float ambient_strength;
uniform float specular_strength;
uniform int specular_power;

uniform DirLight dir_light;
uniform PointLight point_lights[number_of_point_lights];

uniform sampler2D texture_0;

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main(){
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(view_pos - frag_pos);

	// Directional Lighting
	vec3 dir_lighting = calc_dir_light(dir_light, norm, view_dir);

	// Point Lights
	vec3 point_lighting = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < number_of_point_lights; i++){
		point_lighting += calc_point_light(point_lights[i], norm, frag_pos, view_dir);
	}

	// Calculate Result
	vec3 result = dir_lighting + point_lighting;
	result *= mix(vec3(texture(texture_0, tex_coords)), object_color, 0.9f);

	frag_color = vec4(result, 1.0f);

}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(-light.direction);
	
	// Ambient Lighting
	vec3 ambient = ambient_strength * vec3(texture(texture_0, tex_coords));

	// Diffuse Lighting
	float diff = max(dot(normal, light_dir), 0.0f);	
	vec3 diffuse = diff * light.light_color * vec3(texture(texture_0, tex_coords)); 

	// Specular Lighting
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_power);
	vec3 specular = specular_strength * spec *  light.light_color * vec3(texture(texture_0, tex_coords));

	return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
	vec3 light_dir = normalize(light.position - frag_pos);

	// Attenuation 
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));
			
	// Ambient Lighting
	vec3 ambient = ambient_strength * vec3(texture(texture_0, tex_coords));
	ambient *= attenuation;

	// Diffuse Lighting
	float diff = max(dot(normal, light_dir), 0.0f);	
	vec3 diffuse = diff * light.light_color * vec3(texture(texture_0, tex_coords)); 
	diffuse *= attenuation;

	// Specular Lighting
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_power);
	vec3 specular = specular_strength * spec * light.light_color * vec3(texture(texture_0, tex_coords));
	specular *= attenuation;

	return (ambient + diffuse + specular);
}