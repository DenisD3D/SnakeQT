#include <iostream>
#include <cassert>
#include <QtXml>

#include "jeu.hpp"

using namespace std;

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

Jeu::Jeu() {
    dirSnake = DROITE;
    std::random_device rd;
    gen.seed(rd());
}

Jeu::Jeu(const Map &map): map(map) {
    dirSnake = DROITE;
    std::random_device rd;
    gen.seed(rd());
}

Jeu::Jeu(const Jeu &jeu): map(jeu.map), snake(jeu.snake) {
    dirSnake = jeu.dirSnake;
    std::random_device rd;
    gen.seed(rd());
}

Jeu::~Jeu() {
}


Jeu &Jeu::operator=(const Jeu &jeu) {
    dirSnake = jeu.dirSnake;
    snake = jeu.snake;
    map = jeu.map;

    return *this;
}

/**
 * Initialize the game
 * @return true if the game is successfully initialized
 */
bool Jeu::init() {
    snake.clear();

    Position posTete;
    posTete.x = map.init_x;
    posTete.y = map.init_y;
    for (int i = 0; i < map.init_snake_length; i++) {
        snake.push_back(posTete);
    }
    dirSnake = map.init_direction;


    std::uniform_int_distribution<> distr(0, map.width - 1);
    do {
        posApple.x = distr(gen);
        posApple.y = distr(gen);
    } while (!posValide(posApple));

    return true;
}

void Jeu::evolue() {
    if (!directionsBuffer.empty()) {
        dirSnake = directionsBuffer.front();
        directionsBuffer.pop();
    }

    Position posTest;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    posTest.x = snake.front().x + depX[dirSnake];
    posTest.y = snake.front().y + depY[dirSnake];

    if (posValide(posTest)) {
        if (posTest == posApple) {
            std::uniform_int_distribution<> distr(0, map.width - 1);
            do {
                posApple.x = distr(gen);
                posApple.y = distr(gen);
            } while (!posValide(posApple));
            // Don't remove the last element of the snake to make it grow
        } else {
            // Remove the last element of the snake to make it move
            snake.pop_back();
        }
        snake.push_front(posTest);
    } else {
        // Game over
        // snake.clear();
    }
}

int Jeu::getNbCasesX() const {
    return map.width;
}

int Jeu::getNbCasesY() const {
    return map.height;
}

TileType Jeu::getCase(const Position &pos) const {
    assert(pos.x>=0 && pos.x<map.width && pos.y>=0 && pos.y<map.height);
    return map.tiles[pos.y * map.width + pos.x];
}

const list<Position> &Jeu::getSnake() const {
    return snake;
}

bool Jeu::posValide(const Position &pos) const {
    if (pos.x >= 0 && pos.x < map.width && pos.y >= 0 && pos.y < map.height
        && map.tiles[pos.y * map.width + pos.x].type == GROUND) {
        auto itSnake = snake.begin();
        while (itSnake != snake.end() && *itSnake != pos)
            ++itSnake;
        return (itSnake == snake.end());
    }
    return false;
}

/**
 * Set the direction of the snake, if there is some input delay, schedule the direction change
 * @param dir Direction to set
 */
void Jeu::setDirection(const Direction dir) {
    // Input buffer is full
    if (directionsBuffer.size() > 4)
        return;

    const Direction previousDir = directionsBuffer.empty() ? dirSnake : directionsBuffer.back();

    // Prevent going in the opposite direction
    if (previousDir == GAUCHE && dir == DROITE)
        return;
    if (previousDir == DROITE && dir == GAUCHE)
        return;
    if (previousDir == HAUT && dir == BAS)
        return;
    if (previousDir == BAS && dir == HAUT)
        return;

    // Add direction to buffer, unpacked in evolue()
    directionsBuffer.push(dir);
}

Position &Jeu::getPosApple() {
    return posApple;
}

Map &Jeu::getMap() {
    return map;
}
