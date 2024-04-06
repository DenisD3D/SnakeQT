

#ifndef SNAKEQT_ENDGAMESCREEN_HPP
#define SNAKEQT_ENDGAMESCREEN_HPP

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class EndGameScreen : public QWidget {
Q_OBJECT

public:
    explicit EndGameScreen(int score, QWidget *parent = nullptr);

private:
    QLabel *scoreLabel;
};

#endif //SNAKEQT_ENDGAMESCREEN_HPP
