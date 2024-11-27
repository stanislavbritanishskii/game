#include "utils.hpp"
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

GLuint createTextTexture(const std::string &fontPath, const std::string &text, int fontSize, int &textureWidth, int &textureHeight) {
	// Initialize FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "ERROR::FREETYPE: Could not initialize FreeType Library" << std::endl;
		return 0;
	}

	// Load font
	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		FT_Done_FreeType(ft);
		return 0;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// Calculate the texture size
	int maxHeight = 0, totalWidth = 0;
	for (const char &c : text) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load glyph for '" << c << "'" << std::endl;
			continue;
		}
		maxHeight = std::max(static_cast<int>(maxHeight), static_cast<int>(face->glyph->bitmap.rows));
		totalWidth += face->glyph->bitmap.width + face->glyph->bitmap_left;
	}

	textureWidth = totalWidth;
	textureHeight = maxHeight;

	// Create a buffer to store the text bitmap (with alpha channel)
	// We now need a RGBA texture, so each pixel will have 4 components (R, G, B, A)
	unsigned char *bitmap = new unsigned char[textureWidth * textureHeight * 4](); // Initialize to 0 (transparent)
	int offsetX = 0;

	for (const char &c : text) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load glyph for '" << c << "'" << std::endl;
			continue;
		}
		FT_Bitmap &glyphBitmap = face->glyph->bitmap;

		// Copy glyph bitmap to our texture buffer (only for the red channel)
		for (int row = 0; row < glyphBitmap.rows; ++row) {
			for (int col = 0; col < glyphBitmap.width; ++col) {
				int x = offsetX + col;
				int y = maxHeight - row - 1; // Invert Y-axis

				// Set red channel to black (0) for text
				bitmap[(y * textureWidth + x) * 4] = 0;   // Red channel (black text)
				bitmap[(y * textureWidth + x) * 4 + 1] = 0; // Green channel (no color)
				bitmap[(y * textureWidth + x) * 4 + 2] = 0; // Blue channel (no color)

				// Set the alpha channel based on the glyph bitmap intensity (text is solid black)
				bitmap[(y * textureWidth + x) * 4 + 3] = glyphBitmap.buffer[row * glyphBitmap.pitch + col]; // Alpha (text color)
			}
		}

		offsetX += glyphBitmap.width + face->glyph->bitmap_left;
	}

	// Generate OpenGL texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Cleanup
	delete[] bitmap;
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return texture;
}
