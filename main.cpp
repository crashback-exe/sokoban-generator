#include <iostream>
#include <vector>
#include "src/level.cpp"
#include "src/path.cpp"
using namespace std;

int main(int argc, char *argv[])
{

	SokobanLevel testLevel(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
	testLevel.Generate();
	testLevel.Show();

	return 0;
}
