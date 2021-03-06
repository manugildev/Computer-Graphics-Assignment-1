#pragma once
#include "En.h"

#include "..\camera\Camera.h"
#include "..\util\texture\Texture.h"
#include "..\util\mesh\Mesh.h"
#include "..\util\mesh\Model.h"
#include "..\util\mesh\CubeModel.h"

class Application;
class LightingShader;

const glm::vec3 DEFAULT_OBJ_COLOR(1.0f);

class GameObject {
public:
	GameObject(std::string name, Application *app, Model* model, glm::vec3 object_color = DEFAULT_OBJ_COLOR);
	~GameObject();

	virtual void update(double delta_time);
	virtual void render();
	virtual void render1(ShaderProgram* shader_program);
	virtual void set_initial_shader_values();
	virtual void set_pos(glm::vec3 pos);
	virtual void set_acceleration(glm::vec3 acc);
	virtual void set_speed(glm::vec3 speed);
	virtual void set_max_speed(glm::vec3 max_speed);
	virtual void set_rotation(glm::vec3 pos);
	virtual void set_rotation_acceleration(glm::vec3 acc);
	virtual void set_rotation_speed(glm::vec3 speed);
	virtual void set_max_rotation_speed(glm::vec3 max_speed);
	virtual void set_parent(GameObject* parent);
	virtual void set_quaternion(glm::quat quaternion);
	virtual void set_model_mat(glm::mat4 mat);
	virtual void set_global_model_mat(glm::mat4 mat);
	virtual void set_name(std::string name);
	virtual void set_model(Model* model);
	virtual void set_scale(glm::vec3 scale);
	virtual void set_ambient_strength(float ambient_strength);
	virtual void set_specular_strength(float specular_strength);
	virtual void set_specular_power(int specular_power);
	virtual void set_mix_power(float mix_power);
	virtual void set_cook_f0(float value);
	virtual void set_cook_r(float value);
	virtual void set_cook_k(float value);
	virtual void set_circular_speed(glm::vec2 value);
	virtual void set_distance_from_center(float value);
	virtual void set_circular_angle(glm::vec2 value);
	virtual void set_toon_shading(bool toon_shading);
	virtual void set_cook_shading(bool cook_shading);
	virtual void set_show_normal_texture(bool value);
	virtual void set_apply_normal_map(bool value);
	virtual void set_shader_program(ShaderProgram* shader_program);

	std::string get_name();
	glm::vec3 get_pos();
	glm::vec3 get_rotation();
	glm::vec3 get_speed();
	glm::vec3 get_rotation_speed();
	glm::vec3 get_color();
	glm::mat4 get_model_mat();
	glm::mat4 get_global_model_mat();
	glm::quat get_quaternion();
	int get_specular_power();
	float get_specular_strength();
	float get_cook_f0();
	float get_cook_r();
	float get_cook_k();
	float get_mix_power(); 
	bool get_apply_normal_map();
	bool get_show_normal_texture();
	GameObject* get_parent();
	ShaderProgram* get_shader_program();
	Model* get_model();

	glm::vec3 calculate_rotation_position();

	virtual void update_lights();
	void update_model_mat();
	glm::vec3 rotation = glm::vec3(0.0f); //TODO: Change this

protected:
	Camera * camera;
	Model* model;
	Application* app;
	GameObject* parent;
	glm::mat4 model_mat;
	glm::mat4 global_model_mat;
	glm::vec3 object_color;
	ShaderProgram* shader_program;
	std::string name;

	CubeModel cube_mesh;
	glm::vec3 scale = glm::vec3(1.0f);

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 speed = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);
	glm::quat quaternion;
	glm::vec3 max_speed = glm::vec3(std::numeric_limits<float>::max());

	glm::vec3 rotation_speed = glm::vec3(0.0f);
	glm::vec3 rotation_acceleration = glm::vec3(0.0f);
	glm::vec3 max_rotation_speed = glm::vec3(std::numeric_limits<float>::max());

	bool toon_shading = false;
	bool cook_shading = false;
	float specular_strength = 1;
	float ambient_strength = 0.9f;
	int specular_power = 0;
	float cook_k = 0;
	float cook_f0 = 0;
	float mix_power;
	float cook_r = 0;
	bool apply_normal_map = true;
	bool show_normal_texture = false;

	float distance_from_center = 0;
	glm::vec2 circular_angle = glm::vec2(0.0f);
	glm::vec2 circular_speed = glm::vec2(0.0f);
	unsigned int fixed_axis = 0; // Can be 0,1,2 (x,y,z) //TODO: probably its better to do an implementation where you set the velocity of the different axis
	glm::vec3 rotation_factor;

};
