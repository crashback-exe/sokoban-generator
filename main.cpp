#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "src/level.cpp"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc <= 3)
		cout << "Usage: Sokoban.exe <height> <width> <box-count> [json] [debug]";
	SokobanLevel level(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));

	if (string(argv[4]) == "debug")
		for (;;)
		{
			system("cls");
			level.Generate();
			level.Show();
		}

	if (string(argv[4]) == "json")
	{
		level.Generate();
		level.ShowJson();
		return 0;
	}

	level.Generate();

	char input;
	for (;;)
	{
		level.Show();
		cout << "Move with WASD, R to regenerate, E to exit" << endl;
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
	}

	return 0;
}
