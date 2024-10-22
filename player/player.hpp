#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "utils.hpp"
#include "map.hpp"
#include "projectile.hpp"

typedef struct cursor {
	int x;
	int y;
}cursor_t;
class Player {
private:
	float _x;
	float _y;
	float _orientation;
	float _velocity;
	float _rotation_speed;
	float _fps;
	int _size;
	cursor_t _cursor;
	GLuint _texture;
	float _shoot_delay;
	float _teleport_delay;
	int _bullet_count;
	float _bullet_speed;
	int _accuracy;
	float _bullet_lifetime;
	double _last_shot;
	double _last_teleport;
	float _max_hp;
	float _cur_hp;
	float nova_delay;
	double last_nova;
	int nova_count;

public:
	Player();

	~Player();

	float getX();

	float getY();

	float getOrientation();

	float getVelocity();

	float getRotationSpeed();

	int getFps();

	float getCurHP();

	void setX(float x);

	void setY(float y);

	void setOrientation(float orientation);

	void setVelocity(float velocity);

	void setRotationSpeed(float rotation_speed);

	void setFps(float fps);

	void setSize(int size);

	void rotate_left();

	void rotate_right();

	void move_front(Map &map);

	void move_back(Map &map);

	void move_left(Map &map);

	void move_right(Map &map);

	void move(float new_x, float new_y, Map &map);

	void draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height);

	void update_cursor(int x, int y);

	int getCursorX() const;

	int getCursorY() const;

	void reset_orientation();

	void teleport(Map map);

	void setShootDelay(float delay);
	void setTeleportDelay(float delay);
	void setBulletCount(int count);
	void setBulletSpeed(float speed);
	void setBulletLifetime(float lifetime);
	void setAccuracy(int accuracy);
	void shoot(Projectiles &projs);
	void shoot_nova(Projectiles &projs);
	int getBulletCount();
	void check_for_hit(Projectiles &prjs);
};

#endif
