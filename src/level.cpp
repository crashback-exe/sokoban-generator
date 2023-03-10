#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <map>
#include "random.cpp"
#include "path.cpp"

using std::cout;
using std::endl;
using std::invalid_argument;
using std::map;
using std::to_string;
using std::vector;

/// @brief List of possible directions
enum DIRECTIONS
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

/// @brief List of possible player steps
map<DIRECTIONS, Coords2D> STEPS = {
	{UP, Coords2D(0, -1)},
	{DOWN, Coords2D(0, 1)},
	{LEFT, Coords2D(-1, 0)},
	{RIGHT, Coords2D(1, 0)},
};

class SokobanLevel
{
private:
	const char PLAYER = '@';
	const char BOX = '$';
	const char TARGET = '.';

	Coords2D playerPos;
	vector<Coords2D> boxesPos;
	vector<Coords2D> targetPos;

	/// @brief The level without boxes targets and the player
	vector<vector<CELL_TYPES>> baseLevel;

	/// @brief The complete generated level
	vector<vector<CELL_TYPES>> level;

	int height;
	int width;
	int boxCount;

	/// @brief Generate the player in a random position in the level
	void PlacePlayer()
	{
		playerPos.x = random(0, width - 1);
		playerPos.y = random(0, height - 1);
	}

	/// @brief Generate boxes and targets in the level
	void GenerateGoals()
	{
		vector<Coords2D> obstacles;
		obstacles.push_back(playerPos);

		Coords2D placementCell;

		for (int i = 0; i < boxCount; i++)
		{
			// Target
			for (;;)
			{
				placementCell.x = random(0, width - 1);
				placementCell.y = random(0, height - 1);

				if (!IsPresent(placementCell, obstacles))
					break;
			}

			targetPos[i].x = placementCell.x;
			targetPos[i].y = placementCell.y;
			obstacles.push_back(targetPos[i]);

			// Box
			for (;;)
			{
				placementCell.x = random(1, width - 2);
				placementCell.y = random(1, height - 2);

				if (!IsPresent(placementCell, obstacles))
					break;
			}

			boxesPos[i].x = placementCell.x;
			boxesPos[i].y = placementCell.y;
			obstacles.push_back(boxesPos[i]);
		}
	}

	/// @brief Generate paths in the level
	void GeneratePaths()
	{
		vector<Coords2D> paths, path, tempPath;

		Coords2D direction;
		Coords2D gridSize(width, height);

		// Generate the paths between box and his target
		for (int i = 0; i < boxCount; i++)
		{
			// Find a path between the box and his corresponding target
			path = FindRandomPath(boxesPos[i], targetPos[i], gridSize, level, boxesPos);

			// Optimize the path to get rid of useless cells
			OptimizePath(path);

			// Insert the generated path into the to-be-carved cell list
			paths.insert(paths.end(), path.begin(), path.end());

			int elementCount = path.size() - 1;

			// Carve the path needed for the player to move the box
			for (int j = 0; j < elementCount; j++)
			{
				/**
				 * For every coordinate in the path, find the opposite direction the player should
				 * go to get to the next coordinate and find a path to it to make the cell reachable
				 */

				// Calculate the opposite coordinate of the coordinate pair
				Coords2D direction = FindDirection(path[j], path[j + 1]);
				Coords2D oppositeCoord;
				oppositeCoord.x = path[j].x - direction.x;
				oppositeCoord.y = path[j].y - direction.y;

				// If it's the first iteration, calculate the path between the player and the first cell
				if (j == 0)
				{
					vector<Coords2D> obstacles = boxesPos;

					// If the first cell to move the box is occupied itself by a box, don't consider the box that
					// occupied that cell as an obstacle
					//printf("%c\n", (char)i+'A');
					
					//printf("Path:\n");
					//for (Coords2D coord : path) printf("\tx: %d y: %d\n", coord.x, coord.y);

					//printf("OppositeCoord: %d %d\n", oppositeCoord.x, oppositeCoord.y);
					for (int k = 0; k < boxCount; k++)
					{
						if (boxesPos[k].x == oppositeCoord.x && boxesPos[k].y == oppositeCoord.y)
						{
							//printf("Box: %d %d\n", boxesPos[k].x, boxesPos[k].y);
							obstacles.erase(obstacles.begin() + k);
						}
					}

					// for (Coords2D obstacle : obstacles)
					// {
					// 	printf("Ostacolo: %d %d\n", obstacle.x, obstacle.y);
					// }
					
					// Find the path
					tempPath = FindRandomPath(playerPos, oppositeCoord, gridSize, level, obstacles);
					
					//printf("FindRandomPath done.\n");
					// Insert the path
					paths.insert(paths.end(), tempPath.begin(), tempPath.end());

					// Skip this iteration
					continue;
				}

				// Find the path between the cell and the cell before
				tempPath = FindRandomPath(path[j - 1], oppositeCoord, gridSize, level, vector<Coords2D>{path[j]});

				// Optimize the path
				OptimizePath(tempPath);

				// Insert the path
				paths.insert(paths.end(), tempPath.begin(), tempPath.end());
			}
		}

		// Carve the paths
		for (Coords2D cell : paths)
		{
			level[cell.y][cell.x] = PATH;
		}
	}

