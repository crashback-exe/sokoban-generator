#include <vector>
#include <iostream>
#include <stdexcept>
#include <windows.h>
#include <conio.h>
#include "src/level.cpp"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc <= 3)
		throw invalid_argument("Missing arguments!");
	SokobanLevel level(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));

	/* Debug */
	// for (;;)
	// {
	// 	system("cls");
	// 	level.Generate();
	// 	level.Show();
	// }
	/* Debug */

	level.Generate();
	level.Show();

	char input;
	for (;;)
	{
		input = getch();
		switch (input)
		{
		case 'w':
			level.PlayerMove(UP);
			break;
		case 'a':
			level.PlayerMove(LEFT);
			break;
		case 's':
			level.PlayerMove(DOWN);
			break;
		case 'd':
			level.PlayerMove(RIGHT);
			break;
		case 'r':
			system("cls");
			level.Generate();
			break;
		case 'e':
			return 0;
		default:
			continue;
		}
		system("cls");
		level.Show();
	}
}
