#include "utils.hpp"

bool is_player_projectile(ProjectileType type)
    {
  return type == ProjectileType::player_proj;
  }

bool is_enemy_projectile(ProjectileType type)
      {
  return type == ProjectileType::pumpkin_proj;
  }
