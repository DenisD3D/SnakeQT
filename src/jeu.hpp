#ifndef JEU_HPP
#define JEU_HPP

#include <queue>
#include <list>
#include <random>

#include "map.hpp"

class Jeu {
protected:
    Map map;
    std::list<Position> snake;
    Direction dirSnake;
    std::queue<Direction> directionsBuffer;
    Position applePos = Position(-1, -1);
    std::mt19937 gen;
    bool pause = false;

public:
    Jeu();

    explicit Jeu(const Map &);

    Jeu(const Jeu &);

    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();

    void tick();

    const std::list<Position> *getSnake() const;

    // Indique si la case ? une position donn?e existe et est libre
    bool posValide(const Position &, int flags) const;

    // Modifie la direction
    void setDirection(Direction);

    const Position *getApplePos() const;

    const Map &getMap() const;

    void togglePause() {
        pause = !pause;
    }

    bool isPaused() const {
        return pause;
    }
};

#endif
