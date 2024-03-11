#include <iostream>
#include "snakewindow.hpp"

using namespace std;

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags): QFrame(pParent, flags) {
    if (!pixmapCorps.load(":/images/snake_corps.png")) {
        cout << "Impossible d'ouvrir snake_corps.png" << endl;
        exit(-1);
    }

    if (!pixmapTete.load(":/images/snake_tete.png")) {
        cout << "Impossible d'ouvrir snake_tete.png" << endl;
        exit(-1);
    }

    if (!pixmapMur.load(":/images/mur.bmp")) {
        cout << "Impossible d'ouvrir mur.bmp" << endl;
        exit(-1);
    }

    jeu.init();

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer);
    timer->start(100);

    // Taille des cases en pixels
    const int largeurCase = pixmapMur.width();
    const int hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX() * largeurCase, jeu.getNbCasesY() * hauteurCase);
}

void SnakeWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    Position pos;

    // Taille des cases en pixels
    const int largeurCase = pixmapMur.width();
    const int hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (pos.y = 0; pos.y < jeu.getNbCasesY(); pos.y++)
        for (pos.x = 0; pos.x < jeu.getNbCasesX(); pos.x++)
            if (jeu.getCase(pos) == MUR)
                painter.drawPixmap(pos.x * largeurCase, pos.y * hauteurCase, pixmapMur);

    // Dessine le serpent
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty()) {
        const Position &posTete = snake.front();
        painter.drawPixmap(posTete.x * largeurCase, posTete.y * hauteurCase, pixmapTete);

        for (auto itSnake = ++snake.begin(); itSnake != snake.end(); ++itSnake)
            painter.drawPixmap(itSnake->x * largeurCase, itSnake->y * hauteurCase, pixmapCorps);
    }
}

void SnakeWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Left)
        jeu.setDirection(GAUCHE);
    else if (event->key() == Qt::Key_Right)
        jeu.setDirection(DROITE);
    else if (event->key() == Qt::Key_Up)
        jeu.setDirection(HAUT);
    else if (event->key() == Qt::Key_Down)
        jeu.setDirection(BAS);
    update();
}

void SnakeWindow::handleTimer() {
    jeu.evolue();
    update();
}
