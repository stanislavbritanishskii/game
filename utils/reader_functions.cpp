#include "utils.hpp"

EnemyType stringToEnemyType(const std::string& str) {
		if (str == "Pumpkin") return EnemyType::pumpkin;
		if (str == "Slime1") return EnemyType::Slime1;
		if (str == "Slime2") return EnemyType::Slime2;
		if (str == "Slime3") return EnemyType::Slime3;
		if (str == "Slime4") return EnemyType::Slime4;
		if (str == "Orc") return EnemyType::Orc;
		if (str == "Orc1") return EnemyType::Orc1;
		if (str == "Orc2") return EnemyType::Orc2;
		if (str == "Orc3") return EnemyType::Orc3;
		if (str == "Wolf") return EnemyType::Wolf;
		if (str == "Bee") return EnemyType::Bee;

		// Add additional mappings here
		throw std::invalid_argument("Unknown EnemyType: " + str);
	}
ProjectileType stringToProjectileType(const std::string& str)
	{
		if (str == "player_proj") return ProjectileType::player_proj;
		if (str == "pumpkin_proj") return ProjectileType::pumpkin_proj;
		if (str == "Slime1_proj") return ProjectileType::Slime1_proj;
		if (str == "Slime2_proj") return ProjectileType::Slime2_proj;
		if (str == "Slime3_proj") return ProjectileType::Slime3_proj;
		if (str == "Slime4_proj") return ProjectileType::Slime4_proj;
		if (str == "Wolf_proj") return ProjectileType::Wolf_proj;
		if (str == "Bee_proj") return ProjectileType::Bee_proj;
		if (str == "Orc_proj") return ProjectileType::Orc_proj;
		if (str == "Orc1_proj") return ProjectileType::Orc1_proj;
		if (str == "Orc2_proj") return ProjectileType::Orc2_proj;
		if (str == "Orc3_proj") return ProjectileType::Orc3_proj;

		throw std::invalid_argument("Unknown ProjectileType: " + str);
	}