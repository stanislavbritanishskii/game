#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "utils.hpp"
#include "enemy.hpp"

class Enemies {
private:
	PlayerPosition player_pos;
	std::vector<Enemy> enemies;
public:
	Enemies(float x, float y, float orientation);
	~Enemies();
	void addEnemy(float x, float y, EnemyType type);
};
#endif