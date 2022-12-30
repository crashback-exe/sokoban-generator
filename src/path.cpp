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
	/* Iterate over all the obstacles, if one of them has the same coordinates as the ones given, return false*/
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
	/* Iterate over the path and if a coordinate occurs more than once, remove the coordinates between the two*/
	int pathSize = (path.size() - 2);

	auto it_y = path.begin();
	while (it_y != path.end())
	{
		auto it_x = it_y + 1;
		while (it_x != path.end())
		{
			if (it_y->x == it_x->x && it_y->y == it_x->y)
			{
				path.erase(it_y, it_x);
				it_x = it_y;
			}

			it_x++;
		}
		it_y++;
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
	/* Until we're not at the end, keep finding a random cell towards the end, if a box is found, dodge it and continue
	   searching paths skipping the cell where we were stuck */
	/* Nore: we prefer path that are already carved */

	static bool previouslyDodgedBox = false;

	vector<Coords2D> path;
	Coords2D next, increase = FindDirection(from, to);

	path.push_back(from);

	// While we're not at the end
	while (from.x != to.x || from.y != to.y)
	{
		// If X is already carved, prefer it
		if (from.x + increase.x >= 0 && from.x + increase.x < gridSize.x			// Inside the grid
			&& !previouslyDodgedBox													// Hasn't passed a box previously
			&& level[from.y][from.x + increase.x] != WALL							// Not a wall
			&& !IsObstaclePresent(Coords2D(from.x + increase.x, from.y), obstacles) // Not a box
			&& from.x != to.x)
			from.x += increase.x;
		// If Y is already carved, prefer it
		else if (from.y + increase.y >= 0 && from.y + increase.y < gridSize.y			 // Inside the grid
				 && !previouslyDodgedBox												 // Hasn't passed a box previously
				 && level[from.y + increase.y][from.x] != WALL							 // Not a wall
				 && !IsObstaclePresent(Coords2D(from.x, from.y + increase.y), obstacles) // Not a box
				 && from.y != to.y)
			from.y += increase.y;
		/* If X and Y are not carved, choose randomly between X and Y */

		// X is chosen
		else if ((random(0, 1) && from.x != to.x) || from.y == to.y)
		{

			// Check if the randomically selected cell is not occupied by an obstacle
			if (IsObstaclePresent(Coords2D(from.x + increase.x, from.y), obstacles))
			{
				previouslyDodgedBox = true;

				// Randomically go top or bottom if possible (1 / -1)
				int randomDirection = (random(0, 1) ? 1 : -1);

				from.y += (from.y + randomDirection > 0 && from.y + randomDirection < gridSize.y - 1)
							  ? randomDirection
							  : -randomDirection;

				// Push the chosen coordinate in the path
				path.push_back(from);

				// Find a new path from the cell to the end and return it
				vector<Coords2D> restOfPath = FindRandomPath(from, to, gridSize, level, obstacles);
				path.insert(path.end(), restOfPath.begin(), restOfPath.end());
				return path;
			}
			previouslyDodgedBox = false;

			from.x += increase.x;
		}

		// Y is chosen
		else
		{

			// Check if the randomically selected cell is not occupied by an obstacle
			if (IsObstaclePresent(Coords2D(from.x, from.y + increase.y), obstacles))
			{
				previouslyDodgedBox = true;

				// Randomically go top or bottom if possible (1 / -1)
				int randomDirection = (random(0, 1) ? 1 : -1);

				from.x += (from.x + randomDirection > 0 && from.x + randomDirection < gridSize.x - 1)
							  ? randomDirection
							  : -randomDirection;

				// Push the chosen coordinate in the path
				path.push_back(from);

				// Find a new path from the cell to the end and return it
				vector<Coords2D> restOfPath = FindRandomPath(from, to, gridSize, level, obstacles);
				path.insert(path.end(), restOfPath.begin(), restOfPath.end());

				return path;
			}
			previouslyDodgedBox = false;

			from.y += increase.y;
		}

		// Push the chosen coordinate in the path
		path.push_back(from);
	}

	return path;
}