#pragma once
#include "..\ShaderProgram.h"

const std::string PARTICLE_VERTEX_FILE = "shaders/particles/particle_vertex_shader.glsl";
const std::string PARTICLE_FRAGMENT_FILE = "shaders/particles/particle_fragment_shader.glsl";

class ParticleShader : public ShaderProgram {
public:

	static ParticleShader* create(std::string vertex_shader = PARTICLE_VERTEX_FILE, std::string fragment_shader = PARTICLE_FRAGMENT_FILE);

	ParticleShader(std::string vertex_shader = PARTICLE_VERTEX_FILE, std::string fragment_shader = PARTICLE_FRAGMENT_FILE);
	~ParticleShader();

	void bind_attributes() override;
	void get_all_uniform_locations() override;

	void set_model_matrix(glm::mat4 matrix);
	void set_view_matrix(glm::mat4 matrix);
	void set_proj_matrix(glm::mat4 matrix);
	void set_number_of_rows(float value);
	void set_object_color(glm::vec3 object_color);

private:
	GLuint location_model_mat, location_view_mat, location_proj_mat;
	GLuint location_number_of_rows;
	GLuint location_object_color;
};

