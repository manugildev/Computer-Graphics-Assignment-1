#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

#include "GameObject.h"
#include "..\res\teapot.h"
#include "..\util\shader.h"
#include "glm\ext.hpp"

GameObject::GameObject() {
	this->model_mat = glm::mat4(1.0f);
	init();
}

void GameObject::init() {

	shader_program_id = compile_shaders();

	/* Generate Object Buffer*/
	GLuint vp_vbo = 0;
	GLuint vbo = 0;
	location_positions = glGetAttribLocation(shader_program_id, "vertex_positions");
	location_normals = glGetAttribLocation(shader_program_id, "vertex_normals");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_vertex_points, GL_STATIC_DRAW);
	GLuint vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_normals, GL_STATIC_DRAW);

	glGenVertexArrays(1, &teapot_vao);
	glBindVertexArray(teapot_vao);

	glEnableVertexAttribArray(location_positions);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(location_positions, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(location_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(location_normals, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}

GameObject::~GameObject() {}

void GameObject::update() {}

void GameObject::render() {
	glUseProgram(shader_program_id);
	glBindVertexArray(teapot_vao);

	int location_model_mat = glGetUniformLocation(shader_program_id, "model_mat");
	int location_view_mat = glGetUniformLocation(shader_program_id, "view_mat");
	int location_proj_mat = glGetUniformLocation(shader_program_id, "proj_mat");

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 perspective_proj = glm::perspective(glm::radians(65.0f), (float) 640 / (float) 490, 0.1f, 100.0f);
	model_mat = glm::rotate(model_mat, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	std::cout << glm::to_string(model_mat) << std::endl;

	glUniformMatrix4fv(location_proj_mat, 1, GL_FALSE, &perspective_proj[0][0]);
	glUniformMatrix4fv(location_view_mat, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(location_model_mat, 1, GL_FALSE, &model_mat[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void GameObject::AddShader(GLuint shader_program, const char * shader_text, GLenum shader_type) {
	GLuint shader_obj = glCreateShader(shader_type);

	if (shader_obj == 0) {
		fprintf_s(stderr, "Error creating shader type %d \n", shader_type);
	}

	std::string out_shader = read_shader_source(shader_text);
	const char * shader_source = out_shader.c_str();
	printf(shader_source);

	glShaderSource(shader_obj, 1, (const char**) &shader_source, NULL);
	glCompileShader(shader_obj);
	GLint success;

	glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar info_log[1024];
		glGetShaderInfoLog(shader_obj, 1024, NULL, info_log);
		fprintf_s(stderr, "Erro compiling shader type %d: '%s'", shader_type, info_log);
	}

	glAttachShader(shader_program, shader_obj);
}

GLuint GameObject::compile_shaders() {
	shader_program_id = glCreateProgram();
	if (shader_program_id == 0) {
		fprintf_s(stderr, "Error creating shader program\n");

	}

	AddShader(shader_program_id, "shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	AddShader(shader_program_id, "shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar error_log[1024] = {0};

	glLinkProgram(shader_program_id);
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shader_program_id, sizeof(error_log), NULL, error_log);
		fprintf_s(stderr, "Error linking shader program: '%s'\n", error_log);
	}

	glValidateProgram(shader_program_id);
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program_id, sizeof(error_log), NULL, error_log);
		fprintf_s(stderr, "Error linking shader program: '%s'\n", error_log);
	}

	glUseProgram(shader_program_id);
	return shader_program_id;
}