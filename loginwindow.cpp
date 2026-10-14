#include "loginwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QApplication>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent),
    usernameInput(nullptr),
    passwordInput(nullptr),
    messageLabel(nullptr),
    loginButton(nullptr)
{
    setupUI();

    setWindowTitle("CarboMetrics - Login");
    setFixedSize(480, 520);
}

void LoginWindow::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(35, 30, 35, 30);
    mainLayout->setSpacing(15);

    // Application title
    QLabel *title = new QLabel("CarboMetrics");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 30px;"
        "font-weight: bold;"
        "color: #218838;"
        "background-color: transparent;"
        );

    // Subtitle
    QLabel *subtitle = new QLabel(
        "Your Personal Carbon Footprint Tracker"
        );
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "background-color: transparent;"
        );

    // Login card
    QFrame *card = new QFrame;
    card->setStyleSheet(
        "QFrame {"
        "background-color: #ffffff;"
        "border: 1px solid #dddddd;"
        "border-radius: 12px;"
        "}"
        );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(25, 25, 25, 25);
    cardLayout->setSpacing(15);

    // Login heading
    QLabel *loginTitle = new QLabel("Welcome Back");
    loginTitle->setAlignment(Qt::AlignCenter);
    loginTitle->setStyleSheet(
        "font-size: 21px;"
        "font-weight: bold;"
        "color: #222222;"
        "background-color: transparent;"
        "border: none;"
        );

    // Username label
    QLabel *usernameLabel = new QLabel("Username");
    usernameLabel->setStyleSheet(
        "color: #333333;"
        "background-color: transparent;"
        "border: none;"
        );

    // Username input
    usernameInput = new QLineEdit;
    usernameInput->setPlaceholderText("Enter your username");
    usernameInput->setMinimumHeight(40);
    usernameInput->setStyleSheet(
        "QLineEdit {"
        "color: #222222;"
        "background-color: #ffffff;"
        "border: 1px solid #cccccc;"
        "border-radius: 6px;"
        "padding: 8px;"
        "selection-color: #ffffff;"
        "selection-background-color: #218838;"
        "}"
        "QLineEdit:focus {"
        "color: #222222;"
        "background-color: #ffffff;"
        "border: 1px solid #218838;"
        "}"
        "QLineEdit::placeholder {"
        "color: #888888;"
        "}"
        );

    // Password label
    QLabel *passwordLabel = new QLabel("Password");
    passwordLabel->setStyleSheet(
        "color: #333333;"
        "background-color: transparent;"
        "border: none;"
        );

    // Password input
    passwordInput = new QLineEdit;
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(40);
    passwordInput->setStyleSheet(
        "QLineEdit {"
        "color: #222222;"
        "background-color: #ffffff;"
        "border: 1px solid #cccccc;"
        "border-radius: 6px;"
        "padding: 8px;"
        "selection-color: #ffffff;"
        "selection-background-color: #218838;"
        "}"
        "QLineEdit:focus {"
        "color: #222222;"
        "background-color: #ffffff;"
        "border: 1px solid #218838;"
        "}"
        "QLineEdit::placeholder {"
        "color: #888888;"
        "}"
        );

    // Error and status messages
    messageLabel = new QLabel;
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet(
        "color: #d32f2f;"
        "background-color: transparent;"
        "border: none;"
        );

    // Login button
    loginButton = new QPushButton("Login");
    loginButton->setMinimumHeight(43);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setStyleSheet(
        "QPushButton {"
        "background-color: #218838;"
        "color: #ffffff;"
        "font-size: 15px;"
        "font-weight: bold;"
        "border: none;"
        "border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "background-color: #19692c;"
        "}"
        "QPushButton:pressed {"
        "background-color: #145523;"
        "}"
        );

    // Add widgets to the login card
    cardLayout->addWidget(loginTitle);
    cardLayout->addWidget(usernameLabel);
    cardLayout->addWidget(usernameInput);
    cardLayout->addWidget(passwordLabel);
    cardLayout->addWidget(passwordInput);
    cardLayout->addWidget(messageLabel);
    cardLayout->addWidget(loginButton);

    // Footer
    QLabel *footer = new QLabel(
        "Make every action count towards a greener planet."
        );
    footer->setAlignment(Qt::AlignCenter);
    footer->setWordWrap(true);
    footer->setStyleSheet(
        "color: #777777;"
        "font-size: 11px;"
        "background-color: transparent;"
        );

    // Assemble the main layout
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(card);
    mainLayout->addStretch();
    mainLayout->addWidget(footer);

    // Window background
    setStyleSheet(
        "LoginWindow {"
        "background-color: #f3f7f3;"
        "}"
        );

    // Button click performs login
    connect(
        loginButton,
        &QPushButton::clicked,
        this,
        &LoginWindow::checkLogin
        );

    // Pressing Enter in the password field performs login
    connect(
        passwordInput,
        &QLineEdit::returnPressed,
        this,
        &LoginWindow::checkLogin
        );

    // Pressing Enter in the username field moves to password
    connect(
        usernameInput,
        &QLineEdit::returnPressed,
        this,
        [this]() {
            passwordInput->setFocus();
        }
        );
}

void LoginWindow::checkLogin()
{
    const QString username = usernameInput->text().trimmed();
    const QString password = passwordInput->text();

    // Sample demonstration credentials
    const QString correctUsername = "carbonuser";
    const QString correctPassword = "Carbon@2026#Eco";

    // Check for empty fields
    if (username.isEmpty() || password.isEmpty()) {
        messageLabel->setStyleSheet(
            "color: #d32f2f;"
            "background-color: transparent;"
            "border: none;"
            );

        messageLabel->setText(
            "Please enter both username and password."
            );

        return;
    }

    // Verify credentials
    if (username == correctUsername &&
        password == correctPassword) {

        messageLabel->setStyleSheet(
            "color: #218838;"
            "background-color: transparent;"
            "border: none;"
            );

        messageLabel->setText("Login successful!");

        emit loginSuccessful();

        close();

    } else {
        messageLabel->setStyleSheet(
            "color: #d32f2f;"
            "background-color: transparent;"
            "border: none;"
            );

        messageLabel->setText(
            "Incorrect username or password. Please try again."
            );

        passwordInput->clear();
        passwordInput->setFocus();
    }
}