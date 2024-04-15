

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


class EndGameScreen final : public QWidget {
Q_OBJECT

public:
    explicit EndGameScreen(int score, QWidget *parent = nullptr);

private slots:
    void submitName();

private:
    QLabel *scoreLabel;
    QLineEdit *nameInput;
};

#endif //SNAKEQT_ENDGAMESCREEN_HPP
