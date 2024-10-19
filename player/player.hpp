#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "utils.hpp"
class Player {
private:
	float _x;
	float _y;
	float _orientation;
	float _velocity;
	float _rotation_speed;
	int _fps;


public:
	Player();

	~Player();

	float getX();

	float getY();

	float getOrientation();

	float getVelocity();

	float getRotationSpeed();

	int getFps();

	void setX(float x);

	void setY(float y);

	void setOrientation(float orientation);

	void setVelocity(float velocity);

	void setRotationSpeed(float rotation_speed);

	void setFps(int fps);


	void rotate_left();

	void rotate_right();

	void move_front();

	void move_back();

	void move_left();

	void move_right();
};

#endif
