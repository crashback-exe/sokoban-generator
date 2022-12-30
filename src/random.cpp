#pragma once

#include <random>

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