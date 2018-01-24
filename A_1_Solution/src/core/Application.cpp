#include "Application.h"
#include "game_objects/plane/Plane.h"
#include "util\Logger.h"

#include <iostream>
#include "util\texture\Texture.h"
#include <glm\glm.hpp>
#include "util\gui\Quad.h"

Application::Application(Camera* camera) : camera(camera) {
	this->init();
}

int Application::init() {

	/* Initialize the library */
	if (!glfwInit()) { LOG_MESSAGE("GLFW not initialized."); return -1; }

	/* Create a windowed mode window and its OpenGL context */
	window = new Window(this, 960, 540, "A_1");

	get_camera()->set_aspect_ratio(window->get_aspect_ratio());

	last_x = window->get_width() / 2.0f;
	last_y = window->get_height() / 2.0f;

	glewExperimental = GL_TRUE;
	glewInit();

	/* Get GL version information */
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	char tmp[150];
	sprintf_s(tmp, "Renderer: %s - OpenGL Version: %s", renderer, version);
	LOG_MESSAGE(tmp);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/* Set viewport to windows size */
	glViewport(0, 0, window->get_width(), window->get_height());
	return 1;
}


void Application::runMainGameLoop() {
	while (!glfwWindowShouldClose(window->window_obj) && glfwGetKey(window->window_obj, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		/* Update */
		this->update();

		/* Render */
		this->frame_buffer->bind();
		glViewport(0, 0, window->get_width(), window->get_height());
		//this->render();
		this->frame_buffer->unbind();

		glViewport(0, 0, window->get_width(), window->get_height());
		this->render();
		this->gui_renderer->render();

		/* Start second viewport */
		//glViewport(0, window->get_height() - window->get_height() / 4, window->get_width() / 4, window->get_height() / 4);

		/* Update Camera for second viewport */
		//this->camera->update_view_matrix_second_viewport(glm::vec3(0.0f, -1.0f, 0.0f));
		//this->camera->set_persp_proj_matrix(glm::ortho(-window->get_width() / 50.f, window->get_width() / 50.0f, -window->get_height() /50.0f, window->get_height() / 50.0f, 0.1f, 10000.f));

		/* Render second viewport */
		//for (int i = 0; i < game_objects.size(); i++) game_objects[i]->render();
		//for (int i = 0; i < point_lights.size(); i++) point_lights[i]->render();
		//this->dir_light->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window->window_obj);
	}
}

void Application::update() {
	this->delta_time = calculate_delta_time();

	/* Poll for and process events */
	glfwPollEvents();
	for (int i = 0; i < game_objects.size(); i++) game_objects[i]->update(delta_time);

	/* Updating the camera aftwerwards because of the FPV*/
	this->do_movement();
	this->camera->update_view_matrix();
	this->camera->set_persp_proj_matrix(glm::perspective(glm::radians(camera->get_field_of_view()), camera->get_aspect_ratio(), 0.1f, 10000.0f));

	for (int i = 0; i < point_lights.size(); i++) point_lights[i]->update(delta_time);
	this->dir_light->update(delta_time);
}

void Application::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	cube_map->render(camera->get_view_matrix(), camera->get_persp_proj_matrix());
	for (int i = 0; i < game_objects.size(); i++) game_objects[i]->render();
	for (int i = 0; i < point_lights.size(); i++) point_lights[i]->render();
	dir_light->render();
}

//TODO: This is probably need to be in the window thing
void Application::bindAsRenderTarget() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, window->get_width(), window->get_height());
}

void Application::update_lights() {
	for (int i = 0; i < game_objects.size(); i++) game_objects[i]->update_lights();
}

float Application::calculate_delta_time() {
	float currentFrame = glfwGetTime();
	delta_time = currentFrame - lastFrame;
	lastFrame = currentFrame;
	return delta_time;
}


