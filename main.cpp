#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "src/level.cpp"
using namespace std;

int main(int argc, char *argv[])
{
	string usage = "Usage: Sokoban.exe <height> <width> <box-count> [play] [json] [debug]";
	
	if (argc <= 3)
	{
		cout << usage;
		return 1;
	}

	SokobanLevel level(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));

	if (argc > 4)
	{
		if (string(argv[4]) == "debug")
		{
			for (int k = 0;; k++)
			{
				for (int l = 0; l < (int)((k / 10) + 1); l++)
					cout << "\b";
				cout << k;
				level.Generate();
			}
			return 0;
		}

		if (string(argv[4]) == "json")
		{
			level.Generate();
			level.ShowJson();
			return 0;
		}

		if (string(argv[4]) == "play")
		{
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

		cout << usage;
		return 1;
	}

	level.Generate();
	level.Show();

	return 0;
}
