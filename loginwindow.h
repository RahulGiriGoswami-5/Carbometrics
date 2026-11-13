#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "databasemanager.h"

class QLineEdit;
class QPushButton;
class QLabel;

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful(int userId, const QString &fullName,
                         const QString &username, const QString &email);

private slots:
    void checkLogin();
    void showRegistration();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLabel *messageLabel;
    QPushButton *loginButton;
    DatabaseManager databaseManager;
    void setupUI();
    void setMessage(const QString &message, bool success = false);
};

#endif
