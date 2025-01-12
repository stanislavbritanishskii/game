// side_tab.cpp
#include "side_tab.hpp"
#include <sstream>


// Constructor
SideTab::SideTab(int width, int height, const std::string &fontPath, int fontSize)
	: width(width), height(height), fps(0.0f), player_max_hp(0.0f), player_cur_hp(0.0f),
	player_damage(0.0f), player_speed(0.0f), player_level(0.0f), player_exp(0.0f), char_scale(8), button_pressed(false)
{
	characterTextures = char_textures(fontPath, fontSize, charTextureWidth, charTextureHeight);
	background = loadTexture("textures/gui_textures/grey_background.png");
	pressed_button = loadTexture("textures/gui_textures/Button2_11.png");
	unpressed_button = loadTexture("textures/gui_textures/Button2_01.png");
	plus_sign = loadTexture("textures/gui_textures/Icon_01.png");

	int y_offset = height / 2 - 20;
	int x_offset = -width / 2 + 5;
	int button_size = 15;

	entries.insert_or_assign(LEVEL, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false),
												{"LEVEL"}, 0, false));
	y_offset -= 30;
	entries.insert_or_assign(EXP, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false),
											{"EXP"}, 0, false));
	y_offset -= 30;
	entries.insert_or_assign(UPGRADE_POINTS, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false),
										{"UPGRADE","POINTS"}, 0, false));
	y_offset -= 30;
	entries.insert_or_assign(HP, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false),
											{"HP"}, 0));
	y_offset -= 30;
	entries.insert_or_assign(DAMAGE, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false),
												{"DAMAGE"}, 0));
	y_offset -= 30;
	entries.insert_or_assign(SPEED, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false),
												{"SPEED"}, 0));
	y_offset -= 30;
	entries.insert_or_assign(ACCURACY, entry_data(x_offset, y_offset,
												button_data(button_size, x_offset, y_offset, false), {"ACCURACY"}, 0));
	y_offset -= 30;
	entries.insert_or_assign(SHOTS_PER_SECOND, entry_data(x_offset, y_offset,
														button_data(button_size, x_offset, y_offset, false),
														{"SHOTS", "PER", "SECOND"}, 0));
	y_offset = -height / 2 + 20;
	entries.insert_or_assign(
			FPS, entry_data(x_offset, y_offset, button_data(button_size, x_offset, y_offset, false), {"FPS"}, 0, false, 4));

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
	entries[HP].value = player_max_hp;
	player_cur_hp = player.getCurHP();
	player_damage = player.getBulletDamage();
	entries[DAMAGE].value = player_damage;
	player_speed = player.getVelocity();
	entries[SPEED].value = player_speed;
	player_level = player.getLevel();
	entries[LEVEL].value = player_level;
	player_exp = player.getXp();
	entries[EXP].value = player_exp;
	player_accuracy = player.getAccuracy();
	entries[ACCURACY].value = player_accuracy;
	shots_per_second = 1 / player.getShootDelay();
	entries[SHOTS_PER_SECOND].value = shots_per_second;
	upgrade_points = player.get_upgrade_points();
	entries[UPGRADE_POINTS].value = upgrade_points;
}

// Update FPS
void SideTab::updateFps(float fps)
{
	this->fps = fps;
	entries[FPS].value = fps;
}

// Render the side tab
void SideTab::render(unsigned int shaderProgram, unsigned int VAO)
{
	// Fill the side tab with a grey background
	renderTexture(shaderProgram, background, VAO, 0, 0, 0, width, height, 1000);

	for (auto entry: entries)
	{
		render_entry(entry.second, shaderProgram, VAO);
	}
}


// Render a text label and value using textures
void SideTab::renderTextValue(const std::string &label, int value, int x, int y, unsigned int shaderProgram,
							unsigned int VAO)
{
	std::stringstream ss;
	ss << label;

	int offset_x = x;
	int char_scale = 10;
	int counter = 0;
	for (char c: ss.str())
	{
		counter++;
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
		counter--;
		if (counter > 0)
			continue;
		if (characterTextures.find(c) != characterTextures.end())
		{
			renderTexture(shaderProgram, characterTextures[c], VAO, offset_x, y, 0.0f, width, height, char_scale);
			offset_x += char_scale;
		}
	}
}


void SideTab::render_button(button_data button, int shaderProgram, unsigned int VAO)
{

	if (button.pressed)
	{
		renderTexture(shaderProgram, pressed_button, VAO, button.x, button.y, 0, width, height, button.size);
	} else
	{
		renderTexture(shaderProgram, unpressed_button, VAO, button.x, button.y, 0, width, height, button.size);
	}
	renderTexture(shaderProgram, plus_sign, VAO, button.x, button.y, 0, width, height, button.size);
}

int SideTab::render_word(const std::string &label, int x, int y, unsigned int shaderProgram, unsigned int VAO, int char_scale_=0)
{
	std::stringstream ss;
	ss << label;

	int offset_x = x;
	int counter = 0;
	if (char_scale_ == 0)
		char_scale_ = char_scale;
	for (char c: ss.str())
	{
		counter++;
		if (characterTextures.find(c) != characterTextures.end())
		{
			renderTexture(shaderProgram, characterTextures[c], VAO, offset_x, y, 0.0f, width, height, char_scale_);
			offset_x += char_scale_;
		}
	}
	return counter * char_scale_;
}

int SideTab::render_value(float value, int x, int y, unsigned int shaderProgram, unsigned int VAO, int char_scale_=0)
{
	if (char_scale_ == 0)
		char_scale_ = char_scale;
	std::stringstream ss;
	int counter = 0;
	int offset_x = x;
	ss << value;
	for (char c: ss.str())
	{
		counter++;
		if (characterTextures.find(c) != characterTextures.end())
		{
			renderTexture(shaderProgram, characterTextures[c], VAO, offset_x, y, 0.0f, width, height, char_scale_);
			offset_x += char_scale_;
		}
	}
	return counter * char_scale_;
}

void SideTab::render_entry(entry_data_t entry, int shaderProgram, unsigned int VAO)
{
	int x_offset = entry.x;
	if (entry.clickable)
	{
		std::cout << entry.button.pressed << std::endl;
		render_button(entry.button, shaderProgram, VAO);
		x_offset += entry.button.size + char_scale;
	}
	for (auto word: entry.words)
	{
		x_offset += render_word(word, x_offset, entry.y, shaderProgram, VAO, entry.char_scale) + entry.char_scale;
	}
	render_value(entry.value, x_offset, entry.y, shaderProgram, VAO, entry.char_scale);
}


void SideTab::click(int x, int y, Player *player)
{
	if (!button_pressed)
	{
		button_pressed = true;

		for (auto &entry: entries)
		{
			if (x > entry.second.button.x - entry.second.button.size / 2 && x < entry.second.button.x + entry.second.
				button.size / 2 && y > entry.second.button.y - entry.second.button.size / 2 && y < entry.second.button.y
				+ entry.second.button.size / 2)
			{
				entry.second.button.pressed =  true;
				switch (entry.first)
				{
					case HP:

						player->upgrade_hp();
					break;
					case SPEED:

						player->upgrade_velocity();
					break;
					case DAMAGE:

						player->upgrade_damage();
					break;
					case ACCURACY:
						player->upgrade_accuracy();
					break;
					case SHOTS_PER_SECOND:
						player->upgrade_shots_per_second();
					break;
				}
			}
		}
	}
}


void SideTab::unclick()
{
	button_pressed = false;
	for (auto &entry: entries)
	{
		std::cout << entry.second.button.pressed << std::endl;
		entry.second.button.pressed = false;
	}
}
