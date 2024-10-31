#include "enemies.hpp"


Enemy create_enemy(float x, float y, EnemyType type, float fps)
{
	if (type == EnemyType::pumpkin)
	{
		GLuint texture = loadTexture(pumpkin_texture);
		Enemy res(x, y, PUMPKIN_SPEED, fps, PUMPKIN_BULLET_COUNT, PUMPKIN_BULLET_SPEED, PUMPKIN_BULLET_DURATION,
				PUMPKIN_DAMAGE, PUMPKIN_SHOOT_DELAY, PUMPKIN_HP, PUMPKIN_HP, true, ProjectileType::pumpkin_proj, texture,
				type, PUMPKIN_SPREAD, PUMPKIN_SIZE, PUMPKIN_ACTIVE_DISTANCE);
		return res;
	}
}

Enemies::Enemies(float x, float y, float orientation, float fps, int screen_width, int screen_height, GLuint shader,
				GLuint VAO): _screen_width(screen_width), _screen_height(screen_height), shaderProgram(shader), VAO(VAO)
{
	_fps = fps;
	player_pos.x = x;
	player_pos.y = y;
	player_pos.orientation = orientation;
}

Enemies::~Enemies()
{
}

void Enemies::addEnemy(float x, float y, EnemyType type)
{
	enemies.push_back(create_enemy(x, y, type, _fps));
}

void Enemies::iterate(Map &my_map, Projectiles &prjcts, float x, float y, float orientation)
{
	for (auto &enemy: enemies)
	{
		enemy.setPlayerPosition(x, y, orientation);
		if (enemy.getActive())
		{
			enemy.move(my_map);
			enemy.shoot(prjcts);
			enemy.draw(shaderProgram, VAO, _screen_width, _screen_height);
			enemy.check_for_hit(prjcts);
		}
	}
}
