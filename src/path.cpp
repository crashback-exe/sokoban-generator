#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include "random.cpp"

using std::cout;
using std::vector;

/// @brief Pair of Coordinates
struct Coords2D
{
	int x;
	int y;
};

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

/// @brief Finds a random valid path between two coordinates avoiding obstacles
/// @param from Point to start from
/// @param to Point of arrival
/// @param obstacles The obstacles to avoid
/// @return A Coords2D vector with all the points of the path
vector<Coords2D> FindRandomPath(Coords2D from, Coords2D to, Coords2D gridSize, vector<Coords2D> obstacles = {})
{
	vector<Coords2D> path;
	Coords2D increase = FindDirection(from, to);

	path.push_back(Coords2D(from.x, from.y));

	while (from.x != to.x || from.y != to.y)
	{
		// from and to are in a vertical line
		if (from.y == to.y)
		{
			// Check if the randomically selected cell is not occupied by an obstacle
			for (Coords2D obstacle : obstacles)
			{
				if ((from.x + increase.x) == obstacle.x && from.y == obstacle.y)
				{
					// Randomically go top or bottom if possible (1 / -1)
					//? lo scelgo random solo se posso
					int randomDirection = (random(0, 1) ? 1 : -1);

					from.y += ((from.y + randomDirection) > 0 && (from.y + randomDirection) < gridSize.y)
								  ? randomDirection
								  : -randomDirection;

					path.push_back(Coords2D(from.x, from.y));
					vector<Coords2D> newPath = FindRandomPath(from, to, gridSize, obstacles);
					path.insert(path.end(), newPath.begin(), newPath.end());
					return path;
				}
			}

			from.x += increase.x;
		}

		// from and to are in an horizontal line
		else if (from.x == to.x)
		{
			// Check if the randomically selected cell is not occupied by an obstacle
			for (Coords2D obstacle : obstacles)
			{

				if (from.x == obstacle.x && (from.y + increase.y) == obstacle.y)
				{
					// Randomically go top or bottom if possible (1 / -1)
					int randomDirection = (random(0, 1) ? 1 : -1);

					from.x += ((from.x + randomDirection) > 0 && (from.x + randomDirection) < gridSize.x)
								  ? randomDirection
								  : -randomDirection;

					path.push_back(Coords2D(from.x, from.y));
					vector<Coords2D> newPath = FindRandomPath(from, to, gridSize, obstacles);
					path.insert(path.end(), newPath.begin(), newPath.end());
					return path;
				}
			}

			from.y += increase.y;
		}

		// X is chosen
		else if (random(0, 1) && from.x != to.x)
		{
			for (Coords2D obstacle : obstacles)
			{
				if (obstacle.x == (from.x + increase.x) && obstacle.y == from.y)
				{
					// Randomically go top or bottom if possible (1 / -1)
					int randomDirection = (random(0, 1) ? 1 : -1);

					from.y += ((from.y + randomDirection) > 0 && (from.y + randomDirection) < gridSize.y)
								  ? randomDirection
								  : -randomDirection;

					path.push_back(Coords2D(from.x, from.y));
					vector<Coords2D> newPath = FindRandomPath(from, to, gridSize, obstacles);
					path.insert(path.end(), newPath.begin(), newPath.end());
					return path;
				}
			}

			from.x += increase.x;
		}

		// Y is chosen
		else if (from.y != to.y)
		{
			for (Coords2D obstacle : obstacles)
			{
				if (obstacle.x == from.x && obstacle.y == (from.y + increase.y))
				{
					// Randomically go top or bottom if possible (1 / -1)
					int randomDirection = (random(0, 1) ? 1 : -1);

					from.x += ((from.x + randomDirection) > 0 && (from.x + randomDirection) < gridSize.x)
								  ? randomDirection
								  : -randomDirection;

					path.push_back(Coords2D(from.x, from.y));
					vector<Coords2D> newPath = FindRandomPath(from, to, gridSize, obstacles);
					path.insert(path.end(), newPath.begin(), newPath.end());
					return path;
				}
			}

			from.y += increase.y;
		}

		path.push_back(Coords2D(from.x, from.y));
	}

	return path;
}
