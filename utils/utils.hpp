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
#include <vector>

#define RATIO 1

inline const char * player_texture = "textures/player.png";
inline const char * main_map_tile = "textures/normal.png";
inline const char * obstacle1_tile = "textures/obstacle.png";
inline const char * obstacle2_tile = "textures/obstacle2.png";
inline const char * cursor = "textures/crosshair.png";
inline const char * bigX = "textures/x.png";
inline const char * player_projectile = "textures/arrow.png";
inline const char * pumpkin_texture = "textures/pumpkin.png";


enum EnemyType {
	pumpkin,
	Slime3,
	Slime2,
	Slime1,
	Slime4,
	Wolf,
	Orc,
	Bee
};

enum ProjectileType {
	player_proj,
	pumpkin_proj,
	Slime1_proj,
	Slime2_proj,
	Slime3_proj,
	Slime4_proj,
	Wolf_proj,
	Orc_proj,
	Bee_proj
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
}Projectile_t;

struct ProjectileData {
	std::string texture_path;
	float rotation;
	GLuint texture;
	ProjectileType projectile_type;
};
enum Direction {
	up,
	down,
	left,
	right,
	still_up,
	still_down,
	still_left,
	still_right
};

inline const float PUMPKIN_SPEED = 100;
inline const float PUMPKIN_HP = 10;
inline const float PUMPKIN_DAMAGE = 100;
inline const float PUMPKIN_BULLET_COUNT = 5;
inline const float PUMPKIN_SPREAD = 90;
inline const float PUMPKIN_SIZE = 32 * RATIO;
inline const float PUMPKIN_ACTIVE_DISTANCE = 320;
inline const float PUMPKIN_BULLET_SPEED = 100 * RATIO;
inline const float PUMPKIN_BULLET_DURATION = 3;
inline const float PUMPKIN_SHOOT_DELAY = 1;




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