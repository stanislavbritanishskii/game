#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include "utils.hpp"
#include "map.hpp"



class Projectiles {
private:
	float _x;
	float _y;
	float _orientation;
	GLuint shader;
	GLuint VAO;
	int screen_width;
	int screen_height;
	std::map<ProjectileType, GLuint> _textures;
	std::vector<Projectile> _projectiles;
	int proj_size;
	float _fps;


public:
	Projectiles(float x, float y, float orientation, GLuint shader, GLuint VAO, int screen_width, int screen_height, int proj_size, float fps);

	~Projectiles();

	void setFPS(float fps);

	void move(Map map);

	float getX() const;

	float getY() const;

	void setX(float x);

	void setY(float y);

	void setOrientation(float orientation);

	void draw();

	bool is_active();

	void add_projectile(ProjectileType type, float x, float y, float orientation, float speed);

	void add_projectile(Projectile proj);

	float get_damage(float x, float y, float size, ProjectileType type);
};

#endif
