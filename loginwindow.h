#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful();

private slots:
    void checkLogin();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLabel *messageLabel;
    QPushButton *loginButton;

    void setupUI();
};

#endif // LOGINWINDOW_H