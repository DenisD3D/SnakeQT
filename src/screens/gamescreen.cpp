#include "gamescreen.hpp"
#include <iostream>

/**
 * Construct main game window
 */
GameScreen::GameScreen(QWidget *parent, const QString &file_info): QWidget(parent) {
    jeu = Jeu(Map(file_info));
    jeu.init();

    gameArea = new GameArea(jeu.getMap(), jeu.getSnake(), jeu.getApplePos(), false, this);

    auto *layout = new QVBoxLayout;
    auto *mapNameLabel = new QLabel(jeu.getMap().getName() + " by " + jeu.getMap().getAuthor(), this);
     scoreLabel = new QLabel("Score: 0", this);

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


    // Pause overlay
    pauseOverlay = new QWidget(this);
    pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 127); color: white; font-size: 24px;");
    pauseOverlay->hide(); // hide it initially

    auto *pauseLabel = new QLabel("Game Paused", pauseOverlay);
    pauseLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    auto *resumeLabel = new QLabel("Press P to resume", pauseOverlay); // New label
    resumeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    resumeLabel->setStyleSheet("font-size: 18px;");

    auto *pauseLayout = new QVBoxLayout(pauseOverlay);
    pauseLayout->setContentsMargins(0, 0, 0, 0);
    pauseLayout->setSpacing(0);
    pauseLayout->addWidget(pauseLabel);
    pauseLayout->addWidget(resumeLabel);


}

void GameScreen::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    pauseOverlay->resize(event->size());
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

    if (event->key() == Qt::Key_P) {
        jeu.togglePause();

        if (jeu.isPaused()) {
            pauseOverlay->show();
        } else {
            pauseOverlay->hide();
        }
    }


    update();
}

void GameScreen::updateScoreLabel() {
    scoreLabel->setText("Score: " + QString::number(jeu.getScore()));
}

void GameScreen::endGame() {
    emit gameOver(jeu.getScore());
}

/**
 * Tick the game
 */
void GameScreen::handleTimer() {
    jeu.tick();

    updateScoreLabel();

    if (jeu.game_over != 0) {
        // Game over
        if (jeu.game_over == 1) {
            // Win
            std::cout << "You win!" << std::endl;
        } else {
            // Lose
            std::cout << "You lose!" << std::endl;
            emit gameOver(jeu.getScore());
        }

    }


    update();
}
