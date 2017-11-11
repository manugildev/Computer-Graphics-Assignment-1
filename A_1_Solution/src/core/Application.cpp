#include "Application.h"
#include "util\Logger.h"

#include <iostream>
#include "util\texture\Texture.h"


Application::Application(Camera* camera) : camera(camera) {
	init();
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

		delta_time = calculate_delta_time();

		/* Poll for and process events */
		glfwPollEvents();
		do_movement();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.49f, 0.54f, 0.55f, 1.0f);
		
		for (int i = 0; i < game_objects.size(); i++) game_objects[i]->update(delta_time);
		for (int i = 0; i < lights.size(); i++) lights[i]->update(delta_time);
		for (int i = 0; i < game_objects.size(); i++) game_objects[i]->render();
		for (int i = 0; i < lights.size(); i++) lights[i]->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window->window_obj);
	}
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

	if (keys[GLFW_KEY_M]) debug = true;
	else debug = false;
}

void Application::scroll_callback(double x_offset, double y_offset) {
	camera->process_mouse_scroll(y_offset);
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
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) this->camera->process_keyboard(FORWARD, delta_time);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) this->camera->process_keyboard(BACKWARD, delta_time);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) this->camera->process_keyboard(LEFT, delta_time);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) this->camera->process_keyboard(RIGHT, delta_time);
}

void Application::set_game_objects(std::vector<GameObject*> game_objects) {
	this->game_objects = game_objects;
}
void Application::set_lights(std::vector<Light*> lights) {
	this->lights = lights;
	this->update_lights();
}

std::vector<GameObject*> Application::get_game_objects() {
	return this->game_objects;
}

std::vector<Light*> Application::get_lights() {
	return this->lights;
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
	window = NULL;
	glfwTerminate();
}
