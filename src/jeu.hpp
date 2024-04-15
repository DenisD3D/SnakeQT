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
    Position bonusPos = Position(-1, -1);
    std::mt19937 gen;
    bool pause = false;
    bool gameOver = false;
    int score = 0;

public:
    Jeu();

    explicit Jeu(const Map &);

    Jeu(const Jeu &);

    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();

    bool tick();

    const std::list<Position> *getSnake() const;

    // Indique si la case ? une position donn?e existe et est libre
    bool posValide(const Position &, int flags) const;

    // Modifie la direction
    void setDirection(Direction);

    const Position *getApplePos() const;

    const Position *getBonusPos() const;

    const Map &getMap() const;

    void togglePause() {
        pause = !pause;
    }

    bool isPaused() const {
        return pause;
    }

    int getScore() const {
        return score;
    }

    void increaseScore(const int point) {
        score = score + point;
    }

    bool isGameOver() const {
        return gameOver;
    }
};

#endif
