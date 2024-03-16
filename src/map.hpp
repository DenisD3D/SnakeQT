#ifndef MAP_H
#define MAP_H
#include <qtablewidget.h>
#include <QtXml>
#include "inmemoryzip.hpp"
#include <QtCore/qmap.h>
#include <QtGui/qpixmap.h>

typedef enum { GROUND, WALL } TerrainType;

typedef enum { GAUCHE = 0, DROITE, HAUT, BAS } Direction;

struct TileType {
    TerrainType type = GROUND;
    QPixmap texture;
    bool is_default = false;
};

class Position {
public:
    int x{}, y{};

    Position();

    Position(int, int);

    bool operator==(const Position &) const;

    bool operator!=(const Position &) const;
};

class Map {
    QString file;
    int width = 10, height = 10;
    int init_x = 1, init_y = 1;
    int init_snake_length = 5;
    QString name, author;
    Direction init_direction = DROITE;
    QPixmap snake_head_texture, snake_body_texture, apple_texture;
    QString default_tile;
    QMap<QString, TileType> types;
    TileType **tiles;
    bool has_custom_apple_texture = false, has_custom_snake_head_texture = false, has_custom_snake_body_texture = false;

public:
    Map();

    explicit Map(const QString &, bool create_map = false);

    Map(const Map &);

    ~Map();

    bool save();

    bool saveAs();

    Map &operator=(const Map &other);

    TileType getTileAt(const Position &) const;

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getInitX() const { return init_x; }
    int getInitY() const { return init_y; }
    int getInitSnakeLength() const { return init_snake_length; }
    QString getName() const { return name; }
    QString getAuthor() const { return author; }
    Direction getInitDirection() const { return init_direction; }
    QPixmap getSnakeHeadTexture() const { return snake_head_texture; }
    QPixmap getSnakeBodyTexture() const { return snake_body_texture; }
    QPixmap getAppleTexture() const { return apple_texture; }
    QMap<QString, TileType> getTypes() const { return types; }
    TileType **getTiles() const { return tiles; }
    QString getDefaultTile() const { return default_tile; }

    // Setters
    void setWidth(const int w) {
        const auto newTiles = new TileType *[w * height];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < w; x++) {
                // loop until the new width
                if (x < width && y < height) {
                    newTiles[y * w + x] = tiles[y * width + x]; // copy existing TileType
                } else {
                    newTiles[y * w + x] = &types[default_tile]; // fill with default tile for new tiles
                }
            }
        }
        delete[] tiles;

        width = w;
        tiles = newTiles;
    }

    void setHeight(const int h) {
        const auto newTiles = new TileType *[width * h];
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < width; x++) {
                if (x < width && y < height) {
                    newTiles[y * width + x] = tiles[y * width + x];
                } else {
                    newTiles[y * width + x] = &types[default_tile];
                }
            }
        }
        delete[] tiles;

        height = h;
        tiles = newTiles;
    }

    void setInitX(const int x) { init_x = x; }
    void setInitY(const int y) { init_y = y; }
    void setInitSnakeLength(const int length) { init_snake_length = length; }
    void setName(const QString &n) { name = n; }
    void setAuthor(const QString &a) { author = a; }
    void setInitDirection(const Direction d) { init_direction = d; }

    void setSnakeHeadTexture(const QPixmap *texture) {
        if (texture == nullptr) {
            has_custom_snake_head_texture = false;
            snake_head_texture.load(":/images/sneak_head.png");
            return;
        }

        has_custom_snake_head_texture = true;
        snake_head_texture = *texture;
    }

    void setSnakeBodyTexture(const QPixmap *texture) {
        if (texture == nullptr) {
            has_custom_snake_body_texture = false;
            snake_body_texture.load(":/images/sneak_body.png");
            return;
        }

        has_custom_snake_body_texture = true;
        snake_body_texture = *texture;
    }

    void setAppleTexture(const QPixmap *texture) {
        if (texture == nullptr) {
            has_custom_apple_texture = false;
            apple_texture.load(":/images/apple.png");
            return;
        }

        has_custom_apple_texture = true;
        apple_texture = *texture;
    }

    void setDefaultTile(const QString &type) { default_tile = type; }

    void setTileAt(Position pos, const QString &type);

    void setTypeTexture(const QString &text, const QPixmap &pixmap);

    void setNewTypeName(const QString &old_name, const QString &new_name);

    void setFile(const QString &f) { file = f; }

    void deleteType(const QString &text);

    void setTypeType(const QString &text, TerrainType type);

    void createType(const QString &name);
};


#endif //MAP_H
