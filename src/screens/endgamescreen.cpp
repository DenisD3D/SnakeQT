#include "endgamescreen.hpp"

#include <qfontdatabase.h>

EndGameScreen::EndGameScreen(const int score, QWidget *parent)
    : QWidget(parent), scoreLabel(new QLabel(this)), nameLineEdit(new QLineEdit(this)) {
    // Load custom font
    const int id = QFontDatabase::addApplicationFont(":/images/game_played.otf");
    const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont snakefont(family);
    snakefont.setPointSize(36);

    // Layout vertical pour organiser les widgets
    const auto layout = new QVBoxLayout(this);

    // Étiquette de texte pour afficher le message de fin de jeu
    const auto endGameLabel = new QLabel("Game Over!", this);
    endGameLabel->setFont(snakefont);
    endGameLabel->setAlignment(Qt::AlignCenter); // Centrez le texte

    // Étiquette de texte pour afficher le score
    const auto scoreLabel = new QLabel("Score: " + QString::number(score), this);
    scoreLabel->setFont(QFont("Arial", 24, QFont::Bold)); // Police en gras et taille 24
    scoreLabel->setAlignment(Qt::AlignCenter); // Centrez le texte

    // l'étiquette de score au layout
    layout->addStretch();
    layout->addWidget(endGameLabel);
    layout->addWidget(scoreLabel);
    layout->addStretch();

    //zone de saisie de texte pour le nom
    nameLineEdit->setPlaceholderText("Enter your name"); // Texte d'invite
    nameLineEdit->setFont(QFont("Arial", 16)); // Police de taille 16

    //bouton de soumission du nom
    QPushButton *submitButton = new QPushButton("Submit", this);

    // Centrer horizontalement le champ de saisie et le bouton
    layout->setAlignment(Qt::AlignHCenter);

    connect(submitButton, &QPushButton::clicked, this, &EndGameScreen::submitName);

    // Ajouter les widgets au layout
    layout->addWidget(nameLineEdit);
    layout->addWidget(submitButton);

    // Déf layout pour la fenêtre
    setLayout(layout);
}

//slot submitName pour clic bouton de soumission du nom
void EndGameScreen::submitName() {
    QString playerName = nameLineEdit->text(); // Récupérer le texte saisi dans la zone de saisie
    // Effectuer le traitement nécessaire avec le nom du joueur, par exemple, l'envoyer à une fonction de gestion des scores
    // Ou afficher le nom quelque part
}