#include "jeu.hpp"

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


    std::uniform_int_distribution<> distr(0, map.getWidth() - 1);
    do {
        applePos.x = distr(gen);
        applePos.y = distr(gen);
    } while (!posValide(applePos, APPLE_SPAWN));

    return true;
}

void Jeu::evolue() {
    if (pause) // Game is paused, don't do anything
        return;

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

        if (posTest == applePos) {
            // The snake eats the apple, place a new apple
            std::uniform_int_distribution<> distr(0, map.getWidth() - 1);
            do {
                applePos.x = distr(gen);
                applePos.y = distr(gen);
            } while (!posValide(applePos, APPLE_SPAWN));
            // Don't remove the last element of the snake to make it grow
        } else {
            // Remove the last element of the snake to make it move
            snake.pop_back();
        }
    } else {
        // Game over
        snake.clear();
    }
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

    // Add direction to buffer, unpacked in evolue()
    directionsBuffer.push(dir);
}

const Position *Jeu::getApplePos() const {
    return &applePos;
}

const Map &Jeu::getMap() const {
    return map;
}
