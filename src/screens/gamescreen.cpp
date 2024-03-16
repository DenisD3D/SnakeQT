#include "gamescreen.hpp"


/**
 * Construct main game window
 */
GameScreen::GameScreen(QWidget *parent, const QString &file_info): QWidget(parent) {
    jeu = Jeu(Map(file_info));
    jeu.init();

    gameArea = new GameArea(jeu.getMap(), jeu.getSnake(), jeu.getApplePos(), false, this);

    auto *layout = new QVBoxLayout;
    auto *mapNameLabel = new QLabel(jeu.getMap().getAuthor() + " by " + jeu.getMap().getAuthor(), this);
    auto *scoreLabel = new QLabel("Score: 0", this);

    layout->addWidget(mapNameLabel);
    layout->addWidget(gameArea);
    layout->addWidget(scoreLabel);

    auto *hLayout = new QHBoxLayout(this);
    hLayout->addStretch(1);
    hLayout->addLayout(layout);
    hLayout->addStretch(1);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameScreen::handleTimer);
    timer->start(100);
}

/**
 * Handle controls
 */
void GameScreen::keyPressEvent(QKeyEvent *event) {
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
void GameScreen::handleTimer() {
    jeu.evolue();
    update();
}