#pragma region INPUT_FUNCTIONS
void Application::key_callback(int key, int scancode, int action, int mode) {
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}

	/* City Rotation */
	if (keys[GLFW_KEY_J]) game_objects[0]->set_rotation_speed(glm::vec3(0.0f, -10.0f, 0.0f));
	if (keys[GLFW_KEY_L]) game_objects[0]->set_rotation_speed(glm::vec3(0.0f, 10.0f, 0.0f));
	if (!keys[GLFW_KEY_L] && !keys[GLFW_KEY_J]) {
		game_objects[0]->set_rotation_acceleration(glm::vec3(0.0f));
		game_objects[0]->set_rotation_speed(glm::vec3(0.0f));
	}

	/* Plane Rotation */ //Todo: Get direction instead of pointer
	Plane* plane = dynamic_cast <Plane*>(game_objects[game_objects.size() - 1]);
	if (keys[GLFW_KEY_Z]) {
		plane->set_speed_y(-60.0f);

		if (!plane->with_quaternions) plane->get_green_arrow()->set_rotation_speed(glm::vec3(0, -60.0f, 0.0f));
	}
	if (keys[GLFW_KEY_X]) {
		plane->set_speed_y(60.0f);
		if (!plane->with_quaternions) plane->get_green_arrow()->set_rotation_speed(glm::vec3(0, 60.0f, 0.0f));
	}
	if (!keys[GLFW_KEY_Z] && !keys[GLFW_KEY_X]) {
		plane->set_speed_y(0);
		if (!plane->with_quaternions) plane->get_green_arrow()->set_rotation_speed(glm::vec3(0, 0.0f, 0.0f));
	}

	if (keys[GLFW_KEY_C]) {
		plane->set_speed_z(60.0f);		
		if (!plane->with_quaternions) plane->get_blue_arrow()->set_rotation_speed(glm::vec3(0, 0.0f, -60.0f));
	}
	if (keys[GLFW_KEY_V]) {
		plane->set_speed_z(-60.0f); 
		if (!plane->with_quaternions) plane->get_blue_arrow()->set_rotation_speed(glm::vec3(0, 0.0f, 60.0f));
	}
	if (!keys[GLFW_KEY_C] && !keys[GLFW_KEY_V]) {
		plane->set_speed_z(0);
		if (!plane->with_quaternions) plane->get_blue_arrow()->set_rotation_speed(glm::vec3(0, 0.0f, 0.0f));
	}

	if (keys[GLFW_KEY_B]) {
		plane->set_speed_x(-60.0f);
		if (!plane->with_quaternions) plane->get_red_arrow()->set_rotation_speed(glm::vec3(0, 0.0f, -60.0f));
	}
	if (keys[GLFW_KEY_N]) {
		plane->set_speed_x(60.0f);
		if (!plane->with_quaternions) plane->get_red_arrow()->set_rotation_speed(glm::vec3(0.0f, 0.0f, 60.0f));
	}
	if (!keys[GLFW_KEY_B] && !keys[GLFW_KEY_N]) {
		plane->set_speed_x(0);
		if (!plane->with_quaternions) plane->get_red_arrow()->set_rotation_speed(glm::vec3(0, 0.0f, 0.0f));
	}

	if (keys[GLFW_KEY_R]) {
		camera->first_person = !camera->first_person;
	}

	if (keys[GLFW_KEY_M]) {
		plane->with_quaternions = !plane->with_quaternions;
		plane->show_debug = !plane->show_debug;
		plane->transform = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0,0,1)); // We init the transform quaternion to 0.0f in the forward vector
		plane->transform = plane->transform * glm::angleAxis(glm::radians(90.0f), glm::vec3(-1, 0, 0));
	}

	if (keys[GLFW_KEY_RIGHT]) {
		plane->start_roll();
	}
	if (keys[GLFW_KEY_UP]) {
		plane->start_yaw();
	}
}

void Application::resize_callback(int width, int height) { if (frame_buffer) frame_buffer->resize(); }

void Application::scroll_callback(double x_offset, double y_offset) {
	camera->process_mouse_scroll(y_offset);
	if (!keys[GLFW_KEY_SPACE]) {
		float specular_power = game_objects[game_objects.size() - 2]->get_specular_power() + y_offset;
		game_objects[game_objects.size() - 2]->set_specular_power(specular_power);
		game_objects[game_objects.size() - 3]->set_specular_power(specular_power);
		game_objects[game_objects.size() - 4]->set_specular_power(specular_power);
	}	else {
		float specular_strength = game_objects[game_objects.size() - 2]->get_specular_strength() + (y_offset/10);
		game_objects[game_objects.size() - 2]->set_specular_strength(specular_strength);
		game_objects[game_objects.size() - 3]->set_specular_strength(specular_strength);
		game_objects[game_objects.size() - 4]->set_specular_strength(specular_strength);
	}
	
}

void Application::mouse_callback(double x_pos, double y_pos) {
	if (first_mouse) {
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	x_offset = x_pos - last_x;
	y_offset = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	camera->process_mouse(x_offset, y_offset);
}
#pragma endregion

void Application::do_movement() {
	if (keys[GLFW_KEY_W]) this->camera->process_keyboard(FORWARD, delta_time);
	if (keys[GLFW_KEY_S] ) this->camera->process_keyboard(BACKWARD, delta_time);
	if (keys[GLFW_KEY_A]) this->camera->process_keyboard(LEFT, delta_time);
	if (keys[GLFW_KEY_D] ) this->camera->process_keyboard(RIGHT, delta_time);


	if (keys[GLFW_KEY_E]) this->camera->process_keyboard(ROLL_RIGHT, delta_time);
	if (keys[GLFW_KEY_Q]) this->camera->process_keyboard(ROLL_LEFT, delta_time);
	
	if (!(keys[GLFW_KEY_W]) &&
		!(keys[GLFW_KEY_S]) &&
		!(keys[GLFW_KEY_A]) &&
		!(keys[GLFW_KEY_D])) this->camera->process_keyboard(STOP, delta_time);
}

void Application::set_game_objects(std::vector<GameObject*> game_objects) {
	this->game_objects = game_objects;
}

void Application::set_gui_renderer(GuiRenderer* gui_renderer) {
	this->gui_renderer = gui_renderer;
}

void Application::set_directional_light(DirLight* dir_light) {
	this->dir_light = dir_light;
}
void Application::set_point_lights(std::vector<PointLight*> point_lights) {
	this->point_lights = point_lights;
	this->update_lights();
}

std::vector<GameObject*> Application::get_game_objects() {
	return this->game_objects;
}

DirLight* Application::get_dir_light() {
	return this->dir_light;
}

std::vector<PointLight*> Application::get_point_lights() {
	return this->point_lights;
}

void Application::set_cube_map(CubeMap * cube_map) {
	this->cube_map = cube_map;
}

void Application::set_frame_buffer(FrameBuffer* frame_buffer) {
	this->frame_buffer = frame_buffer;
}

Camera* Application::get_camera() {
	return this->camera;
}

Window* Application::get_window() {
	return this->window;
}
bool Application::is_debug() {
	return this->debug;
}

Application::~Application() {
	delete window;
	for (int i = 0; i < game_objects.size(); i++) { delete game_objects[i]; game_objects[i] = NULL; }
	for (int i = 0; i < point_lights.size(); i++) { delete point_lights[i]; point_lights[i] = NULL; }
	window = NULL;
	glfwTerminate();
}
