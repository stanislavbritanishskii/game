#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp> // Include nlohmann/json library for JSON handling
#include "utils.hpp"

// EnemyData structure
struct EnemyData {
	float speed;
	float health;
	float damage;
	int bullet_count;
	float spread;
	float size;
	float active_distance;
	float bullet_speed;
	float bullet_duration;
	float shoot_delay;
	std::string texture_name;
};

// PlayerData structure
struct PlayerData {
	float shoot_delay;
	float teleport_delay;
	float velocity;
	float rotation_speed;
	float size;
	float bullet_speed;
	float bullet_duration;
	int bullet_count;
	float nova_delay;
	int hit_box;
	std::string texture_name;
};

// ConfigReader class
class ConfigReader {
private:
	std::unordered_map<EnemyType, EnemyData> enemies_data;
	PlayerData player_data;
	float ratio;

	EnemyType stringToEnemyType(const std::string& str) {
		if (str == "Pumpkin") return EnemyType::pumpkin;
		// Add additional mappings here
		throw std::invalid_argument("Unknown EnemyType: " + str);
	}

public:
	// Load configuration from JSON
	void loadFromJSON(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + filename);
		}

		nlohmann::json json_data;
		file >> json_data;

		// Load RATIO
		ratio = json_data.contains("RATIO") ? json_data["RATIO"].get<float>() : 1.0f;

		// Load enemy data
		for (const auto& [key, value] : json_data["enemies"].items()) {
			EnemyType type = stringToEnemyType(key);
			EnemyData data;
			data.speed = value["speed"].get<float>() * ratio;
			data.health = value["health"].get<float>();
			data.damage = value["damage"].get<float>();
			data.bullet_count = value["bullet_count"].get<int>();
			data.spread = value["spread"].get<float>();
			data.size = value["size"].get<float>() * ratio;
			data.active_distance = value["active_distance"].get<float>() * ratio;
			data.bullet_speed = value["bullet_speed"].get<float>() * ratio;
			data.bullet_duration = value["bullet_duration"].get<float>();
			data.shoot_delay = value["shoot_delay"].get<float>();
			data.texture_name = value.contains("texture_name") ? value["texture_name"].get<std::string>() : "default";
			enemies_data[type] = data;
		}

		// Load player data
		if (json_data.contains("player")) {
			auto& player_json = json_data["player"];
			player_data.shoot_delay = player_json["shoot_delay"].get<float>();
			player_data.velocity = player_json["velocity"].get<float>() * ratio;
			player_data.rotation_speed = player_json["rotation_speed"].get<float>();
			player_data.size = player_json["size"].get<float>() * ratio;
			player_data.bullet_speed = player_json["velocity"].get<float>() * ratio;
			player_data.bullet_duration = player_json["teleport_delay"].get<float>();
			player_data.bullet_count = player_json["bullet_count"].get<int>();
			player_data.teleport_delay = player_json["teleport_delay"].get<float>();
			player_data.nova_delay = player_json["nova_delay"].get<float>();
			player_data.hit_box = player_json["hit_box"].get<int>();
			player_data.texture_name = player_json.contains("texture_name") ? player_json["texture_name"].get<std::string>() : "player_default";
		} else {
			throw std::runtime_error("Missing 'player' data in configuration file");
		}
	}

	// Get enemy data by EnemyType
	const EnemyData& getEnemyData(EnemyType type) const {
		return enemies_data.at(type);
	}

	// Get player data
	const PlayerData& getPlayerData() const {
		return player_data;
	}

	// Get ratio
	float getRatio() const {
		return ratio;
	}
};

#endif // CONFIG_READER_HPP
