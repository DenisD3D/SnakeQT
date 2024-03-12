#include "snakewindow.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SnakeWindow wnd;
    wnd.show();

    return QApplication::exec();
}
