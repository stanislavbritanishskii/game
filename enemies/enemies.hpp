#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "utils.hpp"
#include "enemy.hpp"

class Enemies {
private:
	PlayerPosition player_pos;
	std::vector<Enemy> enemies;
	float _fps;
	GLuint shaderProgram;
	GLuint VAO;
	int _screen_width;
	int _screen_height;
public:
	Enemies(float x, float y, float orientation, float fps, int screen_width, int screen_height, GLuint shader,
				GLuint VAO);
	~Enemies();
	void addEnemy(float x, float y, EnemyType type);
	void iterate(Map &my_map, Projectiles &prjcts, float x, float y, float orientation);
};
#endif