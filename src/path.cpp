#pragma once

#include <vector>
#include <cstdio>
#include "random.cpp"

using std::vector;

/// @brief Pair of Coordinates
struct Coords2D
{
	int x;
	int y;
};

/// @brief Types of cells
enum CELL_TYPES
{
	WALL = '#',
	PATH = ' ',
	TARGET = '.',
};

/// @brief Checks if an obstacle is present
/// @param obstacles obstacles to be checked
/// @param coords coords to be checked
/// @return true if there is, otherwise false
bool IsObstaclePresent(Coords2D coords, vector<Coords2D> obstacles)
{
	for (Coords2D obstacle : obstacles)
		if (coords.x == obstacle.x && coords.y == obstacle.y)
			return true;

	return false;
}

/// @brief Finds the direction to go from `from` to `to`
/// @param from The start position
/// @param to The end position
/// @return The direction as Coords2D struct
Coords2D FindDirection(Coords2D from, Coords2D to)
{
	Coords2D increase;
	// `from` has to go left
	if (from.x > to.x)
		increase.x = -1;

	// `from` has to go right
	if (from.x < to.x)
		increase.x = 1;

	// `from` has not to move horizontally
	if (from.x == to.x)
		increase.x = 0;

	// `from` has to go up
	if (from.y > to.y)
		increase.y = -1;

	// `from` has to go down
	if (from.y < to.y)
		increase.y = 1;

	// `from` has not to move vertically
	if (from.y == to.y)
		increase.y = 0;

	return increase;
}

/// @brief Optimize a given path removing useless ways
/// @param path The path to be optimized
void OptimizePath(vector<Coords2D> &path)
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
}

/// @brief Finds a random valid path between two coordinates avoiding obstacles
/// @param from Point to start from
/// @param to Point of arrival
/// @param gridSize Grid dimension
/// @param level Matrix with the walls and paths
/// @param obstacles The obstacles to avoid
/// @return A Coords2D vector with all the points of the path
vector<Coords2D> FindRandomPath(Coords2D from, Coords2D to, Coords2D gridSize, vector<vector<CELL_TYPES>> level, vector<Coords2D> obstacles = {})
{
	vector<Coords2D> path;
	Coords2D next, increase = FindDirection(from, to);

	path.push_back(from);

	while (from.x != to.x || from.y != to.y)
	{
		next.x = from.x;
		next.y = from.y;
		// if (!prevBox &&
		// 	(level[from.y][from.x + increase.x] == PATH || level[from.y][from.x + increase.x] == TARGET) &&
		// 	!IsObstaclePresent(Coords2D(from.x + increase.x, from.y), obstacles))
		// {
		// 	printf("\nA\n");

		// 	from.x += increase.x;
		// }

		// else if (!prevBox &&
		// 		 (level[from.y + increase.y][from.x] == PATH || level[from.y + increase.y][from.x] == TARGET) &&
		// 		 !IsObstaclePresent(Coords2D(from.x, from.y + increase.y), obstacles))
		// {
		// 	printf("\nB\n");

		// 	from.y += increase.y;
		// }

		if ((random(0, 1) && from.x != to.x) || from.y == to.y)
		{
			next.x += increase.x;
			// Check if the randomically selected cell is not occupied by an obstacle
			if (IsObstaclePresent(next, obstacles))
			{
				// Randomically go top or bottom if possible (1 / -1)
				int randomDirection = (random(0, 1) ? 1 : -1);

				from.y += (from.y + randomDirection > 0 && from.y + randomDirection < gridSize.y - 1)
							  ? randomDirection : -randomDirection;

				path.push_back(from);
				vector<Coords2D> newPath = FindRandomPath(from, to, gridSize, level, obstacles);
				path.insert(path.end(), newPath.begin(), newPath.end());
				return path;
			}

			from.x += increase.x;
		}

		else
		{
			next.y += increase.y;
			// Check if the randomically selected cell is not occupied by an obstacle
			if (IsObstaclePresent(next, obstacles))
			{
				// Randomically go top or bottom if possible (1 / -1)
				int randomDirection = (random(0, 1) ? 1 : -1);

				from.x += (from.x + randomDirection > 0 && from.x + randomDirection < gridSize.x - 1)
							  ? randomDirection : -randomDirection;

				path.push_back(from);
				vector<Coords2D> newPath = FindRandomPath(from, to, gridSize, level, obstacles);
				path.insert(path.end(), newPath.begin(), newPath.end());
				return path;
			}

			from.y += increase.y;
		}
		
		path.push_back(from);
	}

	return path;
}