#include <QDebug>
#include <QtXml>

#include "map.hpp"

#include "zip.hpp"

Map::Map() {
    width = 0;
    height = 0;
    tiles = nullptr;
}

Map::Map(const QFileInfo &file) {
    // Open the file
    InMemoryZipFileHandler zip_file("map1.skm");

    if (!zip_file.isValid()) {
        qDebug() << "Map: could not open file" << file.absoluteFilePath();
        return;
    }

    // Extract the map data
    QByteArray map_data;
    if (zip_file.extractFile("map.xml", map_data) < 0) {
        qDebug() << "Map: could not extract map data";
        return;
    }

    // Parse the map data as XML
    QDomDocument doc;
    doc.setContent(map_data);

    // Get map dimensions
    const QDomElement root = doc.documentElement();
    width = root.attribute("width").toInt();
    height = root.attribute("height").toInt();

    // Load default tiles types
    types["ground"] = {GROUND, QPixmap(":/images/ground.png")};
    types["wall"] = {WALL, QPixmap(":/images/wall.bmp")};

    // Get map tiles types
    const QDomNodeList map_types = root.elementsByTagName("type");
    for (int i = 0; i < map_types.size(); i++) {
        QDomElement tile = map_types.at(i).toElement();
        TileType type;

        QString type_str = tile.attribute("type");
        if (type_str == "ground") {
            type.type = GROUND;
        } else if (type_str == "wall") {
            type.type = WALL;
        } else if (type_str == "apple") {
            type.type = APPLE;
        } else {
            qDebug() << "Map: unknown tile type" << type_str;
            return;
        }

        QPixmap texture;
        QByteArray texture_data;
        zip_file.extractFile(tile.attribute("texture"), texture_data);
        if (!texture.loadFromData(texture_data)) {
            qDebug() << "Map: could not load texture";
            return;
        }
        type.texture = texture;

        types[tile.attribute("name")] = type;
    }

    // Get map tiles
    tiles = new TileType[width * height];
    const QDomNodeList map_tiles = root.elementsByTagName("tile");
    for (int i = 0; i < map_tiles.size(); i++) {
        QDomElement tile = map_tiles.at(i).toElement();
        QString type = tile.attribute("cell");
        tiles[tile.attribute("y").toInt() * width + tile.attribute("x").toInt()] = types[type];
    }
}

Map::Map(const Map &map) {
    width = map.width;
    height = map.height;
    types = map.types;
    tiles = new TileType[width * height];
    for (int i = 0; i < width * height; i++) {
        tiles[i] = map.tiles[i];
    }
}

Map::~Map() {
    delete [] tiles;
}

Map &Map::operator=(const Map &other) {
    if (this != &other) {
        delete[] tiles;

        width = other.width;
        height = other.height;
        types = other.types;

        tiles = new TileType[width * height];
        for (int i = 0; i < width * height; i++) {
            tiles[i] = other.tiles[i];
        }
    }

    return *this;
}
