#include "LightingShader.h"
#include <iostream>

LightingShader * LightingShader::create() {
	LightingShader * p = new LightingShader();
	p->init();
	return p;
}

LightingShader::LightingShader() : ShaderProgram(LAMP_VERTEX_FILE, LAMP_FRAGMENT_FILE) {}

LightingShader::~LightingShader() {}

void LightingShader::bind_attributes() {
	ShaderProgram::bind_attribute(0, "vertex_positions");
	ShaderProgram::bind_attribute(1, "vertex_normals");
}

void LightingShader::get_all_uniform_locations() {
	/* Locations for Object Pos Properties */
	location_model_mat = get_uniform_location("model_mat");
	location_view_mat = get_uniform_location("view_mat");
	location_proj_mat = get_uniform_location("proj_mat");

	/* Locations for Object Material Properties */
	location_object_color = get_uniform_location("object_color");
	location_ambient_strength = get_uniform_location("ambient_strength");
	location_specular_strength = get_uniform_location("specular_strength");
	location_specular_power = get_uniform_location("specular_power");
	location_texture_0 = get_uniform_location("texture_0");
	location_mix_power = get_uniform_location("mix_power");
	location_toon_shading = get_uniform_location("toon");
	location_cook_shading = get_uniform_location("cook");
	location_cook_r = get_uniform_location("cook_r");
	location_cook_k = get_uniform_location("cook_k");
	location_cook_f = get_uniform_location("cook_f");


	/* Locations for Lights */
	// Directional Lights
	location_dir_light.direction = get_uniform_location("dir_light.direction");
	location_dir_light.light_color = get_uniform_location("dir_light.light_color");

	// Point Lights
	for (int i = 0; i < MAX_LIGHTS; i++) {
		std::string index = std::to_string(i);
		location_point_light[i].position = get_uniform_location("point_lights[" + index + "].position");
		location_point_light[i].light_color = get_uniform_location("point_lights[" + index + "].light_color");
		location_point_light[i].constant = get_uniform_location("point_lights[" + index + "].constant");
		location_point_light[i].linear = get_uniform_location("point_lights[" + index + "].linear");
		location_point_light[i].quadratic = get_uniform_location("point_lights[" + index + "].quadratic");
	}

	/* Locations for Material */
	location_material.ambient_color = get_uniform_location("material.ambient_color");
	location_material.diffuse_color = get_uniform_location("material.diffuse_color");
	location_material.specular_color = get_uniform_location("material.specular_color");
	location_material.shininess = get_uniform_location("material.shininess");
	location_material.shininess_strength = get_uniform_location("material.shininess_strength");
}

void LightingShader::set_object_color(glm::vec3 object_color) { modify_vec3(location_object_color, object_color); }

void LightingShader::set_light_pos(glm::vec3 light_pos) { modify_vec3(location_light_pos, light_pos); }

void LightingShader::set_light_color(glm::vec3 light_color) { modify_vec3(location_light_color, light_color); }

void LightingShader::set_model_matrix(glm::mat4 matrix) { modify_mat4(location_model_mat, matrix); }

void LightingShader::set_view_matrix(glm::mat4 matrix) { modify_mat4(location_view_mat, matrix); }

void LightingShader::set_proj_matrix(glm::mat4 matrix) { modify_mat4(location_proj_mat, matrix); }

void LightingShader::set_ambient_strength(float value) { modify_float(location_ambient_strength, value); }

void LightingShader::set_specular_strength(float value) { modify_float(location_specular_strength, value); }

void LightingShader::set_specular_power(int value) { modify_int(location_specular_power, value); }

void LightingShader::set_toon_shading(bool value) { modify_boolean(location_toon_shading, value); }

void LightingShader::set_cook_shading(bool value) { modify_boolean(location_cook_shading, value); }

void LightingShader::set_texture(int number) { modify_texture(location_texture_0, number); }

void LightingShader::set_mix_power(float value) {
	modify_float(location_mix_power, value);
}

void LightingShader::set_cook_r(float value){
	modify_float(location_cook_r, value);
}

void LightingShader::set_cook_k(float value){
	modify_float(location_cook_k, value);
}
void LightingShader::set_cook_f(float value){
	modify_float(location_cook_f, value);
}

void LightingShader::set_directional_light(DirLight* dir_light) {
	modify_vec3(location_dir_light.direction, dir_light->get_direction());
	modify_vec3(location_dir_light.light_color, dir_light->get_light_color());
}

void LightingShader::set_point_lights(std::vector<PointLight*> point_lights) {
	for (int i = 0; i < point_lights.size(); i++) {
		this->set_point_light(point_lights[i], i);
	}
}

void LightingShader::set_point_light(PointLight* point_light, int index) {
	modify_vec3(location_point_light[index].position, point_light->get_light_position());
	modify_vec3(location_point_light[index].light_color, point_light->get_light_color());
	modify_float(location_point_light[index].constant, point_light->get_constant());
	modify_float(location_point_light[index].linear, point_light->get_linear());
	modify_float(location_point_light[index].quadratic, point_light->get_quadratic());
}

void LightingShader::set_material(Material material) {
	this->modify_vec3(location_material.ambient_color, material.ambient_color);
	this->modify_vec3(location_material.diffuse_color, material.diffuse_color);
	this->modify_vec3(location_material.specular_color, material.specular_color);
	this->modify_float(location_material.shininess, material.shininess);
}