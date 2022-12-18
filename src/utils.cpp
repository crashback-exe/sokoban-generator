#pragma once

#include <random>
#include <string>
#include <iostream>
#include "level.cpp"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

/// @brief Pair of Coordinates
struct Coords2D
{
	int x;
	int y;
};

/// @brief List of possible directions
enum DIRECTIONS
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

/// @brief Generate a random integer in the given range (both ends comprhended)
/// @param min Min number to be generated
/// @param max Max number to be generated
/// @return The random number
int random(int min, int max)
{
	// Credits: https://chat.openai.com/chat
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

/// @brief Prints a debug counter and an eventual string
/// @param info Eventual string to be debugged
void Debug(string info = "")
{
	static int counter = 1;
	cout << "# Debug " << counter << " " << info << endl;
	counter++;
}

void Debug(const char *c)
{
	string s(c);
	Debug(s);
}

void Debug(auto info)
{
	Debug(to_string(info));
}