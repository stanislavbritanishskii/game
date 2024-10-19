// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// // #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

// #include <iostream>
#include <player.hpp>

#include "tile.hpp"
#include "utils.hpp"
#include "map.hpp"
#include "player.hpp"

void processInput(GLFWwindow *window, Player* player) {
	// Check if the ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); // Close the window
	}

	// Movement controls (WASD)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		std::cout << "W key pressed: Moving up!" << std::endl;
		player->move_front();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		std::cout << "S key pressed: Moving down!" << std::endl;
		player->move_back();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		std::cout << "A key pressed: Moving left!" << std::endl;
		player->move_left();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		std::cout << "D key pressed: Moving right!" << std::endl;
		player->move_right();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		std::cout << "Q key pressed: rotating left!" << std::endl;
		player->rotate_left();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		std::cout << "E key pressed: rotating right!" << std::endl;
		player->rotate_right();
	}
}

int main()
{
	// Initialize GLFW
	int width = 800;
	int height = 800;
	GLFWwindow *window = init_glfw_window(width, height);

	// Compile shaders
	GLuint shaderProgram = init_shader();
	// Load the texture
	GLuint texture = loadTexture("textures/normal.png");

	// Set up vertices

	GLuint VAO, VBO, EBO;
	setupVertices(VAO, VBO, EBO);

	// Main loop
	Map my_map(width, height, 1000, 1000);
	my_map.setTileSize(20);
	Player player;
	player.setVelocity(300);
	player.setRotationSpeed(100);
	float rotation = 0;
	while (!glfwWindowShouldClose(window))
	{
		rotation += 0.05f;
		// Input handling
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Calculate position based on rotation
		float x = sin(rotation / 180 * M_PI) * 100; // X position
		float y = cos(rotation / 180 * M_PI) * 100; // Y position

		// Render the texture
		// renderTexture(shaderProgram, texture, VAO, x, y, rotation, width, height, 100);
		// renderTexture(shaderProgram, texture, VAO, -x, -y, rotation, width, height, 100);
		// renderTexture(shaderProgram, texture, VAO, 0,0, rotation, width, height, 100);
		processInput(window, &player);
		std::cout << player.getX() << " " << player.getY() << " " << player.getOrientation() << std::endl;
		my_map.setX(player.getX());
		my_map.setY(player.getY());
		my_map.setOrientation(player.getOrientation());
		my_map.drawMap(shaderProgram, VAO);

		// Swap buffers
		glfwSwapBuffers(window);
	}


	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);
	glfwTerminate();
	return 0;
}
