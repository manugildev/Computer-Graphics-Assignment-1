#include "Engine.h"
#include "input\Input.h"
#include "game_objects\car\Car.h"
#include "game_objects\plane\Plane.h"

int main(void) {
	/* Camera */
	Camera camera(glm::vec3(0.0f, 7.0f, 10.0f));
	
	/* Application */
	Application *app = new Application(&camera);

	/* Input Manager */
	InputManager* input_manager = new Input(app);	
	
	/* Models */
	Model* city_model = new Model("models/street/street.obj");
	Model* car_model = new Model("models/volks.obj");
	Model* plane_model = new Model("models/plane/piper_pa18_1.obj");

	/* GameObjects */
	LightingShader* shader_program = LightingShader::create();
	GameObject *city = new GameObject("city", app, city_model, glm::vec3(1.0f));
	city->set_scale(glm::vec3(1.0f));
	city->set_rotation_speed(glm::vec3(0.0f, 0.0f, 0.0f));
	city->set_pos(glm::vec3(0.0f, 0.0f, 0.0f));
	city->set_shader_program(shader_program);

	Car *car1 = new Car("car1", app, car_model, glm::vec3(0.18f, 0.80f, 0.44f));
	car1->set_parent(city);
	car1->set_initial_pos(glm::vec3(0.0f, 1.0f, -10.0f));
	car1->set_rotation(glm::vec3(0.0f, 270.0f, 0.0f));
	car1->set_shader_program(shader_program);
	car1->set_acceleration(glm::vec3(Util::random_range(0.5f, 2.0f), 0.0f, 0.0f));

	Car *car5 = new Car("car5", app, car_model, glm::vec3(0.20f, 0.59f, 0.84f));
	car5->set_parent(city);
	car5->set_initial_pos(glm::vec3(0.0f, 1.0f, -5.0f));
	car5->set_rotation(glm::vec3(0.0f, 270.0f, 0.0f));
	car5->set_shader_program(shader_program);
	car5->set_acceleration(glm::vec3(Util::random_range(0.5f, 2.0f), 0.0f, 0.0f));

	Car *car2 = new Car("car2", app, car_model, glm::vec3(0.90f, 0.29f, 0.23f));
	car2->set_parent(city);
	car2->set_initial_pos(glm::vec3(0.0f, 1.0f, 0.0f));
	car2->set_rotation(glm::vec3(0.0f, -270.0f, 0.0f));
	car2->set_shader_program(shader_program);
	car2->set_acceleration(glm::vec3(-Util::random_range(0.5f, 2.0f), 0.0f, 0.0f));

	Car *car6 = new Car("car6", app, car_model, glm::vec3(0.94f, 0.76f, 0.05f));
	car6->set_parent(city);
	car6->set_initial_pos(glm::vec3(0.0f, 1.0f, 5.0f));
	car6->set_rotation(glm::vec3(0.0f, -270.0f, 0.0f));
	car6->set_shader_program(shader_program);
	car6->set_acceleration(glm::vec3(-Util::random_range(0.5f, 2.0f), 0.0f, 0.0f));

	Car *car3 = new Car("car3", app, car_model, glm::vec3(0.10f, 0.73f, 0.61f));
	car3->set_parent(city);
	car3->set_initial_pos(glm::vec3(0.0f, 1.0f, 0.0f));
	car3->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	car3->set_shader_program(shader_program);
	car3->set_acceleration(glm::vec3(0.0f, 0.0f, -Util::random_range(0.5f, 2.0f)));

	Car *car7 = new Car("car7", app, car_model, glm::vec3(0.6f, 0.34f, 0.71f));
	car7->set_parent(city);
	car7->set_initial_pos(glm::vec3(5.0f, 1.0f, 0.0f));
	car7->set_rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	car7->set_shader_program(shader_program);
	car7->set_acceleration(glm::vec3(0.0f, 0.0f, -Util::random_range(0.5f, 2.0f)));

	Car *car4 = new Car("car4", app, car_model, glm::vec3(0.9f, 0.49f, 0.13f));
	car4->set_parent(city);
	car4->set_initial_pos(glm::vec3(-5.0f, 1.0f, 0.0f));
	car4->set_rotation(glm::vec3(0.0f, -180.0f, 0.0f));
	car4->set_shader_program(shader_program);
	car4->set_acceleration(glm::vec3(0.0f, 0.0f, Util::random_range(0.5f, 2.0f)));

	Car *car8 = new Car("car8", app, car_model, glm::vec3(0.97f, 0.42f, 0.56f));
	car8->set_parent(city);
	car8->set_initial_pos(glm::vec3(-10.0f, 1.0f, 0.0f));
	car8->set_rotation(glm::vec3(0.0f, -180.0f, 0.0f));
	car8->set_shader_program(shader_program);
	car8->set_acceleration(glm::vec3(0.0f, 0.0f, Util::random_range(0.5f, 2.0f)));

	Plane* main_plane = new Plane("plane", app, plane_model, app->get_camera());
	main_plane->set_parent(city);
	main_plane->set_shader_program(shader_program);
	main_plane->set_scale(glm::vec3(0.3f));
	//plane->set_pos(glm::vec3(0.0f, 0.0f, 0.0f));
	main_plane->set_rotation(glm::vec3(90, 180, 90));

	GameObject * cube = new GameObject("cube", app, new Model(), glm::vec3(0.16f, 0.50f, 0.72f));
	cube->set_parent(city);
	cube->set_shader_program(shader_program);
	cube->set_scale(glm::vec3(5, 0.2f, 30));
	cube->set_pos(glm::vec3(60, 10, 0));
	cube->set_mix_power(.9f);
	cube->set_specular_strength(0.0f);

	Model* teapot_model = new Model("models/pumba/pumba.obj");
	GameObject * teapot1 = new GameObject("teapot1", app, teapot_model , glm::vec3(1));
	teapot1->set_parent(city);
	teapot1->set_shader_program(shader_program);
	teapot1->set_scale(glm::vec3(1.0f));
	teapot1->set_pos(glm::vec3(60, 10, -8));
	teapot1->set_rotation_speed(glm::vec3(0, 20, 0));
	teapot1->set_mix_power(0.1f);

	GameObject * teapot2 = new GameObject("teapot2", app,teapot_model, glm::vec3(1));
	teapot2->set_parent(city);
	teapot2->set_shader_program(shader_program);
	teapot2->set_scale(glm::vec3(1.0f));
	teapot2->set_pos(glm::vec3(60, 10, 0));
	teapot2->set_rotation_speed(glm::vec3(0, 20, 0));
	teapot2->set_mix_power(1);
	teapot2->set_toon_shading(true);

	GameObject * teapot3 = new GameObject("teapot3", app, teapot_model, glm::vec3(1));
	teapot3->set_parent(city);
	teapot3->set_shader_program(shader_program);
	teapot3->set_scale(glm::vec3(1.0f));
	teapot3->set_mix_power(0.9999f);
	teapot3->set_scale(glm::vec3(1.0f));
	teapot3->set_pos(glm::vec3(60, 10, 8));
	teapot3->set_rotation_speed(glm::vec3(0, 20, 0));
	teapot3->set_cook_shading(true);

	std::vector<GameObject*> objects = { city, car1, car2, car3, car4, car5, car6, car7, car8, teapot1, teapot2, teapot3, main_plane };

	/* Lights */
	LampShader* shader_program1 = LampShader::create();
	PointLight* p_light_1 = new PointLight(app, glm::vec3(40.0f, 2.0f, 8.0f), glm::vec3(1.0f, 0.0f, 1.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_2 = new PointLight(app, glm::vec3(-40.0f, 2.0f, -13.0f), glm::vec3(1.0f, 1.0f, 0.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_3 = new PointLight(app, glm::vec3(8.0f, 2.0f, -40.0f), glm::vec3(1.0f, .0f, 0.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_4 = new PointLight(app, glm::vec3(-13.0f, 2.0f, 40.0f), glm::vec3(0.0f, 1.0f, 0.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_5 = new PointLight(app, glm::vec3(40.0f, 2.0f, -13.0f), glm::vec3(1.0f, 0.0f, 1.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_6 = new PointLight(app, glm::vec3(-40.0f, 2.0f, 8.0f), glm::vec3(1.0f, 1.0f, 0.0f), .3f, 0.8f, 0.1f);

	PointLight* p_light_7 = new PointLight(app, glm::vec3(60, 10, 8), glm::vec3(1.0f, .0f, 0.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_8 = new PointLight(app, glm::vec3(60, 10, 0), glm::vec3(0.0f, 1.0f, 0.0f), .3f, 0.8f, 0.1f);
	PointLight* p_light_9 = new PointLight(app, glm::vec3(60, 10, -8), glm::vec3(1.0f), .3f, 0.8f, 0.1f);

	p_light_8->set_circular_speed(glm::vec2(40, 40));
	p_light_8->set_distance_from_center(5);
	p_light_7->set_circular_speed(glm::vec2(40, 40));
	p_light_7->set_distance_from_center(5);
	p_light_9->set_circular_speed(glm::vec2(40, 40));
	p_light_9->set_distance_from_center(5);

	std::vector<PointLight*> point_lights = { p_light_1, p_light_2, p_light_3, p_light_4, p_light_5, p_light_6, p_light_7, p_light_8, p_light_9 };
	for (unsigned int i = 0; i < point_lights.size(); i++) point_lights[i]->set_parent(city);
	for (unsigned int i = 0; i < point_lights.size(); i++) point_lights[i]->set_shader_program(shader_program1);

	DirLight* d_light = new DirLight(app, glm::vec3(1.0f, -1.0f, -0.0f), glm::vec3(1.0f));
	d_light->set_shader_program(shader_program1);

	/* CubeMap */
	BasicShader* cube_map_shader = BasicShader::create("shaders/cube_vertex_shader.glsl", "shaders/cube_fragment_shader.glsl");
	CubeMap* cube_map = new CubeMap();
	cube_map->init(cube_map_shader);

	/* GUI */
	GuiRenderer* gui_renderer = new GuiRenderer();

	GuiTexture* first_gui = new GuiTexture(app, gui_renderer->get_shader_program());
	first_gui->set_scale(glm::vec2(0.2f, 0.2f));
	first_gui->set_position(glm::vec2(0.80f, 0.80f));

	GuiTexture* second_gui = new GuiTexture(app, gui_renderer->get_shader_program(), "textures/logo.png");
	second_gui->set_scale(glm::vec2(0.03f, 0.03f));
	second_gui->set_position(glm::vec2(0.0f, -0.0f));

	//gui_renderer->add_gui_texture(first_gui);
	gui_renderer->add_gui_texture(second_gui);

	std::vector<ShaderProgram*> shaders = { shader_program, shader_program1, cube_map_shader };

	FrameBuffer* frame_buffer = new FrameBuffer(app, first_gui);
	app->set_frame_buffer(frame_buffer);

	/* Setting up the Application */
	app->set_input_manager(input_manager);
	app->set_game_objects(objects);
	app->set_directional_light(d_light);
	app->set_point_lights(point_lights);
	app->set_cube_map(cube_map);
	app->set_gui_renderer(gui_renderer);
	app->set_shaders(shaders);

	/* Run the loop */
	app->runMainGameLoop();
}