# SnakeQT

###### Snake game in C++ using QT5 - Denis DAVIAUD & Augustin ROUH - INSA CVL 2024

## Build

1. [Download and install](https://cmake.org/resources/) cmake (version 3.11 or later recommended) and mingw.
2. Install Qt5 (recommended version 5.15.2)
   On linux

```bash
sudo apt-get install qt5-default
# ou
pip install aqtinstall
aqt install-qt linux desktop 5.15.2 gcc_64
```

On Windows

```bash
pip install aqtinstall
aqt install-qt windows desktop 5.15.2 win64_mingw81
```

3. Clone the repository (including submodules) with the following command:

```bash
git clone --recurse-submodules https://github.com/DenisD3D/SnakeQT.git
```

4. If qt is installed through aqtinstall, set the environment variable `Qt5_DIR` to the directory containing
   the `Qt5Config.cmake` file (e.g. `C:\Qt\5.15.2\mingw81_64\lib\cmake\Qt5`)
5. Run cmake in the root directory of the project.

```bash
cmake -S . -B build
cmake --build build
```

6. On Windows, define QT_QPA_PLATFORM_PLUGIN_PATH to the directory containing the `qwindows.dll` file (
   e.g. `C:\Qt\5.15.2\mingw81_64\plugins\platforms`) before running SnakeQT.exe.
7. Run the executable in the build directory.

Already compiled version of the game are available for each commit under the Actions section of Github. They are compiled automatically on a remote server for each new version of the game : https://github.com/DenisD3D/SnakeQT/actions

## Features

- File based map loading
- Embedded custom texture in map files
- In-game map editor for maps up to 30x30
- In-game map browser and downloader from remote servers
- Online high score system computed from apple and bonus eaten
- Full screen support

## Maps

Maps are available directly in the game. They can also be downloaded manually from
the [map repository](https://github.com/DenisD3D/SnakeQT-API/tree/main/maps) and placed in
the `maps` folder. To make a map available for download it must be added to the map repository.
Current most advanced maps are :

- Basic 2
- Island 1

## Map File format

Map are stored as a zip file ending with .skm. The zip file contains the following files:

- `map.xml`: a xml file containing the map data

```xml
<?xml version="1.0" encoding="utf-8" ?>
<!-- Define map properties: size, name, apple texture and default tile -->
<map width="10" height="10" apple_texture="custom_apple.png" name="Map map" author="Map Author"
     description="Map description" default_tile="custom_ground">
    <!-- Define snake properties: start position & length, textures -->
    <snake init_x="7" init_y="7" init_length="2" init_direction="up" head_texture="custom_snake_head.png"
           body_texture="custom_snake_body.png"/>

    <!-- Define a custom tile type with its own texture. type is an integer defining properties as flags (WALKABLE = 1 << 0, APPLE_SPAWN = 1 << 1, BONUS_SPAWN = 1 << 2 -->
    <type name="custom_ground" texture="custom_ground.png" type="7"/>

    <!-- Set the tile at x, y to the cell of type name. Tile that aren't set default to default_tile -->
    <tile x="0" y="0" cell="custom_wall"/>
</map>
```

- All the textures used in the map, placed as png file at the root of the zip file

## Possible improvement

- Use map name, display author and high score in the map browser (as return by API)
- Add a campaign mode where game switch to the next map when a certain score is reached
- Support for adaptive textures (choosing the right texture depending on the surrounding tiles) instead of loading all 9
  textures as separate ones

## API

The game uses a REST API to download maps and upload high scores. The API is available at https://snakeqt.denisd3d.fr.
To source code is available on [github](https://github.com/DenisD3D/SnakeQT-API) and the documentation directly on
the [dedicated website page](https://snakeqt.denisd3d.fr/docs). HighScore upload is voluntarily hidden from the
documentation to avoid manual upload.

## Tile preprocessing using imageMagick

The tiles texture used in the game come from https://kenney.nl/ assets. Some textures aren't in the right size for the
game or might contain transparency. To fix this, the following command can be used to fix all images in subfolders of
the current directory:

```bash
find . -name '*.png' -exec convert {} -scale 32x32 -background "#ecb06e" -alpha remove -alpha off {} \;
```

Replace #ecb06e by the background color of your map
If all textures are on the same spritesheet, they can be extracted into individual images using:

```bash
magick convert roguelikeSheet_transparent.png -crop 57x31-1-1@ +repage +adjoin spaced-1_%d.png
```

Where 57 is the number of sprite in the x-axis and 31 in the y-axis and -1-1 is the offset between each sprite.

