// side_tab.hpp
#ifndef SIDE_TAB_HPP
#define SIDE_TAB_HPP

#include <map>
#include <string>
#include "player.hpp"

class SideTab {
private:
	GLuint background;
	int width;
	int height;
	std::map<char, GLuint> characterTextures;
	int charTextureWidth;
	int charTextureHeight;
	float player_max_hp;
	float player_cur_hp;
	float player_damage;
	float player_speed;
	float player_level;
	float player_exp;
	float fps;
	float player_accuracy;

public:
	// Constructor
	SideTab(int width, int height, const std::string &fontPath, int fontSize);

	// Destructor
	~SideTab();

	// Get player parameters and update the side tab
	void getPlayerParams(Player &player);

	// Update FPS
	void updateFps(float fps);

	// Render the side tab
	void render(unsigned int shaderProgram, unsigned int VAO);
	void renderTextValue(const std::string &label, int value, int x, int y, unsigned int shaderProgram,
							unsigned int VAO);
};
#endif