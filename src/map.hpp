#ifndef MAP_H
#define MAP_H
#include <qfileinfo.h>
#include <qpixmap.h>

#define DEFAULT_TILE_TYPES_COUNT 1

typedef enum { GROUND, WALL, APPLE } TerrainType;

struct TileType {
    TerrainType type;
    QPixmap texture;
};

class Map {
public:
    int width{}, height{};
    QHash<QString, TileType> types;
    TileType *tiles;

    Map();

    explicit Map(const QFileInfo &);

    Map(const Map &);

    ~Map();

    Map &operator=(const Map &other);
};


#endif //MAP_H
