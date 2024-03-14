#ifndef MAP_H
#define MAP_H
#include <qfileinfo.h>
#include <qpixmap.h>

#define DEFAULT_TILE_TYPES_COUNT 1

typedef enum { GROUND, WALL, APPLE } TerrainType;

typedef enum { GAUCHE, DROITE, HAUT, BAS } Direction;

struct TileType {
    TerrainType type;
    QPixmap texture;
};

class Map {
public:
    int width{}, height{};
    int init_x = 1, init_y = 1;
    int init_snake_length = 5;
    Direction init_direction = DROITE;
    QPixmap snake_head_texture, snake_body_texture;

    QHash<QString, TileType> types;
    TileType *tiles;

    Map();

    explicit Map(const QFileInfo &);

    Map(const Map &);

    ~Map();

    Map &operator=(const Map &other);
};


#endif //MAP_H
