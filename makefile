all: clear compile exe

clear:
	cls

compile:
	g++ main.cpp -o Sokoban.exe -static-libgcc -std=c++20 -I ./src

exe:
	Sokoban.exe 8 8 3