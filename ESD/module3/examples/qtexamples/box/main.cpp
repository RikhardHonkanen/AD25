#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    qDebug() << "Qt version:" << QT_VERSION_STR;

    QApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}