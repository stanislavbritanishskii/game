#include "map.hpp"
#include <cstdlib> // For std::rand
#include <ctime>   // For std::time
#include <cmath>
#include <iomanip>
#define TEST_MAP 0


Map::Map(int screen_width, int screen_height, int map_width, int map_height, GLFWwindow *window, GLuint texture, GLuint shader, GLuint VAO)
	: screen_width(screen_width), screen_height(screen_height),
	map_width(map_width), map_height(map_height),
	tile_size(32), // Assuming default tile size
	_x(0.0f), _y(0.0f), orientation(0.0f), shader_program(shader)
{
	textures[TileTypes::normal] = loadTexture(main_map_tile);
	textures[TileTypes::obstacle] = loadTexture(obstacle1_tile);
	textures[TileTypes::obstacle2] = loadTexture(obstacle2_tile);
	textures[TileTypes::marked] = loadTexture(bigX);
	// Initialize terrain with random TileTypes
#if !TEST_MAP
	int font_size = 100, font_width = 100, font_height = 100;
	std::cout << "font size: " << font_width << ", " << font_height << std::endl;
	std::map<char, GLuint> characters = char_textures("DejaVuSans.ttf", font_size, font_width, font_height);
	std::cout << "created textures" << std::endl;
	for (int x = 0; x < screen_width; x++)
	{
		font_size = 100, font_width = 100, font_height = 100;
		std::string text;
		// if (x < 10)
		// text = "0";
		text += std::to_string(x);

		dist_textures.push_back(createTextTexture("DejaVuSans.ttf", text, font_size, font_width, font_height));
	}
	std::srand(std::time(nullptr)); // Seed for random generation
	terrain.resize(map_height);
	distances.resize(map_height);
	std::cout << std::fixed << std::setprecision(1);
	double current_time = glfwGetTime();
	for (int i = 0; i < map_height; ++i)
	{
		if (glfwGetTime() > current_time + 0.1)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
			std::cout <<"\rcreating map "<< ((float)(i) * 100) / (map_height) << "%" << std::flush;
			float percentage = ((float)(i) * 100) / (map_height);

			for (int i = -screen_height / 2; i < screen_height / 2 + 1; i += tile_size)
			{
				for (int j = -screen_width / 2; j < screen_width  / 2 + 1; j+= tile_size)
				{
					renderTexture(shader_program, textures[TileTypes::normal], VAO, i, j, orientation, screen_width, screen_height, tile_size);
				}
			}
			// renderTexture(shader_program, textures[TileTypes::normal], VAO, 0, 0, orientation, screen_width, screen_height, std::min(screen_width, screen_height));

			// renderTexture(shader_program, dist_textures[i], VAO, 0, 0, orientation, screen_width, screen_height, std::min(screen_width, screen_height) / 2);
			renderTexture(shader_program, characters[(int)percentage / 10 + '0'], VAO, -40, 0, orientation, screen_width, screen_height, 40);
			renderTexture(shader_program, characters[(int)percentage % 10 + '0'], VAO, 0, 0, orientation, screen_width, screen_height, 40);
			renderTexture(shader_program, characters['.'], VAO, 26, -18, orientation, screen_width, screen_height, 7);
			renderTexture(shader_program, characters[((int)(percentage * 10) % 10) + '0'], VAO, 50, 0, orientation, screen_width, screen_height, 40);
			renderTexture(shader_program, characters['%'], VAO, 90, 0, orientation, screen_width, screen_height, 40);
			glfwSwapBuffers(window);
			current_time = glfwGetTime();
		}
		terrain[i].resize(map_width);
		distances[i].resize(map_width);
		for (int j = 0; j < map_width; ++j)
		{
			distances[i][j] = map_height * map_width;

//			glfwWindowShouldClose(window);
//			glClear(GL_COLOR_BUFFER_BIT);
//
//			renderTexture(shader, texture, VAO, 0, 0, glfwGetTime(), screen_width, screen_height, 1);
//			glfwSwapBuffers(window);
			// std::cout <<"\rcreating map "<< ((float)(i * map_width + j) * 100) / (map_height * map_width) << "%" << std::flush;
			glfwPollEvents();
			// Randomly assign TileTypes
			if (std::rand() % 3 > 0)
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

	terrain[map_height/2][map_width / 2] = normal;
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
	this->_x = x;
}

float Map::getX()
{
	return _x;
}

void Map::setY(float y)
{
	this->_y = y;
}

float Map::getY()
{
	return _y;
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
	int x_min = std::max((int)(_x / tile_size + map_width / 2 - screen_width / tile_size / 2), 0);
	int x_max = std::min((int)(_x / tile_size + map_width / 2 + screen_width / tile_size / 2), map_width);
	int y_min = std::max((int)(_y / tile_size + map_height / 2 - screen_height / tile_size / 2), 0);
	int y_max = std::min((int)(_y / tile_size + map_height / 2 + screen_height / tile_size / 2), map_height);


	for (int i = y_min; i < y_max; i++)
	{
		for (int j = x_min; j < x_max; j++)
		{
			// Calculate the original tile position relative to the map center
			float original_x = (j - map_width / 2 + 0.5f) * tile_size;
			float original_y = (i - map_height / 2 + 0.5f) * tile_size;

			// Apply the rotation around the center (this->x, this->y)
			float new_x = (original_x - this->_x) * cos_theta - (original_y - this->_y) * sin_theta;
			float new_y = (original_x - this->_x) * sin_theta + (original_y - this->_y) * cos_theta;
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

				if (distances[i][j] < dist_textures.size() && distances[i][j] < 0)
					renderTexture(shader_program, dist_textures[distances[i][j]], VAO, new_x, new_y, orientation - 90, screen_width, screen_height, tile_size / 2);
			}
		}
	}
	for (int i = 0; i < chosen_tiles.size(); ++i)
	{
		float original_x = (chosen_tiles[i].first - map_width / 2 + 0.5f) * tile_size;
		float original_y = (chosen_tiles[i].second - map_height / 2 + 0.5f) * tile_size;

		// Apply the rotation around the center (this->x, this->y)
		float new_x = (original_x - this->_x) * cos_theta - (original_y - this->_y) * sin_theta;
		float new_y = (original_x - this->_x) * sin_theta + (original_y - this->_y) * cos_theta;
		renderTexture(shader_program, textures[marked], VAO, new_x, new_y,
					orientation, screen_width, screen_height, tile_size);
	}
}

