#ifndef PROJECTILE_READER_HPP
#define PROJECTILE_READER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp> // Include nlohmann/json library for JSON handling
#include "utils.hpp"

// EnemyData structure


// ConfigReader class
class ProjectileReader {
private:
	std::map<ProjectileType, ProjectileData> projectile_data;

	// Convert string to ProjectileType
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

		throw std::invalid_argument("Unknown ProjectileType: " + str);
	}

public:
	// Load configuration from JSON
	void loadFromJSON(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + filename);
		}

		nlohmann::json json_data;
		file >> json_data;

		for (auto& [key, value] : json_data.items()) {
			try {
				ProjectileType type = stringToProjectileType(key);
				ProjectileData data;

				// Validate and assign "textures_path"
				if (value.contains("textures_path") && value["textures_path"].is_string()) {
					data.texture_path = value["textures_path"].get<std::string>();
				} else {
					throw std::runtime_error("Invalid or missing 'textures_path' for " + key);
				}

				// Validate and assign "texture_rotation"
				if (value.contains("texture_rotation") && value["texture_rotation"].is_number_float()) {
					data.rotation = value["texture_rotation"].get<float>();
				} else {
					throw std::runtime_error("Invalid or missing 'texture_rotation' for " + key);
				}

				data.projectile_type = type;
				projectile_data[type] = data;
			} catch (const std::exception& e) {
				std::cerr << "Error parsing projectile data for key: " << key << ", error: " << e.what() << std::endl;
			}
		}
	}


	// Get projectile data
	std::map<ProjectileType, ProjectileData> get_data()
	{
		return projectile_data;
	}
};

#endif // PROJECTILE_READER_HPP
