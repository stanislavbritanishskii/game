#include "player.hpp"
#include <cmath>

#define PI 3.14159265

Player::Player()
	: _x(0.0f), _y(0.0f), _orientation(0.0f), _velocity(5.0f), _rotation_speed(5.0f), _fps(60.0f), _shoot_delay(0.1),
	_teleport_delay(1), _bullet_count(1), _bullet_speed(200), _accuracy(60), _bullet_lifetime(1), _max_hp(1000),
	_cur_hp(1000), nova_delay(2), last_nova(0), nova_count(20), hp_bar(32, 32, 3), _level(0), _xp(0), _bullet_damage(1)
{
	_texture = loadTexture(player_texture);
	std::vector<GLuint> base_textures = {_texture};
	textures.insert_or_assign(Direction::up, base_textures);
	textures.insert_or_assign(Direction::down, base_textures);
	textures.insert_or_assign(Direction::left, base_textures);
	textures.insert_or_assign(Direction::right, base_textures);
	textures.insert_or_assign(Direction::still_up, base_textures);
	textures.insert_or_assign(Direction::still_down, base_textures);
	textures.insert_or_assign(Direction::still_right, base_textures);
	textures.insert_or_assign(Direction::still_left, base_textures);

	_size = 32;
	_cursor.x = 0;
	_cursor.y = 0;
	_cur_texture = 0;
	time_to_change_texture = 0.1;
}

Player::~Player()
{
}

void Player::setSize(int size)
{
	_size = size;
}


float Player::getX()
{
	return _x;
}

float Player::getY()
{
	return _y;
}

float Player::getOrientation()
{
	return _orientation;
}

float Player::getVelocity()
{
	return _velocity;
}

float Player::getRotationSpeed()
{
	return _rotation_speed;
}

int Player::getFps()
{
	return _fps;
}

void Player::setX(float x)
{
	_x = x;
}

void Player::setY(float y)
{
	_y = y;
}

void Player::setOrientation(float orientation)
{
	_orientation = orientation;
}

void Player::setVelocity(float velocity)
{
	_velocity = velocity;
}

void Player::setRotationSpeed(float rotation_speed)
{
	_rotation_speed = rotation_speed;
}

void Player::setFps(float fps)
{
	_fps = fps;
}

float Player::getCurHP()
{
	return _cur_hp;
}

void Player::setMaxHp(float maxHp)
{
	_max_hp = maxHp;
	_cur_hp = maxHp;
}


// Rotate left by decreasing the orientation, considering FPS
void Player::rotate_left()
{
	_orientation -= _rotation_speed * (1.0f / _fps);
	if (_orientation < 0)
	{
		_orientation += 360.0f;
	}
}

// Rotate right by increasing the orientation, considering FPS
void Player::rotate_right()
{
	_orientation += _rotation_speed * (1.0f / _fps);
	if (_orientation >= 360.0f)
	{
		_orientation -= 360.0f;
	}
}


void Player::move(double new_x, double new_y, Map &map)
{
	double x_size = 0;
	double y_size = 0;
	if (new_x > _x)
		x_size = hit_box / 2;
	if (new_x < _x)
		x_size = -hit_box / 2;
	if (new_y > _y)
		y_size = hit_box / 2;
	if (new_y < _y)
		y_size = -hit_box / 2;
	if (map.is_obstacle(new_x + x_size, _y))
		new_x = _x;
	if (map.is_obstacle(_x, new_y + y_size))
		new_y = _y;
	if (map.is_obstacle(new_x + x_size, new_y + y_size))
	{
		new_x = _x;
		new_y = _y;
	}

	_x = new_x;
	_y = new_y;
}


// Move the player forward in the direction they are facing, considering FPS
void Player::move_front(Map &map)
{
	double new_x = _x + _velocity * (1.0f / _fps) * cos(_orientation * PI / 180.0f);
	double new_y = _y - _velocity * (1.0f / _fps) * sin(_orientation * PI / 180.0f);
	move(new_x, new_y, map);
}

