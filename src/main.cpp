
#include <player.hpp>

#include "tile.hpp"
#include "utils.hpp"
#include "map.hpp"
#include "player.hpp"
#include <thread> // For std::this_thread::sleep_for
#include <projectile.hpp>
#include "enemies.hpp"
#include "enemy.hpp"



void processInput(GLFWwindow *window, Player* player, Map &map, int width, int height, Projectiles &prjcts) {
	// Check if the ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); // Close the window
	}

	// Movement controls (WASD)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player->move_front(map);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player->move_back(map);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player->move_left(map);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player->move_right(map);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		player->rotate_left();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		player->rotate_right();
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		player->reset_orientation();
		map.clearChosenTiles();
	}

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
	int tile_size = 32 * RATIO;
	Player player;
	player.setVelocity(400 * RATIO);
	player.setRotationSpeed(200);
	player.setSize(25 * RATIO);
	player.setBulletCount(2);
	player.setTeleportDelay(0.2);
	GLuint texture = loadTexture(player_texture);
	Map my_map(width, height, map_width, map_height, window, texture, shaderProgram, VAO);
	my_map.setTileSize(tile_size);

	player.setAccuracy(1);
	player.setBulletSpeed(400 * RATIO);

	Projectiles prjcts(0,0,0,shaderProgram, VAO, width, height, 32 * RATIO, 60);
	double lastTime = glfwGetTime();

	float fps = 60;
	float frame_duration = 1 / fps;
	double next_frame_time = lastTime + frame_duration;
	GLuint pumpkin_tex = loadTexture(pumpkin_texture);
	std::vector<Enemy> enemies;
	for (int i =10; i < 1000; i++)
		enemies.push_back(Enemy((std::rand() % (map_width - 2 ) - map_width / 2 + 2) * tile_size , (std::rand() % (map_height - 2)  - map_height / 2 +2) * tile_size, 100 * RATIO, 60, 5, 400 * RATIO, 1, 10, 1, 10, 10, true, ProjectileType::pumpkin_proj, pumpkin_tex, EnemyType::pumpkin, 30, 32 * RATIO, 400 * RATIO));


	std::cout << std::endl;
	while (!glfwWindowShouldClose(window))
	{

		if (glfwGetTime() < next_frame_time)
		{
			std::this_thread::sleep_for(std::chrono::duration<double>(lastTime + frame_duration - glfwGetTime()));
		}
		fps = 1 / (glfwGetTime() - lastTime);
		std::cout << "\rfps: "<< (int)fps<<std::flush;
		lastTime = glfwGetTime();
		next_frame_time = lastTime + frame_duration;

		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window, &player, my_map, width, height, prjcts);
		my_map.setX(player.getX());
		my_map.setY(player.getY());
		my_map.setOrientation(player.getOrientation());
		my_map.drawMap(shaderProgram, VAO);
		player.draw(shaderProgram, VAO, width, height);
		player.check_for_hit(prjcts);
		for (auto &enemy : enemies)
		{
			enemy.setPlayerPosition(player.getX(), player.getY(), player.getOrientation());
			if (enemy.getActive())
			{
				enemy.move(my_map);
				enemy.shoot(prjcts);
				enemy.draw(shaderProgram, VAO, width, height);
				enemy.check_for_hit(prjcts);
			}
		}
		prjcts.setX(player.getX());
		prjcts.setY(player.getY());
		prjcts.setOrientation(player.getOrientation());
		prjcts.move(my_map);
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
