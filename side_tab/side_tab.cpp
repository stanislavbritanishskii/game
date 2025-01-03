// side_tab.cpp
#include "side_tab.hpp"
#include <sstream>


// Constructor
SideTab::SideTab(int width, int height, const std::string &fontPath, int fontSize)
	: width(width), height(height), fps(0.0f), player_max_hp(0.0f), player_cur_hp(0.0f),
	player_damage(0.0f), player_speed(0.0f), player_level(0.0f), player_exp(0.0f)
{
	characterTextures = char_textures(fontPath, fontSize, charTextureWidth, charTextureHeight);
	background = loadTexture("textures/grey_background.png");
}

// Destructor
SideTab::~SideTab()
{
	for (auto &pair: characterTextures)
	{
		glDeleteTextures(1, &pair.second);
	}
}

// Get player parameters and update the side tab
void SideTab::getPlayerParams(Player &player)
{
	player_max_hp = player.getMaxHp();
	player_cur_hp = player.getCurHP();
	player_damage = player.getBulletDamage(); // Assuming damage is linked to bullet count
	player_speed = player.getVelocity();
	player_level = player.getLevel();
	player_exp = player.getXp();
	player_accuracy = player.getAccuracy();
	shots_per_second = 1 / player.getShootDelay();
}

// Update FPS
void SideTab::updateFps(float fps)
{
	this->fps = fps;
}

// Render the side tab
void SideTab::render(unsigned int shaderProgram, unsigned int VAO)
{
	// Fill the side tab with a grey background
	renderTexture(shaderProgram, background, VAO, 0, 0, 0, width, height, 1000);

	int y_offset = height / 2 - 20; // Start rendering from the top of the side tab

	// Render FPS
	int x_offset = -width / 2 + 15;
	renderTextValue("FPS ", fps, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	// Render Player Parameters
	renderTextValue("MAX HP ", player_max_hp, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	renderTextValue("CUR HP ", player_cur_hp, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	renderTextValue("DMG ", player_damage, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	renderTextValue("SPD ", player_speed, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;


	renderTextValue("ACC ", player_accuracy, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	renderTextValue("SPS ", shots_per_second, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	renderTextValue("LVL ", player_level, x_offset, y_offset, shaderProgram, VAO);
	y_offset -= 30;

	renderTextValue("EXP ", player_exp, x_offset, y_offset, shaderProgram, VAO);

}


// Render a text label and value using textures
void SideTab::renderTextValue(const std::string &label, int value, int x, int y, unsigned int shaderProgram,
							unsigned int VAO)
{
	std::stringstream ss;
	ss << label;

	int offset_x = x;
	int char_scale = 10;
	int counter  = 0;
	for (char c: ss.str())
	{
		counter ++;
		if (characterTextures.find(c) != characterTextures.end())
		{
			renderTexture(shaderProgram, characterTextures[c], VAO, offset_x, y, 0.0f, width, height, char_scale);
			offset_x += char_scale;
		}
	}
	ss.flush();
	ss << value;
	offset_x += char_scale;
	for (char c: ss.str())
	{
		counter --;
		if (counter > 0)
			continue;
		if (characterTextures.find(c) != characterTextures.end())
		{
			renderTexture(shaderProgram, characterTextures[c], VAO, offset_x, y, 0.0f, width, height, char_scale);
			offset_x += char_scale;
		}
	}
}
