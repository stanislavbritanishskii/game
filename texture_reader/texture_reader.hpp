#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <fstream>
#include "utils.hpp"

class TextureManager {
private:
	std::map<std::string, std::vector<std::string>> textures;

	std::string directionToString(Direction direction) const {
		switch (direction) {
			case up: return "up";
			case down: return "down";
			case left: return "left";
			case right: return "right";
			case still_up: return "still_up";
			case still_down: return "still_down";
			case still_left: return "still_left";
			case still_right: return "still_right";
			default: throw std::invalid_argument("Invalid direction");
		}
	}

public:
	void loadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open())
			throw std::runtime_error("Could not open file: " + filename);

		nlohmann::json jsonData;
		file >> jsonData;

		for (const auto& [key, value] : jsonData.items()) {
			if (!value.is_array())
				throw std::runtime_error("Invalid JSON format for key: " + key);
			textures[key] = value.get<std::vector<std::string>>();
		}
	}

	std::vector<std::string> getTextures(Direction direction) const {
		std::string dirKey = directionToString(direction);
		auto it = textures.find(dirKey);
		if (it != textures.end()) {
			return it->second;
		} else {
			throw std::runtime_error("Textures for direction '" + dirKey + "' not found");
		}
	}
};
