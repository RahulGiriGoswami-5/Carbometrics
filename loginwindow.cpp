#include "loginwindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), usernameInput(nullptr), passwordInput(nullptr),
    messageLabel(nullptr), loginButton(nullptr)
{
    databaseManager.openDatabase();
    databaseManager.createTables();
    setupUI();
    setWindowTitle("CarboMetrics - Login");
    setFixedSize(480, 570);
}

void LoginWindow::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(38, 30, 38, 28);
    mainLayout->setSpacing(14);

    auto *title = new QLabel("🌱 CarboMetrics");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:30px;font-weight:700;color:#218838;background:transparent;");
    auto *subtitle = new QLabel("Your Personal Carbon Footprint Tracker");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size:13px;color:#66716a;background:transparent;");

    auto *card = new QFrame;
    card->setStyleSheet("QFrame{background:#ffffff;border:1px solid #dce5dd;border-radius:14px;}");
    auto *form = new QVBoxLayout(card);
    form->setContentsMargins(26, 25, 26, 25);
    form->setSpacing(13);

    auto *heading = new QLabel("Welcome Back");
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size:22px;font-weight:700;color:#202820;background:transparent;border:0;");
    auto *userLabel = new QLabel("Username");
    userLabel->setStyleSheet("color:#303a32;background:transparent;border:0;");
    usernameInput = new QLineEdit;
    usernameInput->setPlaceholderText("Enter your username");
    usernameInput->setMinimumHeight(42);
    auto *passLabel = new QLabel("Password");
    passLabel->setStyleSheet("color:#303a32;background:transparent;border:0;");
    passwordInput = new QLineEdit;
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(42);
    messageLabel = new QLabel;
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("color:#c62828;background:transparent;border:0;");

    const QString inputStyle = "QLineEdit{color:#202820;background:#fff;border:1px solid #cbd5cc;border-radius:7px;padding:9px;} QLineEdit:focus{border:1px solid #218838;}";
    usernameInput->setStyleSheet(inputStyle);
    passwordInput->setStyleSheet(inputStyle);

    loginButton = new QPushButton("Login");
    loginButton->setMinimumHeight(44);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setStyleSheet("QPushButton{background:#218838;color:white;font-size:15px;font-weight:700;border:0;border-radius:7px;} QPushButton:hover{background:#19692c;}");
    auto *registerButton = new QPushButton("Create a new account");
    registerButton->setMinimumHeight(38);
    registerButton->setCursor(Qt::PointingHandCursor);
    registerButton->setStyleSheet("QPushButton{background:transparent;color:#218838;border:0;font-weight:600;} QPushButton:hover{text-decoration:underline;}");

    form->addWidget(heading);
    form->addWidget(userLabel);
    form->addWidget(usernameInput);
    form->addWidget(passLabel);
    form->addWidget(passwordInput);
    form->addWidget(messageLabel);
    form->addWidget(loginButton);
    form->addWidget(registerButton);

    auto *footer = new QLabel("Create your own account. Your carbon data stays separate from other users.");
    footer->setAlignment(Qt::AlignCenter);
    footer->setWordWrap(true);
    footer->setStyleSheet("font-size:11px;color:#78827a;background:transparent;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(card);
    mainLayout->addStretch();
    mainLayout->addWidget(footer);
    setStyleSheet("LoginWindow{background:#f2f7f2;}");

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::checkLogin);
    connect(passwordInput, &QLineEdit::returnPressed, this, &LoginWindow::checkLogin);
    connect(usernameInput, &QLineEdit::returnPressed, this, [this]() { passwordInput->setFocus(); });
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::showRegistration);
}

void LoginWindow::setMessage(const QString &message, bool success)
{
    messageLabel->setStyleSheet(QString("color:%1;background:transparent;border:0;").arg(success ? "#218838" : "#c62828"));
    messageLabel->setText(message);
}

void LoginWindow::checkLogin()
{
    const QString username = usernameInput->text().trimmed();
    const QString password = passwordInput->text();
    if (username.isEmpty() || password.isEmpty()) { setMessage("Please enter both username and password."); return; }

    UserProfile profile;
    if (!databaseManager.authenticateUser(username, password, &profile)) {
        setMessage(databaseManager.lastError());
        passwordInput->clear();
        passwordInput->setFocus();
        return;
    }
    emit loginSuccessful(profile.userId, profile.fullName, profile.username, profile.email);
    close();
}

void LoginWindow::showRegistration()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Create your CarboMetrics account");
    dialog.setMinimumWidth(390);
    dialog.setStyleSheet("QDialog{background:#f4f8f4;} QLabel{color:#263329;} QLineEdit{background:white;color:#202820;border:1px solid #cbd5cc;border-radius:6px;padding:8px;min-height:24px;} QPushButton{background:#218838;color:white;border:0;border-radius:6px;padding:10px 18px;font-weight:600;} QPushButton:hover{background:#19692c;}");
    auto *layout = new QVBoxLayout(&dialog);
    auto *heading = new QLabel("Create your account");
    heading->setStyleSheet("font-size:21px;font-weight:700;color:#218838;");
    auto *form = new QFormLayout;
    auto *name = new QLineEdit;
    auto *username = new QLineEdit;
    auto *email = new QLineEdit;
    auto *password = new QLineEdit;
    auto *confirm = new QLineEdit;
    password->setEchoMode(QLineEdit::Password);
    confirm->setEchoMode(QLineEdit::Password);
    name->setPlaceholderText("Your full name");
    username->setPlaceholderText("At least 3 characters");
    email->setPlaceholderText("you@example.com");
    password->setPlaceholderText("At least 8 characters");
    confirm->setPlaceholderText("Re-enter password");
    form->addRow("Full name", name);
    form->addRow("Username", username);
    form->addRow("Email", email);
    form->addRow("Password", password);
    form->addRow("Confirm password", confirm);
    auto *message = new QLabel;
    message->setWordWrap(true);
    message->setStyleSheet("color:#c62828;");
    auto *buttons = new QHBoxLayout;
    auto *cancel = new QPushButton("Cancel");
    auto *create = new QPushButton("Create account");
    buttons->addWidget(cancel);
    buttons->addWidget(create);
    layout->addWidget(heading);
    layout->addLayout(form);
    layout->addWidget(message);
    layout->addLayout(buttons);
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(create, &QPushButton::clicked, &dialog, [&]() {
        if (password->text() != confirm->text()) { message->setText("Passwords do not match."); return; }
        UserProfile profile;
        if (!databaseManager.registerUser(name->text(), username->text(), email->text(), password->text(), &profile)) {
            message->setText(databaseManager.lastError()); return;
        }
        QMessageBox::information(&dialog, "Account created", "Your account has been created. You can now log in.");
        this->usernameInput->setText(profile.username);
        this->passwordInput->clear();
        dialog.accept();
    });
    dialog.exec();
}
