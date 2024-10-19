#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "utils.hpp"
#include "map.hpp"

class Player {
private:
	float _x;
	float _y;
	float _orientation;
	float _velocity;
	float _rotation_speed;
	int _fps;
	int _size;
	GLuint _texture;


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

	void move_front(Map map);

	void move_back(Map map);

	void move_left(Map map);

	void move_right(Map map);

	void draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height);
};

#endif
