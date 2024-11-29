#include "player.hpp"
#include <cmath>

#define PI 3.14159265

Player::Player()
	: _x(0.0f), _y(0.0f), _orientation(0.0f), _velocity(5.0f), _rotation_speed(5.0f), _fps(60.0f), _shoot_delay(0.1),
	_teleport_delay(1), _bullet_count(1), _bullet_speed(200),_accuracy(60), _bullet_lifetime(1), _max_hp(1000), _cur_hp(1000), nova_delay(2),last_nova(0), nova_count(20), hp_bar(32,32,3)
{
	_texture = loadTexture(player_texture);
	_size = 32;
	_cursor.x = 0;
	_cursor.y = 0;
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
		x_size = _size / 2;
	if (new_x < _x)
		x_size = -_size / 2;
	if (new_y > _y)
		y_size = _size / 2;
	if (new_y < _y)
		y_size = -_size / 2;
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
	move(new_x, new_y, map);

}

void Player::draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height)
{
	renderTexture(shader_program, _texture, VAO, 0, 0,
				180, screen_width, screen_height, _size);

	renderTexture(shader_program, hp_bar.getContourTexture(), VAO, 0, -_size / 2,
				180, screen_width, screen_height, _size);
	renderTexture(shader_program, hp_bar.getRedTexture(), VAO, 0, -_size / 2,
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
	if (glfwGetTime() > _shoot_delay + _last_shot)
	{
		for ( int i =0; i < _bullet_count; i++)
		{
			_last_shot = glfwGetTime();
			float offset = (std::rand() % _accuracy) - _accuracy / 2;

			float orientation = atan2(_x - _cursor.x, _cursor.y - _y) + M_PI / 2.0f + offset / 180 * M_PI;


			Projectile new_proj = {
					_x, _y, orientation, true, _bullet_speed, ProjectileType::player_proj,
					glfwGetTime(), _bullet_lifetime, 1
			};
			projs.add_projectile(new_proj);
		}
	}
}

void Player::shoot_nova(Projectiles &projs)
{
	if (glfwGetTime() > nova_delay + last_nova)
	{
		for ( int i =0; i < nova_count; i++)
		{
			float orientation = 2 * M_PI / nova_count * i;
			Projectile new_proj = {
					static_cast<float>(_cursor.x), static_cast<float>(_cursor.y), orientation, true, _bullet_speed, ProjectileType::player_proj,
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
	_cur_hp -= prjs.get_player_damage(_x, _y, _size);
	hp_bar.setCurrentHP(_cur_hp);
	hp_bar.setMaxHP(_max_hp);
	if (_cur_hp < 0)
	{
		exit(0);
	}
}