// Move the player backward opposite to the direction they are facing, considering FPS
void Player::move_back(Map &map)
{
	double new_x = _x - _velocity * (1.0f / _fps) * cos(_orientation * PI / 180.0f);
	double new_y = _y + _velocity * (1.0f / _fps) * sin(_orientation * PI / 180.0f);
	move(new_x, new_y, map);
}

// Strafe left (move sideways) relative to the current orientation, considering FPS
void Player::move_left(Map &map)
{
	double new_x = _x - _velocity * (1.0f / _fps) * cos((_orientation + 90.0f) * PI / 180.0f);
	double new_y = _y + _velocity * (1.0f / _fps) * sin((_orientation + 90.0f) * PI / 180.0f);
	move(new_x, new_y, map);
}

// Strafe right (move sideways) relative to the current orientation, considering FPS
void Player::move_right(Map &map)
{
	double new_x = _x + _velocity * (1.0f / _fps) * cos((_orientation + 90.0f) * PI / 180.0f);
	double new_y = _y - _velocity * (1.0f / _fps) * sin((_orientation + 90.0f) * PI / 180.0f);
	move(new_x, new_y, map);
}

void Player::full_move(Map &map, int up, int right, int rotate_right, double delta_time)
{
	double dist = _velocity * delta_time;
	if (up and right)
		dist /= M_SQRT2;

	double new_x = _x + up * (dist * cos(_orientation * PI / 180.0f));
	new_x += right * (dist * cos((_orientation + 90.0f) * PI / 180.0f));
	double new_y = _y - up * (dist * sin(_orientation * PI / 180.0f));
	new_y -= right * (dist * sin((_orientation + 90.0f) * PI / 180.0f));
	_orientation += _rotation_speed * delta_time * rotate_right;
	if (_orientation < 0)
	{
		_orientation += 360.0f;
	}
	if (_orientation > 360)
	{
		_orientation -= 360;
	}
	Direction new_direction;
	if (right > 0)
		new_direction = Direction::left;
	else if (right < 0)
		new_direction = Direction::right;
	else if (up < 0)
		new_direction = Direction::down;
	else if (up > 0)
		new_direction = Direction::up;
	else
		new_direction = Direction((int) direction % 4 + 4);
	if (new_direction != direction)
	{
		_cur_texture = 0;
		last_change_texture = glfwGetTime();
	}
	direction = new_direction;
	move(new_x, new_y, map);
}

void Player::draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height)
{
	double cur_time = glfwGetTime();
	if (cur_time > last_change_texture + time_to_change_texture)
	{
		last_change_texture = glfwGetTime();
		_cur_texture++;
		if (_cur_texture >= textures[direction].size())
		{
			_cur_texture = 0;
		}
	}
	_texture = textures[direction][_cur_texture];
	renderTexture(shader_program, _texture, VAO, 0, 0,
				180, screen_width, screen_height, _size);

	renderTexture(shader_program, hp_bar.getContourTexture(), VAO, 0, -hit_box / 2,
				180, screen_width, screen_height, _size);
	renderTexture(shader_program, hp_bar.getRedTexture(), VAO, 0, -hit_box / 2,
				180, screen_width, screen_height, _size);

}

void Player::update_cursor(int x, int y)
{
	_cursor.x = _x + (cos((_orientation + 90.0f) * PI / 180.0f) * x - sin((_orientation + 90.0f) * PI / 180.0f) * y);
	_cursor.y = _y - (sin((_orientation + 90.0f) * PI / 180.0f) * x + cos((_orientation + 90.0f) * PI / 180.0f) * y);
	// _cursor.x = 0;
}


int Player::getCursorX() const
{
	return _cursor.x;
}

int Player::getCursorY() const
{
	return _cursor.y;
}

void Player::reset_orientation()
{
	_orientation = 0;
}

