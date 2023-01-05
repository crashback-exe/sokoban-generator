
# Sokoban level generator

A sokoban level generator
Usage: `Sokoban.exe <height> <width> <box-count> [json] [debug]`
Params:
- `height` - Board height
- `width` - Board width
- `box-count` - Number of boxes in the board
- `play` - (Optional) Start a minimal interface to try the game
- `json` - (Optional) Output the result as  a json string
- `debug` - (Optional) Generate and show boards infinitely

The download of the executable can be found [here](https://github.com/crashback-exe/sokoban-generator/releases/latest)

## Example
`Sokoban.exe 8 8 3 play`
- `@` - Player
- `$` - Box
- `.` - Target
 
![Game example](https://user-images.githubusercontent.com/65626314/210843740-95fe8e9c-3a44-4b59-8422-e353fc5ce6d6.png =500x)

## How it works
The board is initialized filling it with walls, the player is placed in a random cell in the board, and the boxes and the goals are then placed in random, unique cells in the board; after the generation, a random, valid path between every box and his corresponding target is generated, preferring an already carved, valid path, then the path is optimized by checking if a cell occurs twice in the path and if does, removing the cells in the middle of these two, if an obstacle is found in the path it gets dodged; then, the cells needed to move the box from its first position to his target are carved and the path between the player and the first cell needed to move the box is carved.

## Known issues
Sometimes the program crashes due to an error that occurs in 0,0775% of levels. This issue can be fixed.