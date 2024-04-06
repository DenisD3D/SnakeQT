

#include "endgamescreen.hpp"

EndGameScreen::EndGameScreen(int score, QWidget *parent)
        : QWidget(parent), scoreLabel(new QLabel(this)) {
    // Layout vertical pour organiser les widgets
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Étiquette de texte pour afficher le score
    QLabel *scoreLabel = new QLabel("Score: " + QString::number(score), this);
    scoreLabel->setFont(QFont("Arial", 24, QFont::Bold)); // Police en gras et taille 24
    scoreLabel->setAlignment(Qt::AlignCenter); // Centrez le texte

    // Ajouter l'étiquette de score au layout
    layout->addWidget(scoreLabel);

    // Définir le layout pour la fenêtre
    setLayout(layout);
}