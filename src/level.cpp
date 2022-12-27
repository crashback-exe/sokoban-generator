#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
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
		Coords2D placementCell;

		do
		{
			placementCell.x = random(0, width - 1);
			placementCell.y = random(0, height - 1);

			if (level[placementCell.y][placementCell.x] == WALL)
			{
				playerPos.x = placementCell.x;
				playerPos.y = placementCell.y;
				break;
			}
		} while (level[placementCell.y][placementCell.x] != WALL);
	}

	/// @brief Generate boxes and targets in the level
	void GenerateGoals()
	{
		Coords2D placementCell;
		bool isOccupied;

		for (int i = 0; i < boxCount; i++)
		{
			// Box
			do
			{
				isOccupied = false;

				placementCell.x = random(1, width - 2);
				placementCell.y = random(1, height - 2);

				for (Coords2D box : boxesPos)
				{
					if (box.x == placementCell.x && box.y == placementCell.y)
					{
						isOccupied = true;
						break;
					}
				}

				if (!isOccupied)
				{
					boxesPos[i].x = placementCell.x;
					boxesPos[i].y = placementCell.y;
					break;
				}
			} while (isOccupied);

			// Target
			do
			{
				isOccupied = false;
				
				placementCell.x = random(0, width - 1);
				placementCell.y = random(0, height - 1);

				for (Coords2D box : boxesPos)
				{
					if (box.x == placementCell.x && box.y == placementCell.y)
					{
						isOccupied = true;
						break;
					}
				}

				if (level[placementCell.y][placementCell.x] == WALL && !isOccupied)
				{
					targetPos[i].x = placementCell.x;
					targetPos[i].y = placementCell.y;
					level[targetPos[i].y][targetPos[i].x] = TARGET;
					break;
				}
			} while (level[placementCell.y][placementCell.x] != WALL && isOccupied);
		}
	}

	/// @brief Generate paths in the level
	void GeneratePaths()
	{
		// Generate the paths between box and his target
		vector<Coords2D> paths, path, tempBoxesPos;

		Coords2D direction;

		for (int i = 0; i < boxCount; i++)
		{
			tempBoxesPos = boxesPos;

			path = FindRandomPath(boxesPos[i], targetPos[i], Coords2D(width, height), boxesPos);
			paths.insert(paths.end(), path.begin(), path.end());

			for (Coords2D &box : tempBoxesPos)
			{
				if (box.x == boxesPos[i].x && box.y == boxesPos[i].y)
				{
					box.x = -1;
					box.y = -1;
				}
			}

			path = FindRandomPath(playerPos, boxesPos[i], Coords2D(width, height), tempBoxesPos);
			paths.insert(paths.end(), path.begin(), path.end());
		}

		for (Coords2D cell : paths)
			level[cell.y][cell.x] = PATH;
	}

public:
	/// @brief Create a Sokoban level with given width and height
	/// @param _height Level height
	/// @param _width Level width
	SokobanLevel(int _height, int _width, int _boxCount)
	{
		if ((_height - 2) * (_width - 2) < _boxCount)
			throw invalid_argument("Level " + to_string(_height) + "x" + to_string(_width) + " too small for " + to_string(_boxCount) + " boxes");

		height = _height;
		width = _width;
		boxCount = _boxCount;

		level.resize(height, vector<CELL_TYPES>(width, WALL));

		boxesPos.resize(boxCount, Coords2D(-1, -1));
		targetPos.resize(boxCount, Coords2D(-1, -1));
	}

	/// @brief Generate the board
	void Generate()
	{
		// Randomically place player in the level
		PlacePlayer();
		GenerateGoals();

		/* Debug */
		cout << "/*     */" << endl;
		Show();
		cout << "/*     */" << endl
			 << endl;
		/* Debug */

		GeneratePaths();
	}

	/// @brief Check if the player can move from a cell to a cell
	/// @param from The starting cell
	/// @param to The target cell
	/// @return true if the player can move to the cell, otherwise false
	bool CanMoveTo(Coords2D from, DIRECTIONS direction)
	{
		Coords2D to = {from.x + STEPS[direction].x, from.y + STEPS[direction].y};

		if (to.x < width && to.y < height && to.x >= 0 && to.y >= 0 &&
			(level[to.y][to.x] == PATH || level[to.y][to.x] == TARGET))
			return true;

		if (level[to.y][to.x] == WALL)
			return false;

		// todo
		// non possiamo usare level[][]
		if (level[to.y][to.x] == BOX)
		{
			if (CanMoveTo(to, direction))
				return true;
			return false;
		}

		cout << "QUESTA SCRITTA NON DEVE MAI APPARIRE" << endl;
		return false;
	}

	/// @brief Moves the player in the given direction
	/// @param direction The direction where the player moves
	/// @return true if the player has moved, false if it hasn't
	bool PlayerMove(DIRECTIONS direction)
	{
		if (!CanMoveTo(playerPos, direction))
			return false;

		playerPos.x += STEPS[direction].x;
		playerPos.y += STEPS[direction].y;
		return true;
	}

	/// @brief Print on standard output the level
	void Show()
	{
		Coords2D pos;
		bool printed;

		cout << "   ";
		for (int i = 0; i < width; i++)
			cout << i << " ";
		cout << endl
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