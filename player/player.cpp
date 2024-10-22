#include "player.hpp"
#include <cmath>

#define PI 3.14159265

Player::Player()
	: _x(0.0f), _y(0.0f), _orientation(0.0f), _velocity(5.0f), _rotation_speed(5.0f), _fps(60.0f), _shoot_delay(0.1),
	_teleport_delay(1), _bullet_count(1), _bullet_speed(200),_accuracy(60), _bullet_lifetime(1), _max_hp(1000), _cur_hp(1000)
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


void Player::move(float new_x, float new_y, Map map)
{
	float x_size = 0;
	float y_size = 0;
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
void Player::move_front(Map map)
{
	float new_x = _x + _velocity * (1.0f / _fps) * cos(_orientation * PI / 180.0f);
	float new_y = _y - _velocity * (1.0f / _fps) * sin(_orientation * PI / 180.0f);
	move(new_x, new_y, map);
}

// Move the player backward opposite to the direction they are facing, considering FPS
void Player::move_back(Map map)
{
	float new_x = _x - _velocity * (1.0f / _fps) * cos(_orientation * PI / 180.0f);
	float new_y = _y + _velocity * (1.0f / _fps) * sin(_orientation * PI / 180.0f);
	move(new_x, new_y, map);
}

// Strafe left (move sideways) relative to the current orientation, considering FPS
void Player::move_left(Map map)
{
	float new_x = _x - _velocity * (1.0f / _fps) * cos((_orientation + 90.0f) * PI / 180.0f);
	float new_y = _y + _velocity * (1.0f / _fps) * sin((_orientation + 90.0f) * PI / 180.0f);
	move(new_x, new_y, map);
}

// Strafe right (move sideways) relative to the current orientation, considering FPS
void Player::move_right(Map map)
{
	float new_x = _x + _velocity * (1.0f / _fps) * cos((_orientation + 90.0f) * PI / 180.0f);
	float new_y = _y - _velocity * (1.0f / _fps) * sin((_orientation + 90.0f) * PI / 180.0f);
	move(new_x, new_y, map);
}


void Player::draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height)
{
	renderTexture(shader_program, _texture, VAO, 0, 0,
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

void Player::teleport(Map map)
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
	_cur_hp -= prjs.get_damage(_x, _y, _size, ProjectileType::enemy_proj1);
	if (_cur_hp < 0)
	{
		exit(0);
	}
}