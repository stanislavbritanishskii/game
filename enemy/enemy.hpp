#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "utils.hpp"
#include "projectile.hpp"
#include "map.hpp"
enum EnemyType {
	pumpkin
};

typedef struct PlayerPosition {
	float x;
	float y;
	float orientation;
}PlayerPosition_T;

class Enemy {
private:
	float x;
	float y;
	float speed;
	float fps;
	float bullet_count;
	float bullet_speed;
	float bullet_duration;
	float bullet_damage;
	float shoot_delay;
	double last_shot;
	float max_hp;
	float current_hp;
	float bullet_spread;
	float size;
	bool alive;
	ProjectileType projectile_type;
	GLuint texture;
	EnemyType type;
	PlayerPosition_T player_pos;

public:
	// Default constructor
	Enemy();

	// Constructor with all parameters
	Enemy(float x, float y, float speed, float fps, float bullet_count, float bullet_speed,
		  float bullet_duration, float bullet_damage, float shoot_delay, float max_hp,
		  float current_hp, bool alive, ProjectileType projectile_type, GLuint texture,
		  EnemyType type, float bullet_spread, float size);

	// Getters
	float getX() const;
	float getY() const;
	float getSpeed() const;
	float getFps() const;
	float getBulletCount() const;
	float getBulletSpeed() const;
	float getBulletDuration() const;
	float getBulletDamage() const;
	float getShootDelay() const;
	float getMaxHp() const;
	float getCurrentHp() const;
	bool isAlive() const;
	ProjectileType getProjectileType() const;
	GLuint getTexture() const;
	EnemyType getType() const;

	// Setters
	void setX(float x);
	void setY(float y);
	void setSpeed(float speed);
	void setFps(float fps);
	void setBulletCount(float bullet_count);
	void setBulletSpeed(float bullet_speed);
	void setBulletDuration(float bullet_duration);
	void setBulletDamage(float bullet_damage);
	void setShootDelay(float shoot_delay);
	void setMaxHp(float max_hp);
	void setCurrentHp(float current_hp);
	void setAlive(bool alive);
	void setProjectileType(ProjectileType projectile_type);
	void setTexture(GLuint texture);
	void setType(EnemyType type);
	void setPlayerPosition(float player_x, float player_y, float player_or);

	void shoot(Projectiles &prjs);
	void draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height);
	void move(Map map);
	void check_for_hit(Projectiles &prjs);
};

#endif
