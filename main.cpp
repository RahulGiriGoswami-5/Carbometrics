#include "mainwindow.h"
#include "loginwindow.h"

#include <QApplication>
#include <QTimer>

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
        [&](int userId, const QString &fullName, const QString &username, const QString &email) {
            UserProfile authenticatedProfile;
            authenticatedProfile.userId = userId;
            authenticatedProfile.fullName = fullName;
            authenticatedProfile.username = username;
            authenticatedProfile.email = email;
            mainWindow = new MainWindow(authenticatedProfile);
            QObject::connect(mainWindow, &MainWindow::logoutRequested, &app, [&]() {
                QTimer::singleShot(0, &app, [&]() {
                    delete mainWindow;
                    mainWindow = nullptr;
                    loginWindow.show();
                    loginWindow.raise();
                    loginWindow.activateWindow();
                });
            });
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