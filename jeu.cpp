#include <iostream>
#include <cassert>
#include "jeu.hpp"

using namespace std;

Position::Position() = default;

Position::Position(const int a, const int b) {
    x = a;
    y = b;
}

bool Position::operator==(const Position &pos) const {
    return (x == pos.x && y == pos.y);
}

bool Position::operator!=(const Position &pos) const {
    return (x != pos.x || y != pos.y);
}

Jeu::Jeu() {
    terrain = nullptr;
    largeur = 0;
    hauteur = 0;
    dirSnake = DROITE;
}

Jeu::Jeu(const Jeu &jeu): snake(jeu.snake) {
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;

    if (jeu.terrain != nullptr) {
        terrain = new Case[largeur * hauteur];
        for (int c = 0; c < largeur * hauteur; c++)
            terrain[c] = jeu.terrain[c];
    } else
        terrain = nullptr;
}

Jeu::~Jeu() {
    if (terrain != nullptr)
        delete[] terrain;
}

Jeu &Jeu::operator=(const Jeu &jeu) {
    if (terrain != nullptr)
        delete[] terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    snake = jeu.snake;

    if (jeu.terrain != nullptr) {
        terrain = new Case[largeur * hauteur];
        for (int c = 0; c < largeur * hauteur; c++)
            terrain[c] = jeu.terrain[c];
    } else
        terrain = nullptr;
    return *this;
}

bool Jeu::init() {
    int x, y;
    // list<Position>::iterator itSnake;

    const char terrain_defaut[15][21] = {
        "####..##############",
        "#........##........#",
        "#.#####..##...####.#",
        "#........##........#",
        "#..................#",
        "#..................#",
        "....................",
        "....................",
        "....................",
        "....................",
        "#..................#",
        "#..................#",
        "#.....#......#.....#",
        "#.....#......#.....#",
        "####..##############"
    };

    largeur = 20;
    hauteur = 15;

    terrain = new Case[largeur * hauteur];

    for (y = 0; y < hauteur; ++y)
        for (x = 0; x < largeur; ++x)
            if (terrain_defaut[y][x] == '#')
                terrain[y * largeur + x] = MUR;
            else
                terrain[y * largeur + x] = VIDE;

    int longueurSerpent = 5;
    snake.clear();

    Position posTete;
    posTete.x = 15;
    posTete.y = 8;
    for (int i = 0; i < longueurSerpent; i++) {
        snake.push_back(posTete);
        posTete.x--;
    }

    return true;
}

void Jeu::evolue() {
    if (!directionsBuffer.empty()) {
        dirSnake = directionsBuffer.front();
        directionsBuffer.pop();
    }

    Position posTest;
    list<Position>::iterator itSnake;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    posTest.x = snake.front().x + depX[dirSnake];
    posTest.y = snake.front().y + depY[dirSnake];

    if (posValide(posTest)) {
        snake.pop_back();
        snake.push_front(posTest);
    }
}

int Jeu::getNbCasesX() const {
    return largeur;
}

int Jeu::getNbCasesY() const {
    return hauteur;
}

Case Jeu::getCase(const Position &pos) const {
    assert(pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur);
    return terrain[pos.y * largeur + pos.x];
}

const list<Position> &Jeu::getSnake() const {
    return snake;
}

bool Jeu::posValide(const Position &pos) const {
    if (pos.x >= 0 && pos.x < largeur && pos.y >= 0 && pos.y < hauteur
        && terrain[pos.y * largeur + pos.x] == VIDE) {
        list<Position>::const_iterator itSnake;
        itSnake = snake.begin();
        while (itSnake != snake.end() && *itSnake != pos)
            itSnake++;
        return (itSnake == snake.end());
    } else
        return false;
}

void Jeu::setDirection(const Direction dir) {
    // Input buffer is full
    if (directionsBuffer.size() > 4)
        return;

    Direction previousDir = directionsBuffer.empty() ? dirSnake :  directionsBuffer.back();

    // Trying to go in the opposite direction
    if (previousDir == GAUCHE && dir == DROITE)
        return;
    if (previousDir == DROITE && dir == GAUCHE)
        return;
    if (previousDir == HAUT && dir == BAS)
        return;
    if (previousDir == BAS && dir == HAUT)
        return;

    directionsBuffer.push(dir);
}

void Jeu::addRandomWall() {
    int x, y;
    Position pos;

    do {
        x = rand() % largeur;
        y = rand() % hauteur;
        pos.x = x;
        pos.y = y;
    } while (terrain[y * largeur + x] != VIDE || !posValide(pos));

    terrain[y * largeur + x] = MUR;
}

void Jeu::removeRandomWall() {
    int x, y;
    Position pos;

    do {
        x = rand() % largeur;
        y = rand() % hauteur;
        pos.x = x;
        pos.y = y;
    } while (terrain[y * largeur + x] != MUR);

    terrain[y * largeur + x] = VIDE;
}