void Player::teleport(Map &map)
{
	if (glfwGetTime() > _teleport_delay + _last_teleport && !map.is_obstacle(_cursor.x, _cursor.y))
	{
		_x = _cursor.x;
		_y = _cursor.y;
		_last_teleport = glfwGetTime();
	}
}


void Player::setAccuracy(int accuracy)
{
	_accuracy = accuracy;
}

void Player::setBulletCount(int count)
{
	_bullet_count = count;
}

void Player::setBulletLifetime(float lifetime)
{
	_bullet_lifetime = lifetime;
}

void Player::shoot(Projectiles &projs)
{
	if ((int) direction >= 4)
	{
		float orientation = atan2(_x - _cursor.x ,_y -  _cursor.y) + M_PI / 2.0f - _orientation / 180.0f * M_PI;
		while (orientation < -M_PI)
			orientation += 2 * M_PI;
		while (orientation > M_PI)
			orientation -= 2 * M_PI;
		if (orientation > -M_PI / 4 && orientation < M_PI / 4)
			direction = Direction::still_up;
		else if (orientation > -M_PI * 3 / 4 && orientation < -M_PI / 4)
			direction = Direction::still_right;
		else if (orientation > M_PI / 4 && orientation < M_PI * 3 / 4)
			direction = Direction::still_left;
		else if (orientation < -M_PI * 3 / 4 || orientation > M_PI * 3 / 4)
			direction = Direction::still_down;

		if (_cur_texture >= textures[direction].size())
		{
			_cur_texture = 0;
		}
	}
	if (glfwGetTime() > _shoot_delay + _last_shot)
	{
		for (int i = 0; i < _bullet_count; i++)
		{
			_last_shot = glfwGetTime();
			float offset = (std::rand() % _accuracy) - _accuracy / 2;

			float orientation = atan2(_x - _cursor.x, _cursor.y - _y) + M_PI / 2.0f + offset / 180 * M_PI;


			Projectile new_proj = {
				_x, _y, orientation, true, _bullet_speed, ProjectileType::player_proj,
				glfwGetTime(), _bullet_lifetime, _bullet_damage
			};
			projs.add_projectile(new_proj);
		}
	}
}

void Player::shoot_nova(Projectiles &projs)
{
	if (glfwGetTime() > nova_delay + last_nova)
	{
		for (int i = 0; i < nova_count; i++)
		{
			float orientation = 2 * M_PI / nova_count * i;
			Projectile new_proj = {
				static_cast<float>(_cursor.x), static_cast<float>(_cursor.y), orientation, true, _bullet_speed,
				ProjectileType::player_proj,
				glfwGetTime(), _bullet_lifetime, 1
			};
			projs.add_projectile(new_proj);
		}
		last_nova = glfwGetTime();
	}
}

void Player::setBulletSpeed(float speed)
{
	_bullet_speed = speed;
}

void Player::setShootDelay(float delay)
{
	_shoot_delay = delay;
}

void Player::setNovaDelay(float delay)
{
	nova_delay = delay;
}


void Player::setTeleportDelay(float delay)
{
	_teleport_delay = delay;
}

int Player::getBulletCount()
{
	return _bullet_count;
}

void Player::check_for_hit(Projectiles &prjs)
{
	_cur_hp -= prjs.get_player_damage(_x, _y, hit_box);
	hp_bar.setCurrentHP(_cur_hp);
	hp_bar.setMaxHP(_max_hp);
	if (_cur_hp <= 0)
	{
		exit(0);
	}
}

void Player::setHitBox(int new_hit_box)
{
	hit_box = new_hit_box;
}


void Player::set_up_textures(std::vector<GLuint> ups)
{
	textures.insert_or_assign(Direction::up, ups);
}

void Player::set_down_textures(std::vector<GLuint> downs)
{
	textures.insert_or_assign(Direction::down, downs);
}

void Player::set_left_textures(std::vector<GLuint> lefts)
{
	textures.insert_or_assign(Direction::left, lefts);
}

void Player::set_right_textures(std::vector<GLuint> rights)
{
	textures.insert_or_assign(Direction::right, rights);
}

