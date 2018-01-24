#include <iostream>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\detail\func_vector_relational.hpp>
#include <glm\ext.hpp>

#include "GameObject.h"
#include "..\Application.h"
#include "..\src\core\util\shaders\lighting\LightingShader.h"

GameObject::GameObject(Application *app, Model* model, glm::vec3 object_color) : app(app), camera(app->get_camera()), model(model), object_color(object_color) {
	this->model_mat = glm::mat4(1.0f);
}

void GameObject::set_shader_program(LightingShader* shader_program) {
	this->shader_program = shader_program;
	this->set_initial_shader_values();
}

void GameObject::set_initial_shader_values() {
	shader_program->start();
	shader_program->set_ambient_strength(.02f);
	shader_program->set_specular_strength(0.1f);
	shader_program->set_mix_power(0.0f);
	shader_program->set_specular_power(4);
	shader_program->stop();
}

void GameObject::update_lights() {
	// TODO: Only working for one light right now, implement shaders for multi lighting
	shader_program->start();
	shader_program->set_directional_light(app->get_dir_light());
	shader_program->set_point_lights(app->get_point_lights());
	shader_program->stop();
}

void GameObject::update(float delta_time) {
	//model_mat = glm::rotate(model_mat, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 new_speed = this->speed + (this->acceleration * delta_time);
	if (glm::all(glm::lessThan(new_speed, this->max_speed))) this->speed = new_speed;
	this->position += this->speed * delta_time;
	this->set_pos(this->position);

	glm::vec3 new_rotation_speed = this->rotation_speed + (this->rotation_acceleration * delta_time);
	if (glm::all(glm::lessThan(new_rotation_speed, this->max_rotation_speed))) this->rotation_speed = new_rotation_speed;
	this->rotation += this->rotation_speed * delta_time;

	glm::quat rotX = glm::angleAxis(glm::radians(rotation[0]), glm::vec3(1.f, 0.f, 0.f));
	glm::quat rotY = glm::angleAxis(glm::radians(rotation[1]), glm::vec3(0.f, 1.f, 0.f));
	glm::quat rotZ = glm::angleAxis(glm::radians(rotation[2]), glm::vec3(0.f, 0.f, 1.f));

	quaternion = rotZ * rotY * rotX;

	this->update_model_mat();
}

void GameObject::update_model_mat() {
	this->model_mat = glm::scale(glm::mat4(1.0f), this->scale);

	glm::mat4 rotation_mat = glm::toMat4(quaternion);
	this->model_mat = rotation_mat * model_mat;
		
	this->model_mat[3][0] = this->position[0];
	this->model_mat[3][1] = this->position[1];
	this->model_mat[3][2] = this->position[2];
	//this->model_mat = glm::translate(this->model_mat, this->position);
	if (parent != nullptr) this->model_mat = parent->model_mat * this->model_mat;
}

void GameObject::render() {

	glm::mat4 view = this->camera->get_view_matrix();
	glm::mat4 perspective_proj = this->camera->get_persp_proj_matrix();

	this->shader_program->start();	
	this->shader_program->set_view_matrix(view);
	this->shader_program->set_proj_matrix(perspective_proj);
	this->shader_program->set_model_matrix(model_mat);

	this->shader_program->set_toon_shading(toon_shading);
	this->shader_program->set_object_color(object_color);
	this->shader_program->set_specular_strength(specular_strength);
	this->shader_program->set_ambient_strength(ambient_strength);
	this->shader_program->set_specular_power(specular_power);
	this->shader_program->set_mix_power(mix_power);

	if (this->app->is_debug()) { // TODO: Make this work again
		this->model->draw(nullptr, GL_LINES);
	}
	else this->model->draw(this->shader_program);
	
	this->shader_program->set_toon_shading(false);	
	this->shader_program->stop();
}

#pragma region PROPERTIES_SETTERS
void GameObject::set_pos(glm::vec3 pos) {
	this->position = pos;
}

void GameObject::set_acceleration(glm::vec3 acc) {
	this->acceleration = acc;
}

void GameObject::set_speed(glm::vec3 speed) {
	this->speed = speed;
}

void GameObject::set_max_speed(glm::vec3 max_speed) {
	this->max_speed = max_speed;
}

void GameObject::set_rotation(glm::vec3 rotation) {
	glm::quat qPitch = glm::angleAxis(glm::radians(rotation[0]), glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(glm::radians(rotation[1]), glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(glm::radians(rotation[2]), glm::vec3(0, 0, 1));

	quaternion = qYaw * qPitch * qRoll;

	this->rotation = rotation;
}

void GameObject::set_rotation_acceleration(glm::vec3 acc) {
	this->rotation_acceleration = acc;
}

void GameObject::set_rotation_speed(glm::vec3 speed) {
	this->rotation_speed = speed;
}

void GameObject::set_max_rotation_speed(glm::vec3 max_speed) {
	this->max_rotation_speed = max_speed;
}

glm::vec3 GameObject::get_pos() {
	return position;
}

glm::vec3 GameObject::get_rotation() {
	return this->rotation;
}

glm::vec3 GameObject::get_speed() {
	return this->speed;
}

glm::vec3 GameObject::get_rotation_speed() {
	return this->rotation_speed;
}

glm::mat4 GameObject::get_model_mat()
{
	return this->model_mat;
}

glm::quat GameObject::get_quaternion() { return this->quaternion; }

float GameObject::get_specular_power() {
	return this->specular_power;
}

float GameObject::get_specular_strength(){
	return this->specular_strength;
}

void GameObject::set_scale(glm::vec3 scale) {
	this->scale = scale;
}

void GameObject::set_ambient_strength(float ambient_strength) {
	this->ambient_strength = ambient_strength;
}

void GameObject::set_specular_strength(float specular_strength) {
	if (specular_strength > 1 || specular_strength < 0) return;
	this->specular_strength = specular_strength;
}

void GameObject::set_specular_power(int specular_power) {
	if (specular_power <= 1) return; // Limit it
	this->specular_power = specular_power;
}

void GameObject::set_mix_power(float mix_power) {
	this->mix_power = mix_power;
}

void GameObject::set_toon_shading(bool toon_shading) {
	this->toon_shading = toon_shading;
}

void GameObject::set_parent(GameObject* parent) {
	this->parent = parent;
}

void GameObject::set_model_mat(glm::mat4 model_mat) {
	this->model_mat = model_mat;
}

void GameObject::set_quaternion(glm::quat quaternion) {
	this->quaternion = quaternion; 
}



#pragma endregion

GameObject::~GameObject() {
	delete camera;
	delete shader_program;
	delete model;
	camera = NULL;
	shader_program = NULL;
	model = NULL;

}
