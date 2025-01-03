#include "enemies.hpp"


std::map<Direction, std::vector<GLuint>> load_textures(TextureManager text_conf)
{
	std::map<Direction, std::vector<GLuint>> res;
	std::vector<std::string> text_names;
	std::vector<GLuint> texts;

	// Load textures for the "up" direction
	text_names = text_conf.getTextures(Direction::up);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::up].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	// res[Direction::up] = texts;
	texts.clear();

	// Load textures for the "down" direction
	text_names = text_conf.getTextures(Direction::down);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::down].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	////res[Direction::down] = texts;
	texts.clear();

	// Load textures for the "left" direction
	text_names = text_conf.getTextures(Direction::left);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::left].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	//res[left] = texts;
	texts.clear();

	// Load textures for the "right" direction
	text_names = text_conf.getTextures(Direction::right);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::right].push_back(loadTexture(it->c_str()));

		texts.push_back(loadTexture(it->c_str()));
	}
	//res[right] = texts;
	texts.clear();

	// Load textures for the "still_up" direction
	text_names = text_conf.getTextures(Direction::still_up);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::still_up].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	//res[still_up] = texts;
	texts.clear();

	// Load textures for the "still_down" direction
	text_names = text_conf.getTextures(Direction::still_down);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::still_down].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	//res[still_down] = texts;
	texts.clear();

	// Load textures for the "still_left" direction
	text_names = text_conf.getTextures(Direction::still_left);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::still_left].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	//res[still_left] = texts;
	texts.clear();

	// Load textures for the "still_right" direction
	text_names = text_conf.getTextures(Direction::still_right);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		res[Direction::still_right].push_back(loadTexture(it->c_str()));
		texts.push_back(loadTexture(it->c_str()));
	}
	//res[still_right] = texts;
	return res;
}

Enemy Enemies::create_enemy(float x, float y, EnemyType type, float fps)
{
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
				  data.active_distance,
				  data.xp);
		return res;

}

Enemies::Enemies(float x, float y, float orientation, float fps, int screen_width, int screen_height, GLuint shader,
				GLuint VAO, ConfigReader &conf): _screen_width(screen_width), _screen_height(screen_height), shaderProgram(shader), VAO(VAO)
{
	config = conf;
	_fps = fps;
	player_pos.x = x;
	player_pos.y = y;
	player_pos.orientation = orientation;
	TextureManager textures;
	std::vector<EnemyType> allEnemies = { pumpkin, Slime3, Slime2, Slime1, Slime4, Wolf, Orc, Bee};

	for (EnemyType type : allEnemies) {

	enemy_textures.emplace(type, loadTexture(conf.getEnemyData(type).texture_name.c_str()));
	textures.loadFromFile(conf.getEnemyData(type).texture_path.c_str());
	textures_by_type[type] = load_textures(textures);
	}

}

Enemies::~Enemies()
{
}

void Enemies::addEnemy(float x, float y, EnemyType type)
{
	enemies.push_back(create_enemy(x, y, type, _fps));
}

bool Enemies::addEnemy(Map &my_map, float x, float y, EnemyType type)
{
	if (my_map.is_obstacle(x ,y))
		return false;
	enemies.push_back(create_enemy(x, y, type, _fps));
	return true;
}

int Enemies::iterate(Map &my_map, Projectiles &prjcts, float x, float y, float orientation, double delta_time)
{
	int i =0;
	int exp_gained = 0;
	for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		it->setPlayerPosition(x, y, orientation);
		if (it->getActive())
		{
			// enemy.move(my_map, delta_time);
			it->BFSMove(my_map, delta_time);
			it->shoot(prjcts);
			it->draw(shaderProgram, VAO, _screen_width, _screen_height, textures_by_type[it->getType()]);
			exp_gained +=  it->check_for_hit(prjcts);
			// std::cout << i++ << std::endl;
		}
	}
	return exp_gained;
}
