#include "gamearea.hpp"

GameArea::GameArea(const Map &map, const std::list<Position> *snake, const Position *applePos, const Position *bonusPos,
                   const bool draw_grid,
                   QWidget *parent) : QWidget(parent), map(map), snake(snake), applePos(applePos), bonusPos(bonusPos),
                                      draw_grid(draw_grid) {
    resizeWidget();
}

void GameArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    Position pos;

    // Dessine les cases
    for (pos.y = 0; pos.y < map.getHeight(); pos.y++)
        for (pos.x = 0; pos.x < map.getWidth(); pos.x++)
            painter.drawPixmap(pos.x * TEXTURE_SIZE, pos.y * TEXTURE_SIZE, map.getTileAt(pos).texture);

    // Dessine le serpent
    if (snake != nullptr && !snake->empty()) {
        const Position &posTete = snake->front();
        painter.drawPixmap(posTete.x * TEXTURE_SIZE, posTete.y * TEXTURE_SIZE, map.getSnakeHeadTexture());

        for (auto itSnake = ++snake->begin(); itSnake != snake->end(); ++itSnake)
            painter.drawPixmap(itSnake->x * TEXTURE_SIZE, itSnake->y * TEXTURE_SIZE, map.getSnakeBodyTexture());
    }

    // Dessine la pomme
    if (applePos != nullptr && *applePos != Position(-1, -1))
        painter.drawPixmap(applePos->x * TEXTURE_SIZE, applePos->y * TEXTURE_SIZE, map.getAppleTexture());

    // Dessine le bonus
    if (bonusPos != nullptr && *bonusPos != Position(-1, -1))
        painter.drawPixmap(bonusPos->x * TEXTURE_SIZE, bonusPos->y * TEXTURE_SIZE, map.getBonusTexture());


    if (draw_grid) {
        const QPen pen(Qt::black, 1); // black color, 1px width
        painter.setPen(pen);

        for (int x = 0; x <= map.getWidth(); x++)
            painter.drawLine(x * TEXTURE_SIZE, 0, x * TEXTURE_SIZE, map.getHeight() * TEXTURE_SIZE);

        for (int y = 0; y <= map.getHeight(); y++)
            painter.drawLine(0, y * TEXTURE_SIZE, map.getWidth() * TEXTURE_SIZE, y * TEXTURE_SIZE);
    }
}

void GameArea::mousePressEvent(QMouseEvent *event) {
    if (!(event->buttons() & (Qt::LeftButton | Qt::RightButton))) return;

    const int x = event->x() / TEXTURE_SIZE;
    const int y = event->y() / TEXTURE_SIZE;
    lastPos = Position(x, y);
    emit positionClicked(lastPos, event->buttons() & Qt::RightButton);
    setFocus();
}

void GameArea::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & (Qt::LeftButton | Qt::RightButton))) return;

    const int x = event->x() / TEXTURE_SIZE;
    const int y = event->y() / TEXTURE_SIZE;

    if (x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight())
        return;

    const Position currentPos(x, y);
    if (currentPos != lastPos) {
        lastPos = currentPos;
        emit positionClicked(lastPos, event->buttons() & Qt::RightButton);
    }
}

void GameArea::resizeWidget() {
    setFixedSize(map.getWidth() * TEXTURE_SIZE + 1, map.getHeight() * TEXTURE_SIZE + 1);
}
