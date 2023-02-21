
# Sokoban level generator

Small program that generates random Sokoban levels

Usage: `Sokoban.exe <height> <width> <box-count> [play] [json] [debug]`

Params:
- `height` - Board height
- `width` - Board width
- `box-count` - Number of boxes in the board
- `play` - (Optional) Start a minimal interface to try the game
- `json` - (Optional) Output the result as  a json string
- `debug` - (Optional) Generate and show boards infinitely

The download of the executable can be found [here](https://github.com/crashback-exe/sokoban-generator/releases/latest)

*Note: without specifying any optional flags the output will just be a generated level with the player*

## Compiling
If you have `make` installed on your system you can do
```bash
$ make compile
```
or (to make the executable more portable and fast
```bash
$ make compile_optimized
```

Without using `make`
```bash
$ g++ main.cpp -o Sokoban.exe -static-libgcc -std=c++20 -I ./src
```
or (fast and portable)
```bash
$ g++ main.cpp -o Sokoban.exe -static -static-libgcc -static-libstdc++ -std=c++20 -I ./src -O2
```

## Example
`Sokoban.exe 8 8 3 play`
- `@` - Player
- `$` - Box
- `.` - Target
 
<img src="https://user-images.githubusercontent.com/65626314/210843740-95fe8e9c-3a44-4b59-8422-e353fc5ce6d6.png" alt="Game example" width=500 />

## How it works
The board is initialized filling it with walls, the player is placed in a random cell in the board, and the boxes and the goals are then placed in random, unique cells in the board; after the generation, a random, valid path between every box and his corresponding target is generated, preferring an already carved, valid path, then the path is optimized by checking if a cell occurs twice in the path and if does, removing the cells in the middle of these two, if an obstacle is found in the path it gets dodged; then, the cells needed to move the box from its first position to his target are carved and the path between the player and the first cell needed to move the box is carved.

## Known issues
Sometimes the program crashes due to an error that occurs in 0,0775% of levels. This issue can be fixed.
