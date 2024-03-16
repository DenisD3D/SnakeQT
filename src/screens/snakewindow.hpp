#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

#include "editorscreen.hpp"
#include "mainmenu.hpp"
#include "gamescreen.hpp"

class SnakeWindow final : public QMainWindow {
    Q_OBJECT

protected:
    QStackedWidget *stackedWidget;
    MainMenu *mainMenu;
    GameScreen *gameScreen{};
    EditorScreen *editorScreen{};

public:
    explicit SnakeWindow(QWidget *pParent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
    void handleStartGameClicked();

    static void handleExitClicked();

    void toggleFullScreen() {
        if (windowState() & Qt::WindowFullScreen) {
            showNormal();
        } else {
            showFullScreen();
        }
    }
};

#endif
