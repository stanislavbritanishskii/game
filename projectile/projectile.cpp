#include "projectile.hpp"

Projectiles::Projectiles(float x, float y, float orientation, GLuint shader, GLuint VAO, int screen_width, int screen_height, int proj_size, float fps)
	: _x(x), _y(y), _orientation(orientation), shader(shader), VAO(VAO), screen_width(screen_width), screen_height(screen_height), proj_size(proj_size), _fps(fps) {
	// Initialize textures for different projectile types
	// Load texture for player projectile as an example
	_textures[player_proj] = loadTexture(player_projectile); // Assuming you have a loadTexture function in utils.hpp
	_textures[enemy_proj1] = loadTexture(player_projectile);
}

Projectiles::~Projectiles() {
	// Cleanup resources if necessary
}

void Projectiles::move(Map map) {
	double cur_time = glfwGetTime();
	for (auto &proj : _projectiles) {
		if (proj.active) {
			// Move the projectile based on its speed and orientation

			proj.x += proj.speed * cos(proj.orientation) / _fps;
			proj.y += proj.speed * sin(proj.orientation) / _fps;

			// Check if the projectile is still inside the map boundaries
			if (map.is_obstacle(proj.x, proj.y) || proj.start_time + proj.lifetime < cur_time) {
				proj.active = false; // Deactivate if outside map
			}
		}
	}
}

float Projectiles::getX() const {
	return _x;
}

float Projectiles::getY() const {
	return _y;
}

void Projectiles::setX(float x) {
	_x = x;
}

void Projectiles::setY(float y) {
	_y = y;
}

void Projectiles::setOrientation(float orientation) {
	_orientation = orientation + 90;
}

void Projectiles::draw() {
	float cos_theta = cos(_orientation / 180 * M_PI);
	float sin_theta = sin(_orientation / 180 * M_PI);

	for (const auto &proj : _projectiles) {
		if (proj.active) {
			float new_x = (proj.x - this->_x) * cos_theta - (proj.y - this->_y) * sin_theta;
			float new_y = (proj.x - this->_x) * sin_theta + (proj.y - this->_y) * cos_theta;
			renderTexture(shader, _textures[proj.type], VAO, new_x, new_y,
				 _orientation + proj.orientation / M_PI * 180 + 50, screen_width, screen_height, proj_size);
		}
	}
}

bool Projectiles::is_active() {
	for (const auto &proj : _projectiles) {
		if (proj.active) {
			return true; // At least one projectile is active
		}
	}
	return false;
}

void Projectiles::add_projectile(ProjectileType type, float x, float y, float orientation, float speed) {
	Projectile new_proj = { x, y, -(float)(orientation / 180 * M_PI), true, speed, type };
	_projectiles.push_back(new_proj);
}

void Projectiles::add_projectile(Projectile proj)
{
	_projectiles.push_back(proj);
}


float Projectiles::get_damage(float x, float y, float size, ProjectileType type)
{
	float res = 0;
	for (auto &proj : _projectiles)
	{
		if (proj.active && proj.type == type) {
			if (pow(proj.x - x, 2) + pow(proj.y - y, 2) < pow(size, 2)) {
				res += proj.damage;
				proj.active = false;
			}
		}
	}
	return res;
}
