# SnakeQT
###### Snake game in Python using Pygame and PyQt5 - Denis DAVIAUD & Augustin ROUH - INSA CVL 2024

## Build
1. [Download and install](https://cmake.org/resources/) cmake (version 3.11 or later recommended).
2. Install Qt5 (recommended version 5.15.2)
3. Clone the repository (including submodules) with the following command:
```bash
git clone --recurse-submodules https://github.com/DenisD3D/SnakeQT.git
```
4. Run cmake in the root directory of the project.
```bash
cmake -S . -B build
cmake --build build
```


## Tile preprocessing using imageMagick
Some tiles aren't in the right size or contains transparency. To fix this, the following command can be used to fix all images in subfolders of the current directory:
```bash
find . -name '*.png' -exec convert {} -scale 32x32 -background "#ecb06e" -alpha remove -alpha off {} \;
```
Replace #ecb06e by the background color of your map
Extract a spritesheet into individual images:
```bash
magick convert roguelikeSheet_transparent.png -crop 57x31-1-1@ +repage +adjoin spaced-1_%d.png
```
Where 57 is the number of sprite in the x-axis and 31 in the y-axis and -1-1 is the offset between each sprite
Tested on https://kenney.nl/ assets