all: clear compile run

clear:
	cls

compile:
	g++ main.cpp -o Sokoban.exe -static-libgcc -std=c++20 -I ./src
	
compile_optimized:
	g++ main.cpp -o Sokoban.exe -static -static-libgcc -static-libstdc++ -std=c++20 -I ./src -O2

test:
	make compile_optimized
	Sokoban.exe 8 8 3 debug

run:
	Sokoban.exe 8 8 3