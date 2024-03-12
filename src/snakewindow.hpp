#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class SnakeWindow final : public QFrame {
protected:
    Jeu jeu;
    QPixmap pixmapCorps, pixmapTete, pixmapMur;

public:
    explicit SnakeWindow(QWidget *pParent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    void handleTimer();

protected:
    void paintEvent(QPaintEvent *) override;

    void keyPressEvent(QKeyEvent *) override;

    void addWall();

    void removeWall();
};
#endif
