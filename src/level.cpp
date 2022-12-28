#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <iterator>
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
		// Generate the paths between box and his target
		vector<Coords2D> paths, path, tempPath;

		Coords2D direction;
		Coords2D gridSize(width, height);

		auto OptimizePath = [](vector<Coords2D> path)
		{
			// auto slice = [](vector<Coords2D> &vect, int start, int end)
			// {
			// 	int k = end - start + 1;

			// 	auto it = vect.cbegin() + start;
			// 	while (it != vect.cend() && k--)
			// 	{
			// 		it = vect.erase(it);
			// 	}
			// };

			int pathSize = (path.size() - 2);

			auto i = path.begin();
			while (i != path.end())
			{
				auto j = i + 1;
				while (j != path.end())
				{
					if (i->x == j->x && i->y == j->y)
					{
						cout << "\n\npath[" << (i - path.begin()) << "].x: " << i->x << " path[" << (i - path.begin()) << "].y: " << i->y << endl;
						cout << "path[" << (j - path.begin()) << "].x: " << j->x << " path[" << (j - path.begin()) << "].y: " << j->y << endl;
						cout << "pre-palle " << (j - path.begin()) << endl;

						cout << "mid-palle " << (j - path.begin()) << endl;

						path.erase(i, j);

						cout << "post-palle " << (j - path.begin()) << endl;
					}

					j++;
				}
				i++;
			}

			// int pathSize = (path.size() - 2);
			// for (int i = 0; i < pathSize; i++)
			// {
			// 	if (i == 0)
			// 		continue;

			// 	for (int j = i + 1; j < pathSize; j++)
			// 	{
			// 		if (path[i].x == path[j].x && path[i].y == path[j].y)
			// 		{
			// 			cout << "\n\npath[" << i << "].x: " << path[i].x << " path[" << i << "].y: " << path[i].y << endl;
			// 			cout << "path[" << j << "].x: " << path[j].x << " path[" << j << "].y: " << path[j].y << endl;
			// 			cout << "pre-palle " << j << endl;

			// 			auto first = path.cbegin() + i;
			// 			auto last = path.cbegin() + j;

			// 			cout << "mid-palle " << j << endl;

			// 			cout << "first: x " << first->x << " y " << first->y << endl;
			// 			cout << "last: x " << last->x << " y " << last->y << endl;
			// 			path.erase(first, last);

			// 			cout << "post-palle " << j << endl;
			// 		}
			// 	}
			// }
		};

		for (int i = 0; i < boxCount; i++)
		{
			path = FindRandomPath(boxesPos[i], targetPos[i], gridSize, boxesPos);

			// OptimizePath(path);

			/***/
			int pathSize = (path.size() - 2);

			auto ii = path.begin();
			while (ii != path.end())
			{
				auto ij = ii + 1;
				while (ij != path.end())
				{
					if (ii->x == ij->x && ii->y == ij->y)
					{
						cout << "\n\npath[" << (ii - path.begin()) << "].x: " << ii->x << " path[" << (ii - path.begin()) << "].y: " << ii->y << endl;
						cout << "path[" << (ij - path.begin()) << "].x: " << ij->x << " path[" << (ij - path.begin()) << "].y: " << ij->y << endl;
						cout << "pre-palle " << (ij - path.begin()) << endl;

						cout << "mid-palle " << (ij - path.begin()) << endl;

						path.erase(ii, ij);

						cout << "post-palle " << (ij - path.begin()) << endl;
					}

					ij++;
				}
				ii++;
			}

			/***/

			int size = path.size() - 2;
			int k = 0;
			// printf("PathSize %d\nPath %d %d\n", size + 2, path[0].x - FindDirection(path[0], path[1]).x, path[0].y - FindDirection(path[0], path[1]).y);
			printf("\n\nPath:\n");
			for (Coords2D coord : path)
			{
				printf("%d = x: %d y: %d\n", k, coord.x, coord.y);
				k++;
			}
			// Coords2D oppositeCoord;
			// for (int j = 0; j < size; j++)
			// {
			// 	oppositeCoord.x = path[j].x - FindDirection(path[j], path[j + 1]).x;
			// 	oppositeCoord.y = path[j].y - FindDirection(path[j], path[j + 1]).y;

			// 	if (j == 0)
			// 		continue;

			// 	printf("path[j] %d %d\n", path[j].x, path[j].y);
			// 	printf("path[j + 1] %d %d\n", path[j + 1].x, path[j + 1].y);
			// 	printf("oppositeCoord %d %d\n", oppositeCoord.x, oppositeCoord.y);
			// 	printf("path[j - 1] %d %d\n\n", path[j - 1].x, path[j - 1].y);

			// 	tempPath = FindRandomPath(path[j - 1], oppositeCoord, gridSize, vector<Coords2D>{path[j]});
			// 	path.insert(path.end(), tempPath.begin(), tempPath.end());
			// }

			// Debug("end");

			paths.insert(paths.end(), path.begin(), path.end());

			path = FindRandomPath(playerPos,
								  Coords2D(path[0].x - FindDirection(path[0], path[1]).x,
										   path[0].y - FindDirection(path[0], path[1]).y),
								  gridSize, boxesPos);

			// OptimizePath(path);

			/***/
			pathSize = (path.size() - 2);

			ii = path.begin();
			while (ii != path.end())
			{
				auto ij = ii + 1;
				while (ij != path.end())
				{
					if (ii->x == ij->x && ii->y == ij->y)
					{
						cout << "\n\npath[" << (ii - path.begin()) << "].x: " << ii->x << " path[" << (ii - path.begin()) << "].y: " << ii->y << endl;
						cout << "path[" << (ij - path.begin()) << "].x: " << ij->x << " path[" << (ij - path.begin()) << "].y: " << ij->y << endl;
						cout << "pre-palle " << (ij - path.begin()) << endl;

						cout << "mid-palle " << (ij-path.begin()) << endl;

						path.erase(ii, ij);

						cout << "post-palle " << (ij-path.begin()) << endl;
					}

					ij++;
				}
				ii++;
			}

			/***/

			paths.insert(paths.end(), path.begin(), path.end());
		}

		//? Optimize the paths
		// todo

		// Carve the paths
		for (Coords2D cell : paths)
		{
			level[cell.y][cell.x] = PATH;
		}
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

		cout << " Y ";
		for (int i = 0; i < width; i++)
			cout << i << " ";
		cout << endl
			 << "X"
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