#include "endgamescreen.hpp"

#include <qfontdatabase.h>

EndGameScreen::EndGameScreen(const int score, QWidget *parent)
    : QWidget(parent), scoreLabel(new QLabel(this)) {
    // Load custom font
    const int id = QFontDatabase::addApplicationFont(":/images/game_played.otf");
    const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont snakefont(family);
    snakefont.setPointSize(36);

    // Layout vertical pour organiser les widgets
    const auto layout = new QVBoxLayout(this);

    // Étiquette de texte pour afficher le message de fin de jeu
    const auto endGameLabel = new QLabel("Game Over!", this);
    endGameLabel->setFont(snakefont); // Police en gras et taille 36
    endGameLabel->setAlignment(Qt::AlignCenter); // Centrez le texte

    // Étiquette de texte pour afficher le score
    const auto scoreLabel = new QLabel("Score: " + QString::number(score), this);
    scoreLabel->setFont(QFont("Arial", 24, QFont::Bold)); // Police en gras et taille 24
    scoreLabel->setAlignment(Qt::AlignCenter); // Centrez le texte

    // Ajouter l'étiquette de score au layout
    layout->addStretch();
    layout->addWidget(endGameLabel);
    layout->addWidget(scoreLabel);
    layout->addStretch();

    // Définir le layout pour la fenêtre
    setLayout(layout);
}
