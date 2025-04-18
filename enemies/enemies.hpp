#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "utils.hpp"
#include "enemy.hpp"
#include "config_reader.hpp"
#include "texture_reader.hpp"


class Enemies {
private:
	std::map<EnemyType, GLuint> enemy_textures;
	PlayerPosition player_pos;
	std::vector<Enemy> enemies;
	float _fps;
	GLuint shaderProgram;
	GLuint VAO;
	int _screen_width;
	int _screen_height;
	std::map<EnemyType, std::map<Direction, std::vector<GLuint>>> textures_by_type;
	Enemy create_enemy(float x, float y, EnemyType type, float fps);
	ConfigReader config;
public:
	Enemies(float x, float y, float orientation, float fps, int screen_width, int screen_height, GLuint shader,
				GLuint VAO, ConfigReader &conf);
	~Enemies();
	void addEnemy(float x, float y, EnemyType type);
	bool addEnemy(Map &my_map, float x, float y, EnemyType type);
	int iterate(Map &my_map, Projectiles &prjcts, float x, float y, float orientation, double delta_time);
};
#endif