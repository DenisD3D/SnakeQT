#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "browsemapscreen.hpp"
#include "editorscreen.hpp"
#include "mainmenu.hpp"
#include "gamescreen.hpp"
#include "endgamescreen.hpp"

class SnakeWindow final : public QMainWindow {
    Q_OBJECT

protected:
    QStackedWidget *stackedWidget;
    MainMenu *mainMenu;
    GameScreen *gameScreen{};
    BrowseMapScreen *browseMapScreen{};
    EditorScreen *editorScreen{};
    EndGameScreen *endGameScreen{};

public:
    explicit SnakeWindow(QWidget *pParent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
    void handlePlayMapClicked();

    void handleCreateOrEditMapClicked();

    void handleGameOver(int score);

    static void handleExitClicked();

    void returnToMainMenu();

    void toggleFullScreen() {
        if (windowState() & Qt::WindowFullScreen) {
            showNormal();
        } else {
            showFullScreen();
        }
    }
};

#endif
