all: clear compile exe

clear:
	cls

compile_fast:
	g++ main.cpp -o Sokoban.exe -static-libgcc -std=c++20 -I ./src
	
compile:
	g++ main.cpp -o Sokoban.exe -static -static-libgcc -static-libstdc++ -std=c++20 -I ./src -O2

test:
	g++ main.cpp -o Sokoban.exe -static -static-libgcc -static-libstdc++ -std=c++20 -I ./src -O2
	Sokoban.exe 8 8 3 debug

exe:
	Sokoban.exe 8 8 3