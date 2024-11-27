#ifndef MAP_HPP
#define MAP_HPP
#include <vector>
#include <map>
#include "utils.hpp"
#include <queue>
enum TileTypes {
	normal,
	obstacle,
	obstacle2,
	marked
};



class Map {
private:
	int screen_width, screen_height;
	int map_width, map_height;
	int tile_size;
	float _x;
	float _y;
	float orientation;
	std::vector<std::vector<TileTypes> > terrain;
	std::vector<std::vector<int> > distances;
	std::vector<GLuint> dist_textures;
	std::map<TileTypes, GLuint> textures;
	GLuint shader_program;
	std::vector<std::pair<int, int>> chosen_tiles;
public:
	Map(int screen_width, int screen_height, int map_width, int map_height, GLFWwindow *window, GLuint texture, GLuint shader, GLuint VAO);

	~Map();

	void setTile(int x, int y, TileTypes type);
	void setTileSize(int size);

	TileTypes getTile(int x, int y);

	void setX(float x);

	float getX();

	void setY(float y);

	float getY();

	void setOrientation(float orientation);

	float getOrientation();

	void setMapSize(int map_width, int map_height);

	int getMapWidth();

	int getMapHeight();

	void setMap(std::vector<std::vector<TileTypes> > map);
	void drawMap(GLuint shader_program, GLuint VAO);
	bool is_tile_obstacle(int x, int y);
	bool is_obstacle(float x, float y);

	void addChosenTile(int x, int y);

	void clearChosenTiles();
	void calculateBFSDistance(int depth);
	int getBFSDistance(int x, int y);
	std::pair<int, int> getTile(float x, float y);

};


#endif
