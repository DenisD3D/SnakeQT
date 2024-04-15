#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#include <QPushButton>
#include <QVBoxLayout>

class MainMenu final : public QWidget {
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr) : QWidget(parent) {
        // Load custom font
        const int id = QFontDatabase::addApplicationFont(":/images/game_played.otf");
        const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont snakefont(family);
        snakefont.setPointSize(20);

        const auto layoutH = new QHBoxLayout(this);

        const auto layoutV = new QVBoxLayout;

        // Title
        const auto title = new QLabel("SnakeQT", this);
        title->setFont(snakefont);
        title->setStyleSheet("font-size: 70px;");
        title->setAlignment(Qt::AlignCenter);
        const auto subTitle = new QLabel("by Denis Daviaud & Augustin Rouh", this);
        subTitle->setFont(snakefont);
        subTitle->setStyleSheet("font-size: 12px;");
        subTitle->setAlignment(Qt::AlignCenter);

        // Buttons
        const auto playButton = new QPushButton("Play a map", this);
        playButton->setFont(snakefont);
        playButton->setStyleSheet("padding-left: 20px; padding-right: 20px;");
        const auto mapEditor = new QPushButton("Create or edit a map", this);
        mapEditor->setFont(snakefont);
        mapEditor->setStyleSheet("padding-left: 20px; padding-right: 20px;");
        const auto exitButton = new QPushButton("Exit", this);
        exitButton->setFont(snakefont);
        exitButton->setStyleSheet("padding-left: 20px; padding-right: 20px;");

        layoutV->addStretch();
        layoutV->addWidget(title);
        layoutV->addWidget(subTitle);
        layoutV->addStretch();
        layoutV->addWidget(playButton);
        layoutV->addWidget(mapEditor);
        layoutV->addWidget(exitButton);
        layoutV->addStretch();

        layoutH->addStretch();
        layoutH->addLayout(layoutV);
        layoutH->addStretch();

        connect(playButton, &QPushButton::clicked, this, &MainMenu::playMapClicked);
        connect(mapEditor, &QPushButton::clicked, this, &MainMenu::createOrEditMapClicked);
        connect(exitButton, &QPushButton::clicked, this, &MainMenu::exitClicked);
    }

signals:
    void playMapClicked();

    void exitClicked();

    void createOrEditMapClicked();
};


#endif //MAINMENU_HPP
