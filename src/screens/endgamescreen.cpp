#include "endgamescreen.hpp"

#include <qfontdatabase.h>
#include <qheaderview.h>

EndGameScreen::EndGameScreen(const QString &map_name, const int score, QWidget *parent)
    : QWidget(parent), scoreLabel(new QLabel(this)), nameInput(new QLineEdit(this)), map_name(map_name), score(score),
      scoreManager(new ScoreManager(this)) {
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
    scoreTable = new QTableWidget(this);
    scoreTable->setColumnCount(2); // 2 colonnes
    scoreTable->setHorizontalHeaderLabels(QStringList() << "Name" << "Score"); // Noms des colonnes
    scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Étirer les colonnes pour remplir l'espace
    scoreManager->get_highscores(map_name);

    //zone de saisie de texte pour le nom
    nameInput->setPlaceholderText("Enter your name"); // Texte d'invite
    nameInput->setFont(QFont("Arial", 16)); // Police de taille 16n
    QFile file("playername.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        const auto playerName = in.readLine();
        nameInput->setText(playerName);
    }

    //bouton de soumission du nom
    auto *submitButton = new QPushButton("Submit my score", this);

    // Centrer horizontalement le champ de saisie et le bouton
    layout->setAlignment(Qt::AlignHCenter);

    connect(submitButton, &QPushButton::clicked, this, &EndGameScreen::submitName);

    // Création du bouton de retour au menu principal
    auto *returnButton = new QPushButton("Main Menu", this);
    snakefont.setPointSize(20);
    returnButton->setFont(snakefont);
    returnButton->setStyleSheet("padding-left: 20px; padding-right: 20px;");
    returnButton->setFixedSize(200, 50); // Taille fixe pour le bouton

    connect(returnButton, &QPushButton::clicked, this, &EndGameScreen::returnToMainMenu);

    auto *replayButton = new QPushButton("Replay", this);
    snakefont.setPointSize(20);
    replayButton->setFont(snakefont);
    replayButton->setStyleSheet("padding-left: 20px; padding-right: 20px;");
    replayButton->setFixedSize(200, 50); // Taille fixe pour le bouton

    connect(replayButton, &QPushButton::clicked, [this, map_name] {
        emit replayMap();
    });

    auto *leftLayout = new QVBoxLayout;
    leftLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    leftLayout->addWidget(nameInput);
    leftLayout->addWidget(submitButton);

    auto *centerLayout = new QHBoxLayout;
    centerLayout->addLayout(leftLayout);
    centerLayout->addWidget(scoreTable);

    // l'étiquette de score au layout
    layout->addStretch();
    layout->addWidget(endGameLabel);
    layout->addWidget(scoreLabel);
    layout->addStretch();
    layout->addSpacing(20); // Ajouter un espace de 20 pixels
    layout->addLayout(centerLayout);
    layout->addSpacing(20);

    // Ajout des bouton au layout
    auto *layoutH = new QHBoxLayout;
    layoutH->addWidget(returnButton);
    layoutH->addWidget(replayButton);

    layout->addLayout(layoutH);

    // Déf layout pour la fenêtre
    setLayout(layout);

    connect(scoreManager, &ScoreManager::highscoresReceived, this, &EndGameScreen::onHighscoresReceived);
}

//slot submitName pour clic bouton de soumission du nom
void EndGameScreen::submitName() {
    // Save player name for next game
    const auto playerName = nameInput->text(); // Récupérer le texte

    if (playerName.isEmpty())
        return;

    //fichier txt en mode écriture
    QFile file("playername.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier";
        return;
    }

    // Créer un flux QTextStream pour écrire dans le fichier
    QTextStream out(&file);
    out << playerName << Qt::endl;

    file.close();

    // Upload highscore
    scoreManager->upload_score(map_name, playerName, score);
    scoreManager->get_highscores(map_name);
}


void EndGameScreen::returnToMainMenu() {
    // Mettez ici le code pour revenir au menu principal, par exemple :
    emit back();
}

void EndGameScreen::onHighscoresReceived(const QList<QPair<QString, QVariant> > &highscores) {
    scoreTable->setRowCount(highscores.size()); // Set the number of rows in the table

    int row = 0;
    for (auto it = highscores.begin(); it != highscores.end(); ++it) {
        // Create a new item for the player's name
        auto *nameItem = new QTableWidgetItem(it->first);
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable); // Make the item non-editable

        // Create a new item for the player's score
        auto *scoreItem = new QTableWidgetItem(it->second.toString());
        scoreItem->setFlags(scoreItem->flags() ^ Qt::ItemIsEditable); // Make the item non-editable

        // Add the items to the table
        scoreTable->setItem(row, 0, nameItem);
        scoreTable->setItem(row, 1, scoreItem);

        ++row;
    }
}
