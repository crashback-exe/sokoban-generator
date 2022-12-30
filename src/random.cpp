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
	auto seed = rd();
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

// #pragma once

// #include <random>
// #include <cstdlib>

// const int SEED = std::random_device{}();

// /// @brief Generate a random integer in the given range (both ends comprhended)
// /// @param min Min number to be generated
// /// @param max Max number to be generated
// /// @return The random number
// int random(int min, int max)
// {
// 	// Credits: https://chat.openai.com/chat
// 	static long int n = 0;
// 	n++;
//     std::mt19937 generator(SEED + n);
//     std::uniform_int_distribution<int> distribution(min, max);
//     return distribution(generator);
// }