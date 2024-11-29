#ifndef HP_BAR_HPP
#define HP_BAR_HPP

#include <GL/glew.h>
#include <vector>
#include "utils.hpp"
class HPBar {
private:
	// Dimensions of the HP bar
	float totalWidth;
	float totalHeight;
	float bar_height;

	// Health tracking
	int maxHP;
	int currentHP;

	// OpenGL textures
	GLuint contourTexture;  // Black rectangle contour
	GLuint redTexture;      // Red inner rectangle representing current HP

	// Internal helper functions
	void updateRedTexture();
	void updateBlackContour();

public:
	// Constructor and destructor
	HPBar(float width, float height, float bar_height);
	~HPBar();

	// Setters for HP values
	void setMaxHP(int max);
	void setCurrentHP(int hp);

	// Getters for the textures
	GLuint getContourTexture() const;
	GLuint getRedTexture() const;
};

#endif // HP_BAR_HPP
