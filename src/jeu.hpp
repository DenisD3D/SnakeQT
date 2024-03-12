#ifndef JEU_HPP
#define JEU_HPP

#include <queue>
#include <list>

#include "map.hpp"

typedef enum { GAUCHE, DROITE, HAUT, BAS } Direction;

class Position {
public:
    int x{}, y{};

    Position();

    Position(int, int);

    bool operator==(const Position &) const;

    bool operator!=(const Position &) const;
};

class Jeu {
protected:
    Map map;
    std::list<Position> snake;
    Direction dirSnake;
    std::queue<Direction> directionsBuffer;

public:
    Jeu();

    explicit Jeu(const Map &);

    Jeu(const Jeu &);

    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();

    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;

    int getNbCasesY() const;

    // Retourne la case ? une position donn?e
    TileType getCase(const Position &) const;

    // Retourne la liste des ?l?ments du serpent en lecture seule
    const std::list<Position> &getSnake() const;

    // Indique si la case ? une position donn?e existe et est libre
    bool posValide(const Position &) const;

    // Modifie la direction
    void setDirection(Direction);
};

#endif
