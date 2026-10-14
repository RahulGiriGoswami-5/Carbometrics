#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("CarboMetrics");
    QApplication::setOrganizationName("CarboMetrics");

    MainWindow window;

    window.show();

    return app.exec();
}