#ifndef SNAKEQT_ENDGAMESCREEN_HPP
#define SNAKEQT_ENDGAMESCREEN_HPP

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "../network_manager.hpp"


class EndGameScreen final : public QWidget {
    Q_OBJECT

public:
    explicit EndGameScreen(const QString &map_name, int score, QWidget *parent = nullptr);

private slots:
    void submitName();

    void returnToMainMenu();

    void onHighscoresReceived(const QList<QPair<QString, QVariant> > &highscores);

private:
    QLabel *scoreLabel;
    QLineEdit *nameInput;
    QString map_name;
    int score;
    ScoreManager *scoreManager;
    QTableWidget *scoreTable;

signals:
    void back();

    void replayMap();
};

#endif //SNAKEQT_ENDGAMESCREEN_HPP