	void Deallocate()
	{
		boxesPos.clear();
		targetPos.clear();
		level.clear();
	}

public:
	/// @brief Create a Sokoban level with given width and height
	/// @param _height Level height
	/// @param _width Level width
	SokobanLevel(int _height, int _width, int _boxCount)
	{
		if ((_height - 2) * (_width - 2) < _boxCount)
			throw invalid_argument("Level " + to_string(_height) + "x" + to_string(_width) +
								   " too small for " + to_string(_boxCount) + " boxes");

		height = _height;
		width = _width;
		boxCount = _boxCount;
	}

	~SokobanLevel()
	{
		Deallocate();
	}

	/// @brief Generate the board
	void Generate()
	{
		Deallocate();

		level.resize(height, vector<CELL_TYPES>(width, WALL));
		boxesPos.resize(boxCount, Coords2D(-1, -1));
		targetPos.resize(boxCount, Coords2D(-1, -1));

		// Place the player in the level
		PlacePlayer();
		// Generate the boxes and the targets
		GenerateGoals();
		// Generate the paths
		GeneratePaths();
	}

	/// @brief Moves the player in the given direction
	/// @param direction The direction where the player moves
	/// @return true if the player has moved, false if it hasn't
	void PlayerMove(DIRECTIONS direction)
	{
		// To is the cell we want to go
		Coords2D to, boxTo;
		to.x = playerPos.x + STEPS[direction].x;
		to.y = playerPos.y + STEPS[direction].y;

		// Check if the position corresponds to a wall
		if ((to.x >= width || to.y >= height || to.x < 0 || to.y < 0) || level[to.y][to.x] == WALL)
			return;

		// Check if the position corresponds to any of the boxes
		for (Coords2D &boxPos : boxesPos)
		{
			// If corresponds, check if the positon behind that box is free
			if (to.x != boxPos.x || to.y != boxPos.y)
				continue;

			boxTo.x = to.x + STEPS[direction].x;
			boxTo.y = to.y + STEPS[direction].y;

			// Check if the cell after the box is free
			if ((boxTo.x < width && boxTo.y < height && boxTo.x >= 0 && boxTo.y >= 0) && level[boxTo.y][boxTo.x] != WALL && !IsPresent(boxTo, boxesPos))
			{
				boxPos.x = boxTo.x;
				boxPos.y = boxTo.y;

				playerPos.x = to.x;
				playerPos.y = to.y;

				return;
			}

			return;
		}

		playerPos.x = to.x;
		playerPos.y = to.y;
	}

	/// @brief Print on standard output the level
	void Show()
	{
		Coords2D pos;

		for (pos.y = 0; pos.y < height; pos.y++)
		{
			cout << "  ";
			for (pos.x = 0; pos.x < width; pos.x++)
			{
				if (IsPresent(pos, vector<Coords2D>{playerPos}))
					cout << PLAYER << " ";

				else if (IsPresent(pos, boxesPos))
				{
					cout <<	BOX << " ";
				}

				else if (IsPresent(pos, targetPos))
				{
					cout << TARGET << " ";
				}

				else
					cout << (char)level[pos.y][pos.x] << " ";
			}
			cout << endl;
		}
	}

	void ShowJson()
	{
		cout << "{\"level\":[";
		for (vector<CELL_TYPES> line : level)
		{
			cout << "[";
			for (CELL_TYPES cell : line)
			{
				cout << "\"" << (char)cell << "\",";
			}
			cout << "\b],";
		}
		cout << "\b],";
		cout << "\"player\":{\"x\":" << playerPos.x << ",\"y\":" << playerPos.y << "},";
		cout << "\"box\":[";
		for (Coords2D boxPos : boxesPos)
		{
			cout << "[" << boxPos.x << "," << boxPos.y << "],";
		}
		cout << "\b],";
		cout << "\"target\":[";
		for (Coords2D pos : targetPos)
		{
			cout << "[" << pos.x << "," << pos.y << "],";
		}
		cout << "\b]";

		cout << "}" << endl;
	}
};