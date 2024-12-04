
#include <player.hpp>

#include "tile.hpp"
#include "utils.hpp"
#include "map.hpp"
#include "player.hpp"
#include <thread> // For std::this_thread::sleep_for
#include <projectile.hpp>
#include "enemies.hpp"
#include "enemy.hpp"
#include <iomanip>
#include "config_reader.hpp"
#include "texture_reader.hpp"




void processInput(GLFWwindow *window, Player* player, Map &map, int width, int height, Projectiles &prjcts, double time, double delta_time) {
	// Check if the ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); // Close the window
	}

	// Movement controls (WASD)
	int move_up = 0;
	int move_right = 0;
	int rotate_right = 0;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move_up ++;
		// player->move_front(map);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move_up --;
		// player->move_back(map);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		move_right --;
		// player->move_left(map);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		move_right ++;
		// player->move_right(map);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		rotate_right --;
		// player->rotate_left();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		rotate_right ++;
		// player->rotate_right();
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		player->reset_orientation();
		map.clearChosenTiles();
	}
	player->full_move(map, move_up, move_right, rotate_right, delta_time);

	// Mouse position tracking
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	player->update_cursor(xpos - width / 2, ypos - height / 2);

	// Mouse button tracking
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		player->teleport(map);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		player->shoot_nova(prjcts);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		player->shoot(prjcts);
		// prjcts->add_projectile(ProjectileType::player_proj, player->getX(), player->getY(), player->getOrientation() + atan2(ypos - height / 2, xpos - width / 2)/ M_PI * 180 + 90, 50);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		std::cout << "Right Mouse Button Pressed" << std::endl;
	}
}


int main()
{
	// Initialize GLFW
	float fps = 120;

	ConfigReader config;
	config.loadFromJSON("configs/config.json");
	int width = 800 * RATIO;
	int height = 800 * RATIO;
	GLFWwindow *window = init_glfw_window(width, height);

	// Compile shaders
	GLuint shaderProgram = init_shader();
	// Set up vertices

	GLuint VAO, VBO, EBO;
	setupVertices(VAO, VBO, EBO);

	// Main loop
	int map_width = 1000;
	int map_height =1000;
	float enemies_per_tile = 0.003;
	int tile_size = 32 * RATIO;
	PlayerData playerData = config.getPlayerData();
	TextureManager player_textures;
	player_textures.loadFromFile(playerData.texture_config_path);
	Player player;
	std::vector<GLuint> up_textures;
	std::vector<GLuint> down_textures;
	std::vector<GLuint> right_textures;
	std::vector<GLuint> left_textures;

	player.set_all_textures(player_textures);



	player.setShootDelay(playerData.shoot_delay);
	player.setVelocity(playerData.velocity);
	player.setRotationSpeed(playerData.rotation_speed);
	player.setSize(playerData.size);
	player.setBulletCount(playerData.bullet_count);
	player.setTeleportDelay(playerData.teleport_delay);
	GLuint texture = loadTexture(player_texture);
	player.setNovaDelay(playerData.nova_delay);
	player.setHitBox(playerData.hit_box);
	Map my_map(width, height, map_width, map_height, window, texture, shaderProgram, VAO);
	my_map.setTileSize(tile_size);

	player.setAccuracy(10);
	player.setBulletSpeed(400 * RATIO);
	Enemies enemies(player.getX(), player.getY(), player.getOrientation(), fps ,width, height, shaderProgram, VAO, config);
	Projectiles prjcts(0,0,0,shaderProgram, VAO, width, height, 32 * RATIO, 60);
	double lastTime = glfwGetTime();

	float frame_duration = 1 / fps;
	double delta_time;
	double next_frame_time = lastTime + frame_duration;
	double current_time = glfwGetTime();
	GLuint pumpkin_tex = loadTexture(pumpkin_texture);
	// std::vector<Enemy> enemies;
	for (int i =0; i < map_height * map_width * enemies_per_tile; i++)
		// enemies.push_back(Enemy((std::rand() % (map_width - 2 ) - map_width / 2 + 2) * tile_size , (std::rand() % (map_height - 2)  - map_height / 2 +2) * tile_size, 100 * RATIO, 60, 5, 400 * RATIO, 1, 10, 1, 10, 10, true, ProjectileType::pumpkin_proj, pumpkin_tex, EnemyType::pumpkin, 30, 32 * RATIO, 400 * RATIO));
	{
		enemies.addEnemy((std::rand() % (map_width - 2 ) - map_width / 2 + 2) * tile_size , (std::rand() % (map_height - 2)  - map_height / 2 +2) * tile_size, pumpkin);
		enemies.addEnemy((std::rand() % (map_width - 2 ) - map_width / 2 + 2) * tile_size , (std::rand() % (map_height - 2)  - map_height / 2 +2) * tile_size, Slime3);
		enemies.addEnemy((std::rand() % (map_width - 2 ) - map_width / 2 + 2) * tile_size , (std::rand() % (map_height - 2)  - map_height / 2 +2) * tile_size, Slime1);
	}

	std::cout << std::endl;
	std::cout << std::fixed << std::setprecision(1);
	while (!glfwWindowShouldClose(window))
	{
		current_time = glfwGetTime();
		while (glfwGetTime() < next_frame_time)
		{
			std::this_thread::sleep_for(std::chrono::duration<double>((lastTime + frame_duration - current_time) / 10));
			glfwPollEvents();

		}
		current_time = glfwGetTime();
		delta_time = current_time - lastTime;
		fps = 1 / delta_time;
		// std::cout << "\r\033[2Kfps: "<< fps<<std::flush;
		lastTime = current_time;
		next_frame_time = lastTime + frame_duration;

		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window, &player, my_map, width, height, prjcts, current_time, delta_time);
		my_map.setX(player.getX());
		my_map.setY(player.getY());
		my_map.setOrientation(player.getOrientation());
		my_map.calculateBFSDistance(30);
		my_map.drawMap(shaderProgram, VAO);
		player.draw(shaderProgram, VAO, width, height);
		player.check_for_hit(prjcts);
		enemies.iterate(my_map, prjcts, player.getX(), player.getY(), player.getOrientation(), delta_time);
		// for (auto &enemy : enemies)
		// {
		// 	enemy.setPlayerPosition(player.getX(), player.getY(), player.getOrientation());
		// 	if (enemy.getActive())
		// 	{
		// 		// enemy.move(my_map, delta_time);
		// 		enemy.BFSMove(my_map, delta_time);
		// 		enemy.shoot(prjcts);
		// 		enemy.draw(shaderProgram, VAO, width, height);
		// 		enemy.check_for_hit(prjcts);
		// 	}
		// }
		prjcts.setX(player.getX());
		prjcts.setY(player.getY());
		prjcts.setOrientation(player.getOrientation());
		prjcts.move(my_map, current_time, delta_time);
		prjcts.draw();
		// Swap buffers
		glfwSwapBuffers(window);
	}


	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
