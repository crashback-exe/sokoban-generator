#pragma once

#include <iostream>
#include <vector>
#include "utils.cpp"
#include "level.cpp"

using std::cout;
using std::vector;

Coords2D FindDirection(Coords2D from, Coords2D to) {
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
vector<Coords2D> FindRandomPath(Coords2D from, Coords2D to, vector<Coords2D> obstacles = {})
{
	vector<Coords2D> path;
	Coords2D increase = FindDirection(from, to);

	bool place;
	while (from.x != to.x || from.y != to.y)
	{
		place = true;
		if ((random(0, 1) && from.x != to.x) || from.y == to.y)
		{
			// todo
			// stesa linea sivalletto in mezzo boxetto
			for (Coords2D obstacle : obstacles)
			{
				if (obstacle.x == (from.x + increase.x) && obstacle.y == from.y)
				{
					place = false;
					break;
				}
			}

			if (place)
				from.x += increase.x;
		}

		else if (from.y != to.y)
		{
			for (Coords2D obstacle : obstacles)
			{
				if (obstacle.x == from.x && obstacle.y == (from.y + increase.y))
				{
					place = false;
					break;
				}
			}

			if (place)
				from.y += increase.y;
		}
		if (place)
			path.push_back(Coords2D(from.x, from.y));
	}

	path.pop_back();
	return path;
}