bool Map::is_tile_obstacle(int x, int y)
{
	if (y < 0 || x < 0 || y >= map_height || x >= map_width)
		return true;
	return terrain[y][x] != TileTypes::normal;
}

bool Map::is_obstacle(float x, float y)
{
	return is_tile_obstacle(x / tile_size + map_width / 2, y / tile_size + map_height / 2);
}

void Map::addChosenTile(int x, int y)
{
	chosen_tiles.emplace_back(x / tile_size + map_width / 2 - (x < 0),y / tile_size + map_height / 2 -( y < 0));
}


void Map::clearChosenTiles()
{
	chosen_tiles.clear();
}

void Map::calculateBFSDistance(int depth)
{
	// Calculate player's tile position
	static int start_x = 0, start_y = 0;
	int current_x = static_cast<int>(_x / tile_size + map_width / 2);
	int current_y = static_cast<int>(_y / tile_size + map_height / 2);

	if (current_x == start_x && current_y == start_y)
		return;

	start_x = current_x;
	start_y = current_y;

	// Ensure starting position is valid
	if (is_tile_obstacle(start_x, start_y))
	{
		std::cerr << "Player's position is on an obstacle. Cannot calculate distances." << std::endl;
		return;
	}

	// BFS initialization
	std::queue<std::pair<int, int>> queue;
	static std::vector<std::vector<bool>> visited(depth * 2 + 1, std::vector<bool>(depth * 2 + 1, false));
	std::fill(visited.begin(), visited.end(), std::vector<bool>(depth * 2 + 1, false));

	std::vector<std::vector<int>> local_distances(depth * 2 + 1, std::vector<int>(depth * 2 + 1, -1));
	queue.push({depth, depth});
	local_distances[depth][depth] = 0;
	visited[depth][depth] = true;

	// Directions for movement: up, down, left, right
	const int dx[] = {0, 0, -1, 1};
	const int dy[] = {-1, 1, 0, 0};

	// BFS traversal
	while (!queue.empty())
	{
		auto [local_y, local_x] = queue.front();
		queue.pop();
		int current_distance = local_distances[local_y][local_x];
		if (current_distance >= depth)
			continue;

		for (int i = 0; i < 4; ++i)
		{
			int new_local_x = local_x + dx[i];
			int new_local_y = local_y + dy[i];
			if (new_local_x < 0 || new_local_x >= depth * 2 + 1 || new_local_y < 0 || new_local_y >= depth * 2 + 1)
				continue;

			// Check if already visited
			if (visited[new_local_y][new_local_x])
				continue;
			visited[new_local_y][new_local_x] = true;

			// Map to global coordinates
			int global_x = start_x + (new_local_x - depth);
			int global_y = start_y + (new_local_y - depth);

			// Check if global coordinates are valid
			if (global_x >= 0 && global_x < map_width && global_y >= 0 && global_y < map_height && !is_tile_obstacle(global_x, global_y))
			{
				local_distances[new_local_y][new_local_x] = current_distance + 1;
				queue.push({new_local_y, new_local_x});
			}
		}
	}

	// Update global distances array
	for (int i = 0; i < depth * 2 + 1; ++i)
	{
		for (int j = 0; j < depth * 2 + 1; ++j)
		{
			if (local_distances[i][j] != -1)
			{
				int global_x = start_x + (j - depth);
				int global_y = start_y + (i - depth);
				distances[global_y][global_x] = local_distances[i][j];
			}
		}
	}
}


std::pair<int, int> Map::getTile(float x, float y)
{
	return std::pair<int, int>(x / tile_size + map_width / 2, y / tile_size + map_height / 2);
}
int Map::getBFSDistance(int x, int y)
{
	return distances[y][x];
}

;
