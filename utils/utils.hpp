#ifndef UTILS_HPP
#define UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h> // Only include here, no implementation
#include <iostream>
#include <map>

#define RATIO 1

inline const char * player_texture = "textures/player.png";
inline const char * main_map_tile = "textures/normal.png";
inline const char * obstacle1_tile = "textures/obstacle.png";
inline const char * obstacle2_tile = "textures/obstacle2.png";
inline const char * cursor = "textures/crosshair.png";
inline const char * bigX = "textures/x.png";
inline const char * player_projectile = "textures/arrow.png";
inline const char * pumpkin_texture = "textures/pumpkin.png";

enum ProjectileType {
	player_proj,
	pumpkin_proj
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

inline const float PUMPKIN_SPEED = 100;
inline const float PUMPKIN_HP = 100;
inline const float PUMPKIN_DAMAGE = 100;
inline const float PUMPKIN_BULLET_COUNT = 100;
inline const float PUMPKIN_SPREAD = 100;
inline const float PUMPKIN_SIZE = 100;
inline const float PUMPKIN_ACTIVE_DISTANCE = 100;
inline const float PUMPKIN_BULLET_SPEED = 100;
inline const float PUMPKIN_BULLET_DURATION = 100;
inline const float PUMPKIN_SHOOT_DELAY = 100;




GLFWwindow* init_glfw_window(int x, int y);
GLuint loadTexture(const char* path);
void setupVertices(GLuint& VAO, GLuint& VBO, GLuint& EBO);
GLuint  init_shader();
void renderTexture(unsigned int shaderProgram, unsigned int texture, unsigned int VAO, float x, float y, float rotation,
					int width, int height, float scale);
bool is_player_projectile(ProjectileType type);
bool is_enemy_projectile(ProjectileType type);
GLuint createTextTexture(const std::string &fontPath, const std::string &text, int fontSize, int &textureWidth, int &textureHeight);
std::map<char, GLuint> char_textures(const std::string &fontPath, int fontSize, int &textureWidth, int &textureHeight);

#endif