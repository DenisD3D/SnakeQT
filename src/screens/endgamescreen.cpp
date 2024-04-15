#include "endgamescreen.hpp"

#include <qfontdatabase.h>

EndGameScreen::EndGameScreen(const int score, QWidget *parent)
    : QWidget(parent), scoreLabel(new QLabel(this)), nameInput(new QLineEdit(this)) {
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

    // tableau score
    QTableWidget *scoreTable = new QTableWidget(5, 2, this);
    scoreTable->setHorizontalHeaderLabels(QStringList() << "NOM" << "SCORE");
    QString playerName[] = {"Denis", "Augustin", "Tactique", "Toni", "Bailauto"};
    int playerScore[] = {300, 250, 210, 20, 10};

// Boucle pour remplir le tableau avec les valeurs spécifiques
    for (int i = 0; i < 5; ++i) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(playerName[i]);
        QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(playerScore[i]));
        scoreTable->setItem(i, 0, nameItem);
        scoreTable->setItem(i, 1, scoreItem);
    }

    //widget vide avec un layout horizontal pour centrer le tableau
    QWidget *tableWidget = new QWidget(this);
    QHBoxLayout *tableLayout = new QHBoxLayout(tableWidget);
    tableLayout->addWidget(scoreTable);
    tableLayout->setContentsMargins(0, 0, 0, 0);
    tableLayout->setSpacing(0);

    // Ajouter les widgets au layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(endGameLabel);
    mainLayout->addWidget(scoreLabel);
    mainLayout->addWidget(tableWidget, 0, Qt::AlignHCenter); // Centrer horizontalement le widget contenant le tableau


    // l'étiquette de score au layout
    layout->addStretch();
    layout->addWidget(endGameLabel);
    layout->addWidget(scoreLabel);
    layout->addStretch();
    layout->addSpacing(20); // Ajouter un espace de 20 pixels
    layout->addWidget(scoreTable);
    layout->addSpacing(20);

    //zone de saisie de texte pour le nom
    QVBoxLayout *textInputLayout = new QVBoxLayout;
    nameInput->setPlaceholderText("Enter your name"); // Texte d'invite
    nameInput->setFont(QFont("Arial", 16)); // Police de taille 16
    textInputLayout->addWidget(nameInput);

    //bouton de soumission du nom
    QPushButton *submitButton = new QPushButton("Submit", this);
    textInputLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Centrer horizontalement le champ de saisie et le bouton
    layout->setAlignment(Qt::AlignHCenter);

    connect(submitButton, &QPushButton::clicked, this, &EndGameScreen::submitName);

    // Création du bouton de retour au menu principal
    QPushButton *returnButton = new QPushButton("Return to Main Menu", this);
    returnButton->setFont(QFont("Arial", 16));
    returnButton->setFixedSize(200, 50); // Taille fixe pour le bouton
    connect(returnButton, &QPushButton::clicked, this, &EndGameScreen::returnToMainMenu);




    // Ajouter les widgets au layout
    layout->addWidget(nameInput);
    layout->addWidget(submitButton);
    // Ajout du bouton au layout
    layout->addWidget(returnButton);

    // Déf layout pour la fenêtre
    setLayout(layout);
}

//slot submitName pour clic bouton de soumission du nom
void EndGameScreen::submitName() {
    QString playerName = nameInput->text(); // Récupérer le texte

    //fichier CSV en mode écriture
    QFile file("scores.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier";
        return;
    }

    // Créer un flux QTextStream pour écrire dans le fichier
    QTextStream out(&file);
    out << playerName << endl;

    file.close();
}


void EndGameScreen::returnToMainMenu() {
    // Mettez ici le code pour revenir au menu principal, par exemple :
    emit returnToMainMenuClicked(); // Émet un signal pour informer le parent que le bouton a été cliqué
}