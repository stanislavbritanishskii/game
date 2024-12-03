#include "hp_bar.hpp"
#include <stdexcept>
#include <iostream> // For debugging (optional)

// Constructor
HPBar::HPBar(float width, float height, float bar_height)
	: totalWidth(width), totalHeight(height), maxHP(100), currentHP(100),
	  contourTexture(0), redTexture(0), bar_height(bar_height) {
	updateRedTexture();
	updateBlackContour();
}

// Destructor
HPBar::~HPBar() {
	if (contourTexture != 0) glDeleteTextures(1, &contourTexture);
	if (redTexture != 0) glDeleteTextures(1, &redTexture);
}

// Setter for max HP
void HPBar::setMaxHP(int max) {
	if (max <= 0) throw std::invalid_argument("Max HP must be greater than 0.");
	maxHP = max;

	// Recreate the red texture to match new max HP
	updateRedTexture();
}

// Setter for current HP
void HPBar::setCurrentHP(int hp) {
	if (hp < 0) hp = 0;
	if (hp > maxHP) hp = maxHP;
	currentHP = hp;

	// Update the red texture scaling
	updateRedTexture();
}

// Getter for contour texture
GLuint HPBar::getContourTexture() const {
	return contourTexture;
}

// Getter for red texture
GLuint HPBar::getRedTexture() const {
	return redTexture;
}


// Update the red rectangle texture
void HPBar::updateRedTexture() {
	// Delete the old texture if it exists
	if (redTexture != 0) glDeleteTextures(1, &redTexture);

	// Generate a new texture
	glGenTextures(1, &redTexture);
	glBindTexture(GL_TEXTURE_2D, redTexture);

	// Calculate the width of the red rectangle based on current HP
	int redWidth = static_cast<int>((static_cast<float>(currentHP) / maxHP) * totalWidth);

	// Create a pixel buffer for the red rectangle
	std::vector<unsigned char> textureData(static_cast<size_t>(totalWidth * totalHeight * 4), 0);
	int index;
	for (int y = totalHeight - 4; y >= totalHeight - bar_height - 4; --y) {
		// std::cout << y << std::endl;

		for (int x = totalWidth; x > totalWidth - redWidth; --x) {
			index = (y * static_cast<int>(totalWidth) + x) * 4;
			textureData[index] = 255;      // Red
			textureData[index + 1] = 0;   // Green
			textureData[index + 2] = 0;   // Blue
			textureData[index + 3] = 255; // Alpha
		}

	}
	// Upload the pixel buffer to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, totalWidth, totalHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

	// Configure texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}
void HPBar::updateBlackContour() {
	// Delete the old texture if it exists
	if (contourTexture != 0) glDeleteTextures(1, &contourTexture);

	// Generate a new texture
	glGenTextures(1, &contourTexture);
	glBindTexture(GL_TEXTURE_2D, contourTexture);

	std::vector<unsigned char> textureData(static_cast<size_t>(totalWidth * totalHeight * 4), 0);
	int index;
	for (int y = totalHeight - 4; y > 0; --y) {
		if (y < totalHeight - bar_height - 4)
		{
			for (int x = 0; x < totalWidth; ++x)
			{
				index = (y * static_cast<int>(totalWidth)) * 4;
				textureData[index + 3] = 0; // Alpha
			}
		}
		else
		{
			for (int x = totalWidth; x > 0; --x) {
				index = (y * static_cast<int>(totalWidth) + x) * 4;
				textureData[index] = 0;      // Red
				textureData[index + 1] = 0;   // Green
				textureData[index + 2] = 0;   // Blue
				textureData[index + 3] = 255; // Alpha
			}
		}
	}

	// Upload the pixel buffer to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, totalWidth, totalHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

	// Configure texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, contourTexture);
}


