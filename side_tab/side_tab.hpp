// side_tab.hpp
#ifndef SIDE_TAB_HPP
#define SIDE_TAB_HPP

#include <map>
#include <string>
#include "player.hpp"

typedef struct button_data {
	int size;
	int x;
	int y;
	bool pressed;

	button_data(size_t size, int x, int y, bool pressed): size(size), x(x), y(y),
														pressed(pressed)
	{
	}
} button_data_t;

enum params {
	FPS,
	HP,
	DAMAGE,
	SPEED,
	ACCURACY,
	SHOTS_PER_SECOND,
	LEVEL,
	EXP,
	UPGRADE_POINTS
};

typedef struct entry_data {
	int x, y;
	button_data_t button;
	std::vector<std::string> words;
	float value;
	bool clickable;
	int char_scale;

	entry_data(): x(0), y(0), button(0, 0, 0, false), value(0), clickable(false), char_scale(1)
	{
	}

	entry_data(int x, int y, button_data_t button, std::vector<std::string> words, float value,
				bool clickable = true, int char_scale=10): x(x), y(y),
										button(button),
										words(words),
										value(value), clickable(clickable), char_scale(char_scale)
	{
	}
} entry_data_t;

class SideTab {
private:
	GLuint background;
	int width;
	int height;
	std::map<char, GLuint> characterTextures;
	std::map<params, entry_data_t> entries;
	GLuint unpressed_button;
	GLuint pressed_button;
	GLuint plus_sign;
	int charTextureWidth;
	int charTextureHeight;
	int char_scale;
	bool button_pressed;
	float player_max_hp;
	float player_cur_hp;
	float player_damage;
	float player_speed;
	float player_level;
	float player_exp;
	float fps;
	float shots_per_second;
	float player_accuracy;
	float upgrade_points;

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

	void render_entry(entry_data_t entry, int shaderProgram, unsigned int VAO);

	int render_word(const std::string &label, int x, int y, unsigned int shaderProgram,
					unsigned int VAO, int char_scale_);

	int render_value(float value, int x, int y, unsigned int shaderProgram,
					unsigned int VAO, int char_scale_);

	void render_button(button_data button, int shaderProgram,
						unsigned int VAO);

	void renderTextValue(const std::string &label, int value, int x, int y, unsigned int shaderProgram,
						unsigned int VAO);

	void click(int x, int y, Player *player);

	void unclick();
};
#endif
