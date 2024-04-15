

#ifndef SNAKEQT_ENDGAMESCREEN_HPP
#define SNAKEQT_ENDGAMESCREEN_HPP

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class EndGameScreen : public QWidget {
Q_OBJECT

public:
    explicit EndGameScreen(int score, QWidget *parent = nullptr);

private slots:
    void submitName();

private:
    QLabel *scoreLabel;
    QLineEdit *nameLineEdit;
};

#endif //SNAKEQT_ENDGAMESCREEN_HPP
