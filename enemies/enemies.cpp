#include "enemies.hpp"


Enemy Enemies::create_enemy(float x, float y, EnemyType type, float fps)
{
	if (type == EnemyType::pumpkin)
	{
		// Enemy res(x, y, PUMPKIN_SPEED, fps, PUMPKIN_BULLET_COUNT, PUMPKIN_BULLET_SPEED, PUMPKIN_BULLET_DURATION,
		// 		PUMPKIN_DAMAGE, PUMPKIN_SHOOT_DELAY, PUMPKIN_HP, PUMPKIN_HP, true, ProjectileType::pumpkin_proj, enemy_textures[EnemyType::pumpkin],
		// 		type, PUMPKIN_SPREAD, PUMPKIN_SIZE, PUMPKIN_ACTIVE_DISTANCE);
		EnemyData data = config.getEnemyData(type);
		Enemy res(x, y,
				  data.speed,
				  fps,
				  data.bullet_count,
				  data.bullet_speed,
				  data.bullet_duration,
				  data.damage,
				  data.shoot_delay,
				  data.health,
				  data.health,
				  true,
				  ProjectileType::pumpkin_proj,
				  enemy_textures[EnemyType::pumpkin],
				  type,
				  data.spread,
				  data.size,
				  data.active_distance);

		return res;
	}
}

Enemies::Enemies(float x, float y, float orientation, float fps, int screen_width, int screen_height, GLuint shader,
				GLuint VAO, ConfigReader &conf): _screen_width(screen_width), _screen_height(screen_height), shaderProgram(shader), VAO(VAO)
{
	config = conf;
	_fps = fps;
	player_pos.x = x;
	player_pos.y = y;
	player_pos.orientation = orientation;
	enemy_textures.emplace(EnemyType::pumpkin, loadTexture(conf.getEnemyData(EnemyType::pumpkin).texture_name.c_str()));

}

Enemies::~Enemies()
{
}

void Enemies::addEnemy(float x, float y, EnemyType type)
{
	enemies.push_back(create_enemy(x, y, type, _fps));
}

void Enemies::iterate(Map &my_map, Projectiles &prjcts, float x, float y, float orientation, double delta_time)
{
	int i =0;
	for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		it->setPlayerPosition(x, y, orientation);
		if (it->getActive())
		{
			// enemy.move(my_map, delta_time);
			it->BFSMove(my_map, delta_time);
			it->shoot(prjcts);
			it->draw(shaderProgram, VAO, _screen_width, _screen_height);
			it->check_for_hit(prjcts);
			// std::cout << i++ << std::endl;
		}
	}
}
