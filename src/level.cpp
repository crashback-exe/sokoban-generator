#pragma once

#include <iostream>
#include <string>
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

/// @brief Types of cells
enum CELL_TYPES
{
	PLAYER = '@',
	WALL = '#',
	PATH = ' ',
	BOX = '$',
	TARGET = '.',
};

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
		Coords2D placementCell;
		bool isOccupied;

		for (int i = 0; i < boxCount; i++)
		{
			// Target
			do
			{
				isOccupied = false;

				placementCell.x = random(0, width - 1);
				placementCell.y = random(0, height - 1);

				if (level[placementCell.y][placementCell.x] != WALL ||
					(playerPos.x == placementCell.x && playerPos.y == placementCell.y))
				{
					isOccupied = true;
					continue;
				}

				for (Coords2D box : boxesPos)
				{
					if (box.x == placementCell.x && box.y == placementCell.y)
					{
						isOccupied = true;
						break;
					}
				}
			} while (isOccupied);

			targetPos[i].x = placementCell.x;
			targetPos[i].y = placementCell.y;
			level[targetPos[i].y][targetPos[i].x] = TARGET;

			// Box
			do
			{
				isOccupied = false;

				placementCell.x = random(1, width - 2);
				placementCell.y = random(1, height - 2);

				if (level[placementCell.y][placementCell.x] != WALL ||
					(playerPos.x == placementCell.x && playerPos.y == placementCell.y))
				{
					isOccupied = true;
					continue;
				}

				for (Coords2D box : boxesPos)
				{
					if (box.x == placementCell.x && box.y == placementCell.y)
					{
						isOccupied = true;
						break;
					}
				}
			} while (isOccupied);

			boxesPos[i].x = placementCell.x;
			boxesPos[i].y = placementCell.y;
		}
	}

	/// @brief Generate paths in the level
	void GeneratePaths()
	{
		vector<Coords2D> paths, path, tempPath;

		Coords2D direction;
		Coords2D gridSize(width, height),
			boxGridSize(width - 1, height - 1);

		/// @brief Optimize a given path removing useless ways
		/// @param path The path to be optimized
		auto OptimizePath = [](vector<Coords2D> &path)
		{
			int pathSize = (path.size() - 2);

			auto ii = path.begin();
			while (ii != path.end())
			{
				auto ij = ii + 1;
				while (ij != path.end())
				{
					if (ii->x == ij->x && ii->y == ij->y)
					{
						path.erase(ii, ij);
						ij = ii;
					}

					ij++;
				}
				ii++;
			}
		};

		// Generate the paths between box and his target
		for (int i = 0; i < boxCount; i++)
		{
			// Find a path between the box and his corresponding target
			path = FindRandomPath(boxesPos[i], targetPos[i], boxGridSize, boxesPos);

			// Optimize the path to get rid of useless cells
			OptimizePath(path);

			// Insert the generated path into the to-be-carved cell list
			paths.insert(paths.end(), path.begin(), path.end());

			int size = path.size();

			// Carve the path needed for the player to move the box
			for (int j = 0; j < size - 1; j++)
			{
				/**
				 * For every coordinate in the path, find the opposite direction the player should
				 * go to get to the next coordinate and find a path to it to make the cell reachable
				 */

				// Calculate the opposite coordinate of the coordinate pair
				Coords2D oppositeCoord;
				oppositeCoord.x = path[j].x - FindDirection(path[j], path[j + 1]).x;
				oppositeCoord.y = path[j].y - FindDirection(path[j], path[j + 1]).y;

				// If it's the first iteration, calculate the path between the player and the first cell
				if (j == 0)
				{

					// Find the path
					tempPath = FindRandomPath(playerPos, oppositeCoord, gridSize, vector<Coords2D>{boxesPos[i]});

					// Insert the path
					paths.insert(paths.end(), tempPath.begin(), tempPath.end());

					// Skip this iteration
					continue;
				}

				// Find the path between the cell and the cell before
				tempPath = FindRandomPath(path[j - 1], oppositeCoord, gridSize, vector<Coords2D>{path[j]});

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

		// Randomically place player in the level
		PlacePlayer();
		GenerateGoals();
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
		if (level[to.y][to.x] == WALL || to.x >= width || to.y >= height || to.x < 0 || to.y < 0)
			return;

		// Check if the position corresponds to any of the boxes
		for (Coords2D &boxPos : boxesPos)
		{
			// If corresponds, check if the positon behind that box is free
			if (to.x == boxPos.x && to.y == boxPos.y)
			{

				boxTo.x = to.x + STEPS[direction].x;
				boxTo.y = to.y + STEPS[direction].y;

				// Check if the cell after the box is free
				if ((boxTo.x < width && boxTo.y < height && boxTo.x >= 0 && boxTo.y >= 0) &&
					(level[boxTo.y][boxTo.x] == PATH || level[boxTo.y][boxTo.x] == TARGET))
				{
					bool isBoxBehind = false;
					// Check if behind the box is another box
					for (Coords2D everyOtherBoxPos : boxesPos)
					{
						if (everyOtherBoxPos.x == boxTo.x && everyOtherBoxPos.y == boxTo.y)
							isBoxBehind = true;
					}

					if (isBoxBehind)
						return;

					boxPos.x = boxTo.x;
					boxPos.y = boxTo.y;

					playerPos.x = to.x;
					playerPos.y = to.y;

					return;
				}

				return;
			}
		}

		playerPos.x = to.x;
		playerPos.y = to.y;
	}

	/// @brief Print on standard output the level
	void Show()
	{
		Coords2D pos;
		bool printed;

		cout << " X ";
		for (int i = 0; i < width; i++)
			cout << i << " ";
		cout << endl
			 << "Y"
			 << endl;

		for (pos.y = 0; pos.y < height; pos.y++)
		{
			cout << pos.y << "  ";
			for (pos.x = 0; pos.x < width; pos.x++)
			{
				printed = false;

				if (pos.x == playerPos.x && pos.y == playerPos.y)
				{
					cout << (char)PLAYER << " ";
					continue;
				}

				// For every box/target pair
				for (int i = 0; i < boxCount; i++)
				{
					if (pos.x == boxesPos[i].x && pos.y == boxesPos[i].y)
					{
						cout << ((char)(i + 'A')) << " "; // BOXES
						printed = true;
						break;
					}
					else if (pos.x == targetPos[i].x && pos.y == targetPos[i].y)
					{
						cout << ((char)(i + '1')) << " "; // TARGETS
						printed = true;
						break;
					}
				}

				if (!printed)
					cout << (char)level[pos.y][pos.x] << " ";
			}
			cout << endl;
		}
	}
};