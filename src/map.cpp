#include "map.hpp"


Position::Position() = default;

Position::Position(const int a, const int b) {
    x = a;
    y = b;
}

bool Position::operator==(const Position &pos) const {
    return x == pos.x && y == pos.y;
}

bool Position::operator!=(const Position &pos) const {
    return x != pos.x || y != pos.y;
}

Map::Map() {
    width = 0;
    height = 0;
    tiles = nullptr;
}

Map::Map(const QString &absolute_file_path, const bool create_map): file(absolute_file_path) {
    default_tile = "ground";
    apple_texture.load(":/images/apple.png");
    snake_head_texture.load(":/images/sneak_head.png");
    snake_body_texture.load(":/images/sneak_body.png");

    // Load default tiles types
    types["ground"] = {GROUND, QPixmap(":/images/ground.png"), true};
    types["wall"] = {WALL, QPixmap(":/images/wall.png"), true};

    if (create_map) {
        tiles = new TileType *[width * height];
        std::fill_n(tiles, width * height, &types[default_tile]);
        return;
    }

    // Open the file
    const InMemoryZipFileReader zip_file(absolute_file_path);

    if (!zip_file.isValid()) {
        qDebug() << "Map: could not open file" << absolute_file_path;
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

    // Get map settings
    const QDomElement root = doc.documentElement();
    width = root.attribute("width").toInt();
    height = root.attribute("height").toInt();

    if (root.hasAttribute("name")) {
        name = root.attribute("name");
    }
    if (root.hasAttribute("author")) {
        author = root.attribute("author");
    }

    if (root.hasAttribute("apple_texture")) {
        has_custom_apple_texture = true;
        QByteArray texture_data;
        zip_file.extractFile(root.attribute("apple_texture"), texture_data);
        if (!apple_texture.loadFromData(texture_data)) {
            qDebug() << "Map: could not load texture";
            return;
        }
    }

    // Get snake settings
    const QDomNodeList snakeElements = root.elementsByTagName("snake");
    if (!snakeElements.isEmpty()) {
        const QDomElement snake = snakeElements.at(0).toElement();
        init_x = snake.attribute("init_x").toInt();
        init_y = snake.attribute("init_y").toInt();
        init_snake_length = snake.attribute("init_length").toInt();
        const QString direction = snake.attribute("init_direction");
        if (direction == "left") {
            init_direction = GAUCHE;
        } else if (direction == "right") {
            init_direction = DROITE;
        } else if (direction == "up") {
            init_direction = HAUT;
        } else if (direction == "down") {
            init_direction = BAS;
        } else {
            qDebug() << "Map: unknown direction" << direction;
        }

        if (snake.hasAttribute("head_texture")) {
            has_custom_snake_head_texture = true;
            QByteArray texture_data;
            zip_file.extractFile(snake.attribute("head_texture"), texture_data);
            if (!snake_head_texture.loadFromData(texture_data)) {
                qDebug() << "Map: could not load texture";
                return;
            }
        }

        if (snake.hasAttribute("body_texture")) {
            has_custom_snake_body_texture = true;
            QByteArray texture_data;
            zip_file.extractFile(snake.attribute("body_texture"), texture_data);
            if (!snake_body_texture.loadFromData(texture_data)) {
                qDebug() << "Map: could not load texture";
                return;
            }
        }
    }

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
        type.is_default = false;

        types[tile.attribute("name")] = type;
    }

    // Get map tiles
    tiles = new TileType *[width * height];
    if (root.hasAttribute("default_tile")) {
        default_tile = root.attribute("default_tile");
    }
    std::fill_n(tiles, width * height, &types[default_tile]);
    const QDomNodeList map_tiles = root.elementsByTagName("tile");
    for (int i = 0; i < map_tiles.size(); i++) {
        QDomElement tile = map_tiles.at(i).toElement();
        QString type = tile.attribute("cell");
        tiles[tile.attribute("y").toInt() * width + tile.attribute("x").toInt()] = &types[type];
    }
}

bool Map::save() {
    const InMemoryZipFileWriter writer(file);
    if (!writer.isValid()) {
        qDebug() << "Map: could not open file to save map: " << file;
        return false;
    }

    // Generate map data
    QDomDocument doc;
    QDomElement root = doc.createElement("map");
    doc.appendChild(root);

    root.setAttribute("width", width);
    root.setAttribute("height", height);
    root.setAttribute("name", name);
    root.setAttribute("author", author);
    if (default_tile != "ground") {
        root.setAttribute("default_tile", default_tile);
    }
    if (has_custom_apple_texture) {
        root.setAttribute("apple_texture", "apple.png");
    }

    QDomElement snake = doc.createElement("snake");
    snake.setAttribute("init_x", init_x);
    snake.setAttribute("init_y", init_y);
    snake.setAttribute("init_length", init_snake_length);
    if (init_direction == GAUCHE) {
        snake.setAttribute("init_direction", "left");
    } else if (init_direction == DROITE) {
        snake.setAttribute("init_direction", "right");
    } else if (init_direction == HAUT) {
        snake.setAttribute("init_direction", "up");
    } else if (init_direction == BAS) {
        snake.setAttribute("init_direction", "down");
    }
    if (has_custom_snake_head_texture) {
        snake.setAttribute("head_texture", "snake_head.png");
    }
    if (has_custom_snake_body_texture) {
        snake.setAttribute("body_texture", "snake_body.png");
    }
    root.appendChild(snake);

    for (auto it = types.keyValueBegin(); it != types.keyValueEnd(); ++it) {
        if (it->second.is_default) {
            continue;
        }

        QDomElement type = doc.createElement("type");
        type.setAttribute("name", it->first);
        if (it->second.type == GROUND) {
            type.setAttribute("type", "ground");
        } else if (it->second.type == WALL) {
            type.setAttribute("type", "wall");
        }
        type.setAttribute("texture", it->first + ".png");
        root.appendChild(type);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QDomElement tile = doc.createElement("tile");
            tile.setAttribute("x", x);
            tile.setAttribute("y", y);

            for (auto it = types.keyValueBegin(); it != types.keyValueEnd(); ++it) {
                if (&(it->second) == tiles[y * width + x]) {
                    tile.setAttribute("cell", it->first);
                }
            }

            if (tile.hasAttribute("cell")) {
                root.appendChild(tile);
            }
        }
    }

    if (!writer.addFileToZip("map.xml", doc.toByteArray())) {
        qDebug() << "Map: could not save map data";
        return false;
    }

    // Save all tiles types textures
    for (auto it = types.keyValueBegin(); it != types.keyValueEnd(); ++it) {
        if (it->second.is_default) {
            continue;
        }

        QByteArray texture_data;
        QBuffer buffer(&texture_data);
        buffer.open(QIODevice::WriteOnly);
        it->second.texture.save(&buffer, "PNG");
        if (!writer.addFileToZip(it->first + ".png", texture_data)) {
            qDebug() << "Map: could not save texture: " << it->first;
            return false;
        }
    }

    // Save custom apple texture
    if (has_custom_apple_texture) {
        QByteArray texture_data;
        QBuffer buffer(&texture_data);
        buffer.open(QIODevice::WriteOnly);
        apple_texture.save(&buffer, "PNG");
        if (!writer.addFileToZip("apple.png", texture_data)) {
            qDebug() << "Map: could not save apple texture";
            return false;
        }
    }

    // Save snake textures
    if (has_custom_snake_head_texture) {
        QByteArray texture_data;
        QBuffer buffer(&texture_data);
        buffer.open(QIODevice::WriteOnly);
        snake_head_texture.save(&buffer, "PNG");
        if (!writer.addFileToZip("snake_head.png", texture_data)) {
            qDebug() << "Map: could not save snake head texture";
            return false;
        }
    }
    if (has_custom_snake_body_texture) {
        QByteArray texture_data;
        QBuffer buffer(&texture_data);
        buffer.open(QIODevice::WriteOnly);
        snake_body_texture.save(&buffer, "PNG");
        if (!writer.addFileToZip("snake_body.png", texture_data)) {
            qDebug() << "Map: could not save snake body texture";
            return false;
        }
    }

    qDebug() << "Map: saved map to: " << file;
    return true;
}

