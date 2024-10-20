#include "enemy.hpp"

// Default constructor
Enemy::Enemy() : x(0.0f), y(0.0f), speed(0.0f), fps(60.0f), bullet_count(0.0f),
				bullet_speed(0.0f), bullet_duration(0.0f), bullet_damage(0.0f),
				shoot_delay(0.0f), max_hp(100.0f), current_hp(100.0f), alive(true),
				projectile_type(ProjectileType::enemy_proj1), texture(0), type(EnemyType::pumpkin), bullet_spread(30),
				last_shot(0), size(32)
{
	player_pos.x = 0;
	player_pos.y = 0;
	player_pos.x = 0;
	player_pos.y = 0;
	player_pos.orientation = 0;
}

// Constructor with all parameters
Enemy::Enemy(float x, float y, float speed, float fps, float bullet_count, float bullet_speed,
			float bullet_duration, float bullet_damage, float shoot_delay, float max_hp,
			float current_hp, bool alive, ProjectileType projectile_type, GLuint texture,
			EnemyType type, float bullet_spread, float size) : x(x), y(y), speed(speed), fps(fps),
																bullet_count(bullet_count),
																bullet_speed(bullet_speed),
																bullet_duration(bullet_duration),
																bullet_damage(bullet_damage),
																shoot_delay(shoot_delay),
																max_hp(max_hp), current_hp(current_hp), alive(alive),
																projectile_type(projectile_type),
																texture(texture), type(type),
																bullet_spread(bullet_spread),
																last_shot(0), size(size)
{
	player_pos.x = 0;
	player_pos.y = 0;
	player_pos.orientation = 0;
}

// Getters
float Enemy::getX() const { return x; }
float Enemy::getY() const { return y; }
float Enemy::getSpeed() const { return speed; }
float Enemy::getFps() const { return fps; }
float Enemy::getBulletCount() const { return bullet_count; }
float Enemy::getBulletSpeed() const { return bullet_speed; }
float Enemy::getBulletDuration() const { return bullet_duration; }
float Enemy::getBulletDamage() const { return bullet_damage; }
float Enemy::getShootDelay() const { return shoot_delay; }
float Enemy::getMaxHp() const { return max_hp; }
float Enemy::getCurrentHp() const { return current_hp; }
bool Enemy::isAlive() const { return alive; }
ProjectileType Enemy::getProjectileType() const { return projectile_type; }
GLuint Enemy::getTexture() const { return texture; }
EnemyType Enemy::getType() const { return type; }

// Setters
void Enemy::setX(float x) { this->x = x; }
void Enemy::setY(float y) { this->y = y; }
void Enemy::setSpeed(float speed) { this->speed = speed; }
void Enemy::setFps(float fps) { this->fps = fps; }
void Enemy::setBulletCount(float bullet_count) { this->bullet_count = bullet_count; }
void Enemy::setBulletSpeed(float bullet_speed) { this->bullet_speed = bullet_speed; }
void Enemy::setBulletDuration(float bullet_duration) { this->bullet_duration = bullet_duration; }
void Enemy::setBulletDamage(float bullet_damage) { this->bullet_damage = bullet_damage; }
void Enemy::setShootDelay(float shoot_delay) { this->shoot_delay = shoot_delay; }
void Enemy::setMaxHp(float max_hp) { this->max_hp = max_hp; }
void Enemy::setCurrentHp(float current_hp) { this->current_hp = current_hp; }
void Enemy::setAlive(bool alive) { this->alive = alive; }
void Enemy::setProjectileType(ProjectileType projectile_type) { this->projectile_type = projectile_type; }
void Enemy::setTexture(GLuint texture) { this->texture = texture; }
void Enemy::setType(EnemyType type) { this->type = type; }

void Enemy::setPlayerPosition(float player_x, float player_y, float player_or)
{
	player_pos.x = player_x;
	player_pos.y = player_y;
	player_pos.orientation = player_or + 90;
}

// Shoot method - Fires projectiles towards target_x and target_y
void Enemy::shoot(Projectiles &prjs)
{
	if (glfwGetTime() > last_shot + shoot_delay && alive)
	{
		last_shot = glfwGetTime();
		float base_orientation = atan2(player_pos.y - y, player_pos.x - x); // Base angle towards the target
		float spread_angle = M_PI / 2.0f; // 180 degrees (half-circle)
		float angle_step = spread_angle / (bullet_count - 1); // Angle between each bullet

		for (int i = 0; i < bullet_count; i++)
		{
			std::cout <<"shooting" << std::endl;
			float current_angle = base_orientation - (spread_angle / 2.0f) + (i * angle_step);
			Projectile new_proj = {
				x, y, current_angle, true, bullet_speed, projectile_type,
				glfwGetTime(), bullet_duration, bullet_damage
			};
			prjs.add_projectile(new_proj);
		}
	}
}


// Draw method - Renders the enemy
void Enemy::draw(GLuint shader_program, GLuint VAO, int screen_width, int screen_height)
{
	if (alive)
	{
		float cos_theta = cos(player_pos.orientation / 180 * M_PI);
		float sin_theta = sin(player_pos.orientation / 180 * M_PI);
		float new_x = (x - player_pos.x) * cos_theta - (y - player_pos.y) * sin_theta;
		float new_y = (x - player_pos.x) * sin_theta + (y - player_pos.y) * cos_theta;
		renderTexture(shader_program, texture, VAO, new_x, new_y,
					180, screen_width, screen_height, size);
	}
}

// Move method - Moves the enemy towards target_x, target_y based on the speed
void Enemy::move(Map map)
{
	if (alive)
	{
		float direction_x = player_pos.x - x;
		float direction_y = player_pos.y - y;
		float length = sqrt(direction_x * direction_x + direction_y * direction_y);

		if (length != 0.0f)
		{
			direction_x /= length; // Normalize
			direction_y /= length;
		}

		// Update position
		x += direction_x * speed / fps;
		y += direction_y * speed / fps;

		// Optionally, check for collision with the map or boundaries
		if (map.is_obstacle(x, y))
		{
			// Handle collision with the map
			// For now, just reverse the movement
			x -= direction_x * speed / fps;
			y -= direction_y * speed / fps;
		}
	}
}

void Enemy::check_for_hit(Projectiles &prjs)
{
	current_hp -= prjs.get_damage(x, y, size, ProjectileType::player_proj);
	if (current_hp < 0)
		alive = false;
}