void Player::set_still_up_textures(std::vector<GLuint> rights)
{
	textures.insert_or_assign(Direction::still_up, rights);
}

void Player::set_still_down_textures(std::vector<GLuint> rights)
{
	textures.insert_or_assign(Direction::still_down, rights);
}

void Player::set_still_right_textures(std::vector<GLuint> rights)
{
	textures.insert_or_assign(Direction::still_right, rights);
}

void Player::set_still_left_textures(std::vector<GLuint> rights)
{
	textures.insert_or_assign(Direction::still_left, rights);
}

void Player::set_all_textures(TextureManager text_conf)
{
	std::vector<std::string> text_names;
	std::vector<GLuint> texts;

	// Load textures for the "up" direction
	text_names = text_conf.getTextures(Direction::up);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_up_textures(texts);
	texts.clear();

	// Load textures for the "down" direction
	text_names = text_conf.getTextures(Direction::down);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_down_textures(texts);
	texts.clear();

	// Load textures for the "left" direction
	text_names = text_conf.getTextures(Direction::left);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_left_textures(texts);
	texts.clear();

	// Load textures for the "right" direction
	text_names = text_conf.getTextures(Direction::right);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_right_textures(texts);
	texts.clear();

	// Load textures for the "still_up" direction
	text_names = text_conf.getTextures(Direction::still_up);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_still_up_textures(texts);
	texts.clear();

	// Load textures for the "still_down" direction
	text_names = text_conf.getTextures(Direction::still_down);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_still_down_textures(texts);
	texts.clear();

	// Load textures for the "still_left" direction
	text_names = text_conf.getTextures(Direction::still_left);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_still_left_textures(texts);
	texts.clear();

	// Load textures for the "still_right" direction
	text_names = text_conf.getTextures(Direction::still_right);
	for (std::vector<std::string>::iterator it = text_names.begin(); it != text_names.end(); ++it)
	{
		texts.push_back(loadTexture(it->c_str()));
	}
	set_still_right_textures(texts);
}

void Player::level_up()
{
	_level++;
	int val = std::rand() % 7;
	if (val == 0)
	{
		_max_hp += 10;
	}
	else if (val == 1)
	{
		_velocity += 10;
	}
	else if (val == 2)
	{
		_shoot_delay -= 0.1;
		if (_shoot_delay < 0.1)
			_shoot_delay = 0.1;
	}
	else if (val == 3)
	{
		_bullet_count += 1;
	}
	else if (val == 4)
	{
		_accuracy -= 1;
		if (_accuracy < 1)
			_accuracy = 1;
	}
	else if (val == 5)
	{
		_bullet_lifetime += 0.1;
	}
	else if (val == 6)
	{
		_bullet_damage += 1;
	}
	_cur_hp = _max_hp;
	_start_level_up = glfwGetTime();
}


void Player::add_xp(int exp)
{
	_xp += exp;
	while (_xp >= (_level + 1) * 10)
	{
		_xp -= (_level + 1) * 10;
		level_up();
	}
}
// Implementations of the missing getters

float Player::getMaxHp() const {
	return _max_hp;
}

float Player::getShootDelay() const {
	return _shoot_delay;
}

float Player::getTeleportDelay() const {
	return _teleport_delay;
}

float Player::getBulletSpeed() const {
	return _bullet_speed;
}

float Player::getBulletLifetime() const {
	return _bullet_lifetime;
}

float Player::getBulletDamage() const {
	return _bullet_damage;
}

float Player::getNovaDelay() const {
	return nova_delay;
}

int Player::getNovaCount() const {
	return nova_count;
}

int Player::getLevel() const {
	return _level;
}

int Player::getXp() const {
	return _xp;
}

int Player::getHitBox() const {
	return hit_box;
}

int Player::getXpToNextLevel() const
{
	return (_level + 1) * 10;
}

int Player::getAccuracy() const
{
	return _accuracy;
}
