#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include "src/level.cpp"
using namespace std;

int main(int argc, char *argv[])
{
	SokobanLevel testLevel(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
	testLevel.Generate();
	testLevel.Show();

	char input;
	for (;;)
	{
		input = getch();
		switch (input)
		{
		case 'w':
			testLevel.PlayerMove(UP);
			break;
		case 'a':
			testLevel.PlayerMove(LEFT);
			break;
		case 's':
			testLevel.PlayerMove(DOWN);
			break;
		case 'd':
			testLevel.PlayerMove(RIGHT);
			break;
		case 'e':
			return 0;
		case 'r':
			system("cls");
			testLevel.Generate();
			break;
		default:
			continue;
		}
		system("cls");
		testLevel.Show();
	}
}