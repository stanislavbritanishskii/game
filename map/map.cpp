#include "map.hpp"
#include <cstdlib> // For std::rand
#include <ctime>   // For std::time
#include <cmath>
#define TEST_MAP 0


Map::Map(int screen_width, int screen_height, int map_width, int map_height)
	: screen_width(screen_width), screen_height(screen_height),
	map_width(map_width), map_height(map_height),
	tile_size(32), // Assuming default tile size
	x(0.0f), y(0.0f), orientation(0.0f)
{
	textures[TileTypes::normal] = loadTexture(main_map_tile);
	textures[TileTypes::obstacle] = loadTexture(obstacle1_tile);
	textures[TileTypes::obstacle2] = loadTexture(obstacle2_tile);
	textures[TileTypes::marked] = loadTexture(bigX);
	// Initialize terrain with random TileTypes
#if !TEST_MAP

	std::srand(std::time(nullptr)); // Seed for random generation
	terrain.resize(map_height);
	for (int i = 0; i < map_height; ++i)
	{
		terrain[i].resize(map_width);
		for (int j = 0; j < map_width; ++j)
		{
			// Randomly assign TileTypes
			if (std::rand() % 20 > 0)
				terrain[i][j] = normal;
			else
			{
				if (std::rand() % 2)
				terrain[i][j] = obstacle;
				else
					terrain[i][j] = obstacle2;
			}
			// terrain[i][j] = (std::rand() % 5 == 0) ? obstacle : normal; // 1/3 chance of obstacle
		}
	}
#elif TEST_MAP == 1
	for (int i = 0; i < map_height; ++i)
	{
		terrain[i].resize(map_width);
		for (int j = 0; j < map_width; ++j)
		{
			// Randomly assign TileTypes
			if (i > map_height / 2 && j > map_width / 2 || i < map_height / 2 && j < map_width / 2)
				terrain[i][j] = obstacle; // 1/3 chance of obstacle
			else
				terrain[i][j] = normal;
		}
	}
#endif
	for (int i =0; i < map_height; ++i)
	{
		terrain[i][0] = obstacle;
		terrain[i][map_width - 1] = obstacle;
	}
	for (int j =0; j < map_width; ++j)
	{
		terrain[0][j] = obstacle;
		terrain[map_height - 1][j] = obstacle;
	}
	terrain[map_width / 2][map_height/2] = normal;
}

Map::~Map()
{
	// Destructor
}

void Map::setTile(int x, int y, TileTypes type)
{
	// Set a specific tile in the map
	if (x >= 0 && x < map_width && y >= 0 && y < map_height)
	{
		terrain[y][x] = type;
	}
}

void Map::setTileSize(int size)
{
	tile_size = size;
}


TileTypes Map::getTile(int x, int y)
{
	// Get the type of a specific tile
	if (x >= 0 && x < map_width && y >= 0 && y < map_height)
	{
		return terrain[y][x];
	}
	return normal; // Default return if out of bounds
}

void Map::setX(float x)
{
	this->x = x;
}

float Map::getX()
{
	return x;
}

void Map::setY(float y)
{
	this->y = y;
}

float Map::getY()
{
	return y;
}

void Map::setOrientation(float orientation)
{
	this->orientation = orientation + 90;
}

float Map::getOrientation()
{
	return orientation;
}

void Map::setMapSize(int map_width, int map_height)
{
	// Set the size of the map
	this->map_width = map_width;
	this->map_height = map_height;
}

int Map::getMapWidth()
{
	return map_width;
}

int Map::getMapHeight()
{
	return map_height;
}

void Map::setMap(std::vector<std::vector<TileTypes> > map)
{
	// Set the entire map terrain
	terrain = map;
}


void Map::drawMap(GLuint shader_program, GLuint VAO)
{
	// Pre-calculate cos and sin for the given orientation (in radians)
	float cos_theta = cos(orientation / 180 * M_PI);
	float sin_theta = sin(orientation / 180 * M_PI);
	int x_min = std::max((int)(x / tile_size + map_width / 2 - screen_width / tile_size / 2), 0);
	int x_max = std::min((int)(x / tile_size + map_width / 2 + screen_width / tile_size / 2), map_width);
	int y_min = std::max((int)(y / tile_size + map_height / 2 - screen_height / tile_size / 2), 0);
	int y_max = std::min((int)(y / tile_size + map_height / 2 + screen_height / tile_size / 2), map_height);


	for (int i = y_min; i < y_max; i++)
	{
		for (int j = x_min; j < x_max; j++)
		{
			// Calculate the original tile position relative to the map center
			float original_x = (j - map_width / 2 + 0.5f) * tile_size;
			float original_y = (i - map_height / 2 + 0.5f) * tile_size;

			// Apply the rotation around the center (this->x, this->y)
			float new_x = (original_x - this->x) * cos_theta - (original_y - this->y) * sin_theta;
			float new_y = (original_x - this->x) * sin_theta + (original_y - this->y) * cos_theta;
			if (new_x * new_x + new_y * new_y < screen_width * screen_height / 4)
			// if (new_x < screen_width / 2 && new_y < screen_height / 2 && new_x > -screen_width / 2 && new_y > -screen_height / 2)

			{
				// Render the normal tile
				renderTexture(shader_program, textures[TileTypes::normal], VAO, new_x, new_y,
							orientation, screen_width, screen_height, tile_size);

				// If the current tile is an obstacle, render the obstacle texture

				if (terrain[i][j] != TileTypes::normal)
				{
					renderTexture(shader_program, textures[terrain[i][j]], VAO, new_x, new_y,
								orientation, screen_width, screen_height, tile_size);
				}

			}
		}
	}
	for (int i = 0; i < chosen_tiles.size(); ++i)
	{
		float original_x = (chosen_tiles[i].first - map_width / 2 + 0.5f) * tile_size;
		float original_y = (chosen_tiles[i].second - map_height / 2 + 0.5f) * tile_size;

		// Apply the rotation around the center (this->x, this->y)
		float new_x = (original_x - this->x) * cos_theta - (original_y - this->y) * sin_theta;
		float new_y = (original_x - this->x) * sin_theta + (original_y - this->y) * cos_theta;
		renderTexture(shader_program, textures[marked], VAO, new_x, new_y,
					orientation, screen_width, screen_height, tile_size);
	}
}

bool Map::is_tile_obstacle(int x, int y)
{
	if (y < 0 || x < 0 || y >= map_height || x >= map_width)
		return true;
	return terrain[y][x] == TileTypes::obstacle || terrain[y][x] == TileTypes::obstacle2;
}

bool Map::is_obstacle(float x, float y)
{
	return is_tile_obstacle(x / tile_size + map_width / 2, y / tile_size + map_height / 2);
}

void Map::addChosenTile(int x, int y)
{
	chosen_tiles.emplace_back(x / tile_size + map_width / 2,y / tile_size + map_height / 2);
}


void Map::clearChosenTiles()
{
	chosen_tiles.clear();
}
