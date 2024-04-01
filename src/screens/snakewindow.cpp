#include "snakewindow.hpp"

#include <iostream>

#include "editorscreen.hpp"

using namespace std;

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags)
    : QMainWindow(pParent, flags) {
    stackedWidget = new QStackedWidget(this);

    // Create the main menu and connect its signals to the appropriate slots
    mainMenu = new MainMenu(this);

    connect(mainMenu, &MainMenu::startGameClicked, this, &SnakeWindow::handleStartGameClicked);
    connect(mainMenu, &MainMenu::browseMapClicked, this, &SnakeWindow::handleBrowseMapClicked);
    connect(mainMenu, &MainMenu::exitClicked, this, &SnakeWindow::handleExitClicked);

    stackedWidget->addWidget(mainMenu);


    setCentralWidget(stackedWidget);

    // Create a menu bar
    const auto menuBar = new QMenuBar;

    const auto fileMenu = new QMenu(tr("&File"), this);
    const QAction *mainMenuAction = fileMenu->addAction(tr("&Main Menu"));
    const QAction *fullScreenAction = fileMenu->addAction(tr("Full &Screen"));
    const QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    connect(mainMenuAction, &QAction::triggered, this, [this] {
        stackedWidget->setCurrentWidget(mainMenu);
    });
    connect(fullScreenAction, &QAction::triggered, this, &SnakeWindow::toggleFullScreen);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);


    const auto playMenu = new QMenu(tr("&Play"), this);
    const QAction *openMapPlayAction = playMenu->addAction(tr("&Open Map"));
    menuBar->addMenu(playMenu);

    // Open the file dialog when the "Open Map" button is clicked and launch the game with the selected map
    connect(openMapPlayAction, &QAction::triggered, this, [this] {
        const auto fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), QString(), tr("Map Files (*.skm)"));
        if (!fileName.isEmpty()) {
            qDebug() << "Opening map: " << fileName;
            gameScreen = new GameScreen(this, fileName);
            stackedWidget->addWidget(gameScreen);
            stackedWidget->setCurrentWidget(gameScreen);
        }
    });

    const auto editorMenu = new QMenu(tr("&Editor"), this);
    const QAction *newMapEditorAction = editorMenu->addAction(tr("&New Map"));
    const QAction *openMapEditorAction = editorMenu->addAction(tr("&Open Map"));
    menuBar->addMenu(editorMenu);

    // Prompt for save location and launch the editor with a new map when the "New Map" button is clicked
    connect(newMapEditorAction, &QAction::triggered, this, [this] {
        auto fileName = QFileDialog::getSaveFileName(this, tr("New Map"), QString(), tr("Map Files (*.skm)"));
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".skm"))
                fileName += ".skm";

            editorScreen = new EditorScreen(fileName, true, this);
            stackedWidget->addWidget(editorScreen);
            stackedWidget->setCurrentWidget(editorScreen);
        }
    });

    // Open the file dialog when the "Open Map" button is clicked and launch the editor with the selected map
    connect(openMapEditorAction, &QAction::triggered, this, [this] {
        const auto fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), QString(), tr("Map Files (*.skm)"));
        if (!fileName.isEmpty()) {
            std::cout << "Opening map: " << fileName.toStdString() << std::endl;
            editorScreen = new EditorScreen(fileName, false, this);
            stackedWidget->addWidget(editorScreen);
            stackedWidget->setCurrentWidget(editorScreen);
        }
    });

    // Set the menu bar
    setMenuBar(menuBar);
}

void SnakeWindow::handleStartGameClicked() {
    // Initialize and switch to the game screen when the "Start Game" button is clicked
    gameScreen = new GameScreen(this);
    stackedWidget->addWidget(gameScreen);
    stackedWidget->setCurrentWidget(gameScreen);
}

void SnakeWindow::handleBrowseMapClicked() {
    browseMapScreen = new BrowseMapScreen(this);
    stackedWidget->addWidget(browseMapScreen);
    stackedWidget->setCurrentWidget(browseMapScreen);

    connect(browseMapScreen, &BrowseMapScreen::back, this, [this] {
        stackedWidget->setCurrentWidget(mainMenu);
    });

    connect(browseMapScreen, &BrowseMapScreen::load, this, [this](const QString &fileName) {
        gameScreen = new GameScreen(this, fileName);
        stackedWidget->addWidget(gameScreen);
        stackedWidget->setCurrentWidget(gameScreen);
    });
}

void SnakeWindow::handleExitClicked() {
    // Exit the application when the "Exit" button is clicked
    QApplication::quit();
}