Map::Map(const Map &map) {
    file = map.file;
    width = map.width;
    height = map.height;
    types = map.types;
    init_x = map.init_x;
    init_y = map.init_y;
    init_snake_length = map.init_snake_length;
    init_direction = map.init_direction;
    snake_head_texture = map.snake_head_texture;
    snake_body_texture = map.snake_body_texture;
    apple_texture = map.apple_texture;
    default_tile = map.default_tile;
    tiles = new TileType *[width * height];
    for (int i = 0; i < width * height; i++) {
        tiles[i] = map.tiles[i];
    }
    has_custom_apple_texture = map.has_custom_apple_texture;
    has_custom_snake_head_texture = map.has_custom_snake_head_texture;
    has_custom_snake_body_texture = map.has_custom_snake_body_texture;

    name = map.name;
    author = map.author;
}

Map::~Map() {
    delete [] tiles;
}

Map &Map::operator=(const Map &other) {
    if (this != &other) {
        delete[] tiles;

        file = other.file;
        width = other.width;
        height = other.height;
        types = other.types;
        init_x = other.init_x;
        init_y = other.init_y;
        init_snake_length = other.init_snake_length;
        init_direction = other.init_direction;
        snake_head_texture = other.snake_head_texture;
        snake_body_texture = other.snake_body_texture;
        apple_texture = other.apple_texture;
        default_tile = other.default_tile;
        tiles = new TileType *[width * height];
        for (int i = 0; i < width * height; i++) {
            tiles[i] = other.tiles[i];
        }

        has_custom_apple_texture = other.has_custom_apple_texture;
        has_custom_snake_head_texture = other.has_custom_snake_head_texture;
        has_custom_snake_body_texture = other.has_custom_snake_body_texture;

        name = other.name;
        author = other.author;
    }

    return *this;
}

TileType Map::getTileAt(const Position &pos) const {
    assert(pos.x>=0 && pos.x<getWidth() && pos.y>=0 && pos.y<getHeight());
    return *tiles[pos.y * getWidth() + pos.x];
}

void Map::setTileAt(const Position pos, const QString &type) {
    assert(pos.x>=0 && pos.x<getWidth() && pos.y>=0 && pos.y<getHeight());
    tiles[pos.y * getWidth() + pos.x] = &types[type];
}

void Map::setTypeTexture(const QString &text, const QPixmap &pixmap) {
    if (types[text].is_default) return;

    types[text].texture = pixmap;
}

void Map::setNewTypeName(const QString &old_name, const QString &new_name) {
    if (types[old_name].is_default) return;

    types[new_name] = types[old_name];
    types.remove(old_name);
}

void Map::deleteType(const QString &text) {
    if (types[text].is_default) return;

    for (int i = 0; i < width * height; i++) {
        if (tiles[i] == &types[text]) {
            tiles[i] = &types[default_tile];
        }
    }

    types.remove(text);
}

void Map::setTypeType(const QString &text, TerrainType type) {
    if (types[text].is_default) return;

    types[text].type = type;
}

void Map::createType(const QString &name) {
    // Make a QPixmap that means "no texture" 32x32
    QImage image(32, 32, QImage::Format_ARGB32);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            if ((x / 4 + y / 4) % 2 == 0) {
                image.setPixel(x, y, qRgb(0, 0, 0));
            } else {
                image.setPixel(x, y, qRgb(119, 60, 197));
            }
        }
    }

    types[name] = {GROUND, QPixmap::fromImage(image), false};
}
