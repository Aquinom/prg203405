#include <QApplication>
#include "gamewindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Use new para garantir que a instância não morra ao fim do escopo
    GameWindow *window = new GameWindow;
    window->show();

    return app.exec();
}
