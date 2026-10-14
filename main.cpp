#include "mainwindow.h"
#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("CarboMetrics");
    QApplication::setOrganizationName("CarboMetrics");

    MainWindow *mainWindow = nullptr;
    LoginWindow loginWindow;

    QObject::connect(
        &loginWindow,
        &LoginWindow::loginSuccessful,
        &app,
        [&]() {
            mainWindow = new MainWindow();
            mainWindow->setAttribute(Qt::WA_DeleteOnClose);
            mainWindow->show();
        }
        );

    QObject::connect(
        &app,
        &QApplication::aboutToQuit,
        [&]() {
            delete mainWindow;
            mainWindow = nullptr;
        }
        );

    loginWindow.show();

    return app.exec();
}