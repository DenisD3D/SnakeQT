#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP
#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "../jeu.hpp"
#include "gamearea.hpp"
#include "../constants.hpp"

class GameScreen final : public QWidget {
    Q_OBJECT

    GameArea *gameArea;

public:
    Jeu jeu;

    explicit GameScreen(QWidget *parent = nullptr, const QString &file_info = "map1.skm");

    void handleTimer();

protected:
    void keyPressEvent(QKeyEvent *) override;
};
#endif //GAMESCREEN_HPP
