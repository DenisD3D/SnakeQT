#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#include <QPushButton>
#include <QVBoxLayout>

class MainMenu final : public QWidget {
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr) : QWidget(parent) {
        const auto layout = new QVBoxLayout(this);

        const auto startButton = new QPushButton("Start Game", this);
        const auto exitButton = new QPushButton("Exit", this);

        layout->addWidget(startButton);
        layout->addWidget(exitButton);

        connect(startButton, &QPushButton::clicked, this, &MainMenu::startGameClicked);
        connect(exitButton, &QPushButton::clicked, this, &MainMenu::exitClicked);
    }

signals:
    void startGameClicked();

    void exitClicked();
};


#endif //MAINMENU_HPP
