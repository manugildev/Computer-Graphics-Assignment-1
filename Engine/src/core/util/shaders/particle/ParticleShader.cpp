#include "ParticleShader.h"

ParticleShader * ParticleShader::create(std::string vertex_shader, std::string fragment_shader) {
	ParticleShader * p = new ParticleShader(vertex_shader, fragment_shader);
	p->init();
	return p;
}

ParticleShader::ParticleShader(std::string vertex_shader, std::string fragment_shader) : ShaderProgram(vertex_shader, fragment_shader) {}

ParticleShader::~ParticleShader() {}

void ParticleShader::bind_attributes() {
	ShaderProgram::bind_attribute(0, "vertex_positions");
	ShaderProgram::bind_attribute(1, "model_mat1");
}

void ParticleShader::get_all_uniform_locations() {
	location_model_mat = get_uniform_location("model_mat");
	location_view_mat = get_uniform_location("view_mat");
	location_proj_mat = get_uniform_location("proj_mat");
	location_number_of_rows = get_uniform_location("number_of_rows");
	location_object_color = get_uniform_location("object_color");
}

/*void ParticleShader::set_model_matrix(glm::mat4 matrix) {
	//modify_mat4(location_model_mat, matrix);
}*/

void ParticleShader::set_number_of_rows(float value) {
	modify_float(location_number_of_rows, value);
}

void ParticleShader::set_object_color(glm::vec3 object_color){
	modify_vec3(location_object_color, object_color);
}

void ParticleShader::set_model_matrix(glm::mat4 value) {
	modify_mat4(location_model_mat, value);
}

void ParticleShader::set_view_matrix(glm::mat4 matrix) { modify_mat4(location_view_mat, matrix); }

void ParticleShader::set_proj_matrix(glm::mat4 matrix) { modify_mat4(location_proj_mat, matrix); }

