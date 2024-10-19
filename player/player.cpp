#include "player.hpp"
#include <cmath>

#define PI 3.14159265

Player::Player()
	: _x(0.0f), _y(0.0f), _orientation(0.0f), _velocity(5.0f), _rotation_speed(5.0f), _fps(60) {}

Player::~Player() {}

float Player::getX() {
	return _x;
}

float Player::getY() {
	return _y;
}

float Player::getOrientation() {
	return _orientation;
}

float Player::getVelocity() {
	return _velocity;
}

float Player::getRotationSpeed() {
	return _rotation_speed;
}

int Player::getFps() {
	return _fps;
}

void Player::setX(float x) {
	_x = x;
}

void Player::setY(float y) {
	_y = y;
}

void Player::setOrientation(float orientation) {
	_orientation = orientation;
}

void Player::setVelocity(float velocity) {
	_velocity = velocity;
}

void Player::setRotationSpeed(float rotation_speed) {
	_rotation_speed = rotation_speed;
}

void Player::setFps(int fps) {
	_fps = fps;
}

// Rotate left by decreasing the orientation, considering FPS
void Player::rotate_left() {
	_orientation -= _rotation_speed * (1.0f / _fps);
	if (_orientation < 0) {
		_orientation += 360.0f;
	}
}

// Rotate right by increasing the orientation, considering FPS
void Player::rotate_right() {
	_orientation += _rotation_speed * (1.0f / _fps);
	if (_orientation >= 360.0f) {
		_orientation -= 360.0f;
	}
}

// Move the player forward in the direction they are facing, considering FPS
void Player::move_front() {
	_x += _velocity * (1.0f / _fps) * cos(_orientation * PI / 180.0f);
	_y -= _velocity * (1.0f / _fps) * sin(_orientation * PI / 180.0f);
}

// Move the player backward opposite to the direction they are facing, considering FPS
void Player::move_back() {
	_x -= _velocity * (1.0f / _fps) * cos(_orientation * PI / 180.0f);
	_y += _velocity * (1.0f / _fps) * sin(_orientation * PI / 180.0f);
}

// Strafe left (move sideways) relative to the current orientation, considering FPS
void Player::move_left() {
	_x -= _velocity * (1.0f / _fps) * cos((_orientation + 90.0f) * PI / 180.0f);
	_y += _velocity * (1.0f / _fps) * sin((_orientation + 90.0f) * PI / 180.0f);
}

// Strafe right (move sideways) relative to the current orientation, considering FPS
void Player::move_right() {
	_x += _velocity * (1.0f / _fps) * cos((_orientation + 90.0f) * PI / 180.0f);
	_y -= _velocity * (1.0f / _fps) * sin((_orientation + 90.0f) * PI / 180.0f);
}
