#ifndef GAMEAREA_HPP
#define GAMEAREA_HPP


#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "../map.hpp"
#include "../constants.hpp"
#include <iostream>

class GameArea final : public QWidget {
    Q_OBJECT

    const Map &map;
    const std::list<Position> *snake;
    const Position *applePos;
    Position lastPos;
    bool draw_grid;

public:
    explicit GameArea(const Map &map, const std::list<Position> *snake = nullptr, const Position *applePos = nullptr,
                      bool draw_grid = false, QWidget *parent = nullptr);

    void resizeWidget();

signals:
    void positionClicked(Position pos, bool is_right_click);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // GAMEAREA_HPP
