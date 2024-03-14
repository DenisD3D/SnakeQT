#include "snakewindow.hpp"

#include <iostream>

#define HEADER_HEIGHT 50
#define TEXTURE_SIZE 32

using namespace std;

/**
 * Construct main game window
 */
SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags): QFrame(pParent, flags) {
    if (!pixmapCorps.load(":/images/body.png")) {
        cout << "Impossible d'ouvrir body.png" << endl;
        exit(-1);
    }

    if (!pixmapTete.load(":/images/head.png")) {
        cout << "Impossible d'ouvrir head.png" << endl;
        exit(-1);
    }

    jeu = Jeu(Map(QFileInfo("map1.skm")));
    jeu.init();

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer);
    timer->start(100);

    setFixedSize(jeu.getNbCasesX() * TEXTURE_SIZE, jeu.getNbCasesY() * TEXTURE_SIZE + HEADER_HEIGHT);
}

/**
 * Paint the game window
 */
void SnakeWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    Position pos;

    // Dessine les cases
    for (pos.y = 0; pos.y < jeu.getNbCasesY(); pos.y++)
        for (pos.x = 0; pos.x < jeu.getNbCasesX(); pos.x++)
            painter.drawPixmap(pos.x * TEXTURE_SIZE, pos.y * TEXTURE_SIZE + HEADER_HEIGHT, jeu.getCase(pos).texture);

    // Dessine le serpent
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty()) {
        const Position &posTete = snake.front();
        painter.drawPixmap(posTete.x * TEXTURE_SIZE, posTete.y * TEXTURE_SIZE + HEADER_HEIGHT, jeu.getMap().snake_head_texture);

        for (auto itSnake = ++snake.begin(); itSnake != snake.end(); ++itSnake)
            painter.drawPixmap(itSnake->x * TEXTURE_SIZE, itSnake->y * TEXTURE_SIZE + HEADER_HEIGHT, jeu.getMap().snake_body_texture);
    }

    // Dessine la pomme
    if (jeu.posValide(jeu.getPosApple()))
        painter.drawPixmap(jeu.getPosApple().x * TEXTURE_SIZE, jeu.getPosApple().y * TEXTURE_SIZE + HEADER_HEIGHT, jeu.getMap().types["apple"].texture);
}

/**
 * Handle controls
 */
void SnakeWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Left)
        jeu.setDirection(GAUCHE);
    if (event->key() == Qt::Key_Right)
        jeu.setDirection(DROITE);
    if (event->key() == Qt::Key_Up)
        jeu.setDirection(HAUT);
    if (event->key() == Qt::Key_Down)
        jeu.setDirection(BAS);
    update();
}

/**
 * Tick the game
 */
void SnakeWindow::handleTimer() {
    jeu.evolue();
    update();
}
