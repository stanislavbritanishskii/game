#ifndef UTILS_HPP
#define UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h> // Only include here, no implementation
#include <iostream>


inline const char * player_texture = "textures/player.png";
inline const char * main_map_tile = "textures/normal.png";
inline const char * obstacle1_tile = "textures/obstacle.png";
inline const char * obstacle2_tile = "textures/obstacle2.png";
inline const char * cursor = "textures/crosshair.png";
inline const char * bigX = "textures/x.png";
inline const char * player_projectile = "textures/arrow.png";
inline const char * enemy1 = "textures/pumpkin.png";

enum ProjectileType {
	player_proj,
	enemy_proj1
};

typedef struct Projectile {
	float x;
	float y;
	float orientation;
	bool active;
	float speed;
	ProjectileType type;
	double start_time;
	double lifetime;
	float damage;
};

GLFWwindow* init_glfw_window(int x, int y);
GLuint loadTexture(const char* path);
void setupVertices(GLuint& VAO, GLuint& VBO, GLuint& EBO);
GLuint  init_shader();
void renderTexture(unsigned int shaderProgram, unsigned int texture, unsigned int VAO, float x, float y, float rotation,
					int width, int height, float scale);


#endif