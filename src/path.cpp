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

bool ObstacleX(Coords2D, Coords2D, vector<Coords2D>&, vector<Coords2D>);

bool ObstacleY(Coords2D from, Coords2D increase, vector<Coords2D> &path, vector<Coords2D> obstacles = {})
{
	Coords2D ghostPlayer;
	ghostPlayer.x = from.x;
	ghostPlayer.y = from.y + increase.y;

	for (Coords2D obstacle : obstacles)
	{
		if (obstacle.x == ghostPlayer.x && obstacle.y == ghostPlayer.y)
		{
			increase.x = -increase.x;
			if (ObstacleX(from, increase, path, obstacles))
				return true;

			from.x += increase.x;
			path.push_back(Coords2D(from.x, from.y));


			if (ObstacleY(from, increase, path, obstacles));
				return true;

			from.y += increase.y;
			path.push_back(Coords2D(from.x, from.y));
			return true;
		}
	}

	return false;
}

bool ObstacleX(Coords2D from, Coords2D increase, vector<Coords2D> &path, vector<Coords2D> obstacles = {})
{
	Coords2D ghostPlayer;
	ghostPlayer.x = from.x + increase.x;
	ghostPlayer.y = from.y;

	for (Coords2D obstacle : obstacles)
	{
		if (obstacle.x == ghostPlayer.x && obstacle.y == ghostPlayer.y)
		{
			increase.y = -increase.y;
			if (ObstacleY(from, increase, path, obstacles));
				return true;

			from.y += increase.y;
			path.push_back(Coords2D(from.x, from.y));


			if (ObstacleX(from, increase, path, obstacles));
				return true;

			from.x += increase.x;
			path.push_back(Coords2D(from.x, from.y));
			return true;
		}
	}

	return false;
}


/// @brief Finds a random valid path between two coordinates avoiding obstacles
/// @param from Point to start from
/// @param to Point of arrival
/// @param obstacles The obstacles to avoid
/// @return A Coords2D vector with all the points of the path
vector<Coords2D> FindRandomPath(Coords2D from, Coords2D to, vector<Coords2D> obstacles = {})
{
	// Vector to store all the path coords
	vector<Coords2D> path;

	// Find the direction where to go
	Coords2D increase = FindDirection(from, to);

	cout << "X: " << increase.x << " Y: " << increase.y;
	cout << endl;

	bool place;

	// While we are not arrived at destination
	while (from.x != to.x || from.y != to.y)
	{
		place = true;

		// Randomically chose between going x or y
		if ((random(0, 1) && from.x != to.x) || from.y == to.y)
		{
			// X is chosen
			
			if (ObstacleX(from, increase, path, obstacles))
				FindRandomPath(from, to, obstacles);
			// for (Coords2D obstacle : obstacles)
			// {
			// 	if (obstacle.x == (from.x + increase.x) && obstacle.y == from.y)
			// 	{
			// 		place = false;
			// 		from.y -= 1;
			// 		path.push_back(Coords2D(from.x, from.y));
			// 		from.x -= increase.x;
			// 		path.push_back(Coords2D(from.x, from.y));
			// 		FindRandomPath(from, to, obstacles);
			// 	}
			// }

			if (place)
				from.x += increase.x;
		}
		else if (from.y != to.y)
		{
			// Y is chosen

			if (ObstacleY(from, increase, path, obstacles))
				FindRandomPath(from, to, obstacles);

			for (Coords2D obstacle : obstacles)
			{
				if (obstacle.x == from.x && obstacle.y == (from.y + increase.y))
				{
					place = false;
					from.x -= 1;
					path.push_back(Coords2D(from.x, from.y));
					from.y -= increase.y;
					path.push_back(Coords2D(from.x, from.y));
					FindRandomPath(from, to, obstacles);
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