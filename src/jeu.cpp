#include "jeu.hpp"
#include <iostream>

#define APPLE_VALUE 10
#define BONUS_VALUE 50

using namespace std;

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
    pause = jeu.pause;
}

Jeu::~Jeu() {
}


Jeu &Jeu::operator=(const Jeu &jeu) {
    dirSnake = jeu.dirSnake;
    snake = jeu.snake;
    map = jeu.map;
    pause = jeu.pause;

    return *this;
}

/**
 * Initialize the game
 * @return true if the game is successfully initialized
 */
bool Jeu::init() {
    snake.clear();

    Position posTete;
    posTete.x = map.getInitX();
    posTete.y = map.getInitY();
    for (int i = 0; i < map.getInitSnakeLength(); i++) {
        snake.push_back(posTete);
    }
    dirSnake = map.getInitDirection();


    std::uniform_int_distribution distrw(0, map.getWidth() - 1);
    std::uniform_int_distribution distrh(0, map.getHeight() - 1);
    int attempts = 0;
    do {
        applePos.x = distrw(gen);
        applePos.y = distrh(gen);
        attempts++;
        if (attempts > 2 * map.getWidth() * map.getHeight()) {
            return false;
        }
    } while (!posValide(applePos, APPLE_SPAWN));

    return true;
}

/**
 * The main game logic, called periodically
 * @return true if the player lost during this tick
 */
bool Jeu::tick() {
    if (pause || gameOver) // Game is paused or finished, don't do anything
        return false;

    if (!directionsBuffer.empty()) {
        dirSnake = directionsBuffer.front();
        directionsBuffer.pop();
    }

    Position posTest;

    constexpr int depX[] = {-1, 1, 0, 0};
    constexpr int depY[] = {0, 0, -1, 1};

    posTest.x = (snake.front().x + depX[dirSnake] + map.getWidth()) % map.getWidth();
    posTest.y = (snake.front().y + depY[dirSnake] + map.getHeight()) % map.getHeight();

    if (posValide(posTest, WALKABLE)) {
        snake.push_front(posTest); // Add the new head

        if (bonusPos == Position(-1, -1)) {
            // There is no bonus on the map
            // There is one chance out of 15 to spawn a bonus
            if (std::uniform_int_distribution(0, 90)(gen) == 0) {
                std::uniform_int_distribution distrw(0, map.getWidth() - 1);
                std::uniform_int_distribution distrh(0, map.getHeight() - 1);
                int attempts = 0;
                do {
                    bonusPos.x = distrw(gen);
                    bonusPos.y = distrh(gen);
                    attempts++;
                    if (attempts > 2 * map.getWidth() * map.getHeight()) {
                        break; // No bonus this time due to lack of space
                    }
                } while (!posValide(bonusPos, BONUS_SPAWN));
            }
        } else if (posTest == bonusPos) {
            // The snake eats the bonus, increase the score and remove the bonus
            increaseScore(BONUS_VALUE);
            bonusPos = Position(-1, -1);
        }

        if (posTest == applePos) {
            // The snake eats the apple, increase the score and place a new apple
            increaseScore(APPLE_VALUE);

            std::uniform_int_distribution<> distrw(0, map.getWidth() - 1);
            std::uniform_int_distribution<> distrh(0, map.getHeight() - 1);
            int attempts = 0;
            do {
                applePos.x = distrw(gen);
                applePos.y = distrh(gen);
                attempts++;
                if (attempts > 2 * map.getWidth() * map.getHeight()) {
                    gameOver = true;
                    return true;
                }
            } while (!posValide(applePos, APPLE_SPAWN));
            // Don't remove the last element of the snake to make it grow
        } else {
            // Remove the last element of the snake to make it move
            snake.pop_back();
        }
    } else {
        // Game over
        snake.clear();
        gameOver = true; // perdu
        return true;
    }

    return false;
}

const list<Position> *Jeu::getSnake() const {
    return &snake;
}

bool Jeu::posValide(const Position &pos, const int flags) const {
    if (pos.x < 0 || pos.x >= map.getWidth() || pos.y < 0 || pos.y >= map.getHeight()) // Out of bounds
        return false;

    if (!(map.getTileAt(pos).type & flags))
        return false;

    auto itSnake = snake.begin();
    while (itSnake != snake.end() && *itSnake != pos)
        ++itSnake;
    return (itSnake == snake.end()); // Is the position free of snake ?
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

    // Add direction to buffer, unpacked in tick()
    directionsBuffer.push(dir);
}

const Position *Jeu::getApplePos() const {
    return &applePos;
}

const Position *Jeu::getBonusPos() const {
    return &bonusPos;
}

const Map &Jeu::getMap() const {
    return map;
}
