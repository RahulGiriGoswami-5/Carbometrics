#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QFrame>
#include <QScrollArea>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QIntValidator>
#include <QFont>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QDate>
#include "transportactivity.h"
#include "electricityactivity.h"
#include "foodactivity.h"

#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    pages(nullptr),
    dashboardPage(nullptr),
    inputPage(nullptr),
    reportsPage(nullptr),
    suggestionsPage(nullptr),
    nameInput(nullptr),
    transportType(nullptr),
    distanceInput(nullptr),
    electricityInput(nullptr),
    foodType(nullptr),
    foodServingsInput(nullptr),
    statusLabel(nullptr),
    activitiesValue(nullptr)
{
    setupUI();
    applyStyles();
}

void MainWindow::setupUI()
{
    setWindowTitle("CarboMetrics | Carbon Footprint Tracker");
    resize(1200, 760);
    setMinimumSize(950, 650);

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *sidebar = createSidebar();

    pages = new QStackedWidget(central);

    dashboardPage = new QWidget();
    inputPage = new QWidget();
    reportsPage = new QWidget();
    suggestionsPage = new QWidget();

    setupDashboard();
    setupInputPage();
    setupReportsPage();
    setupSuggestionsPage();

    pages->addWidget(dashboardPage);
    pages->addWidget(inputPage);
    pages->addWidget(reportsPage);
    pages->addWidget(suggestionsPage);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(pages, 1);

    setCentralWidget(central);

    pages->setCurrentWidget(dashboardPage);
}

QWidget *MainWindow::createSidebar()
{
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(225);

    QVBoxLayout *layout = new QVBoxLayout(sidebar);

    layout->setContentsMargins(18, 25, 18, 20);
    layout->setSpacing(12);

    QLabel *logo = new QLabel("🌱 CarboMetrics");
    logo->setObjectName("logo");

    QLabel *subtitle = new QLabel("SMART CARBON TRACKING");
    subtitle->setObjectName("sidebarSubtitle");

    layout->addWidget(logo);
    layout->addWidget(subtitle);
    layout->addSpacing(35);

    auto addNavButton = [&](const QString &text,
                            const QString &objectName,
                            auto slot)
    {
        QPushButton *button = new QPushButton(text);

        button->setObjectName(objectName);
        button->setMinimumHeight(45);
        button->setCursor(Qt::PointingHandCursor);

        layout->addWidget(button);

        connect(button, &QPushButton::clicked, this, slot);
    };

    addNavButton("⌂  Dashboard", "navButton",
                 &MainWindow::showDashboard);

    addNavButton("＋  Track Emissions", "navButton",
                 &MainWindow::showTrackEmissions);

    addNavButton("▤  My Reports", "navButton",
                 &MainWindow::showReports);

    addNavButton("✦  Smart Suggestions", "navButton",
                 &MainWindow::showSuggestions);

    layout->addStretch();

    QLabel *footer = new QLabel(
        "Small steps today,\nbetter planet tomorrow."
        );

    footer->setObjectName("sidebarFooter");
    footer->setWordWrap(true);

    layout->addWidget(footer);

    return sidebar;
}

QWidget *MainWindow::createStatCard(
    const QString &title,
    const QString &value,
    const QString &description)
{
    QFrame *card = new QFrame();
    card->setObjectName("statCard");

    QVBoxLayout *layout = new QVBoxLayout(card);

    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(10);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("cardTitle");

    QLabel *valueLabel = new QLabel(value);
    valueLabel->setObjectName("cardValue");

    QLabel *descriptionLabel = new QLabel(description);
    descriptionLabel->setObjectName("cardDescription");

    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);
    layout->addWidget(descriptionLabel);

    return card;
}

void MainWindow::setupDashboard()
{
    QVBoxLayout *layout = new QVBoxLayout(dashboardPage);

    layout->setContentsMargins(35, 30, 35, 30);
    layout->setSpacing(20);

    QLabel *heading = new QLabel("Dashboard");
    heading->setObjectName("pageTitle");

    QLabel *welcome = new QLabel(
        "Welcome to CarboMetrics. Track your daily carbon footprint."
        );

    welcome->setObjectName("pageSubtitle");
    welcome->setWordWrap(true);

    layout->addWidget(heading);
    layout->addWidget(welcome);

    QHBoxLayout *cards = new QHBoxLayout();

    cards->addWidget(createStatCard(
        "Today's Emissions",
        "-- kg CO2e",
        "Calculated in Module 2"
        ));

    cards->addWidget(createStatCard(
        "Daily Goal",
        "2.0 kg CO2e",
        "Illustrative target"
        ));

    QWidget *activityCard = createStatCard(
        "Activities Tracked",
        "0",
        "Activities entered today"
        );

    activitiesValue = activityCard->findChild<QLabel *>(
        "cardValue"
        );

    cards->addWidget(activityCard);

    layout->addLayout(cards);

    QFrame *welcomeCard = new QFrame();
    welcomeCard->setObjectName("contentCard");

    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeCard);

    QLabel *sectionTitle = new QLabel("Start Your Sustainability Journey");
    sectionTitle->setObjectName("sectionTitle");

    QLabel *description = new QLabel(
        "Record your daily activities to understand their "
        "environmental impact. CarboMetrics will help you "
        "track your habits and discover opportunities to "
        "reduce your carbon footprint."
        );

    description->setObjectName("bodyText");
    description->setWordWrap(true);

    QPushButton *startButton = new QPushButton("Track Today's Activities");
    startButton->setObjectName("primaryButton");
    startButton->setMinimumHeight(45);

    connect(startButton, &QPushButton::clicked,
            this, &MainWindow::showTrackEmissions);

    welcomeLayout->addWidget(sectionTitle);
    welcomeLayout->addWidget(description);
    welcomeLayout->addSpacing(10);
    welcomeLayout->addWidget(startButton, 0, Qt::AlignLeft);

    layout->addWidget(welcomeCard);
    layout->addStretch();
}

void MainWindow::setupInputPage()
{
    QVBoxLayout *pageLayout = new QVBoxLayout(inputPage);

    pageLayout->setContentsMargins(35, 25, 35, 25);
    pageLayout->setSpacing(16);

    QLabel *heading = new QLabel("Track Your Emissions");
    heading->setObjectName("pageTitle");

    QLabel *subtitle = new QLabel(
        "Enter your details and daily activities below."
        );

    subtitle->setObjectName("pageSubtitle");

    pageLayout->addWidget(heading);
    pageLayout->addWidget(subtitle);

    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *formContainer = new QWidget();

    QVBoxLayout *containerLayout = new QVBoxLayout(formContainer);
    containerLayout->setContentsMargins(2, 2, 10, 10);
    containerLayout->setSpacing(18);

    // USER DETAILS

    QFrame *userCard = new QFrame();
    userCard->setObjectName("contentCard");

    QVBoxLayout *userLayout = new QVBoxLayout(userCard);

    QLabel *userTitle = new QLabel("1. User Details");
    userTitle->setObjectName("sectionTitle");

    QFormLayout *userForm = new QFormLayout();
    userForm->setSpacing(12);

    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter your name");
    nameInput->setMaxLength(60);

    userForm->addRow("Name:", nameInput);

    userLayout->addWidget(userTitle);
    userLayout->addLayout(userForm);

    containerLayout->addWidget(userCard);

    // TRANSPORTATION

    QFrame *transportCard = new QFrame();
    transportCard->setObjectName("contentCard");

    QVBoxLayout *transportLayout = new QVBoxLayout(transportCard);

    QLabel *transportTitle = new QLabel("2. Transportation");
    transportTitle->setObjectName("sectionTitle");

    QFormLayout *transportForm = new QFormLayout();
    transportForm->setSpacing(12);

    transportType = new QComboBox();

    transportType->addItems({
        "Car",
        "Motorcycle",
        "Bus",
        "Train",
        "Electric Vehicle",
        "Bicycle",
        "Walking"
    });

    distanceInput = new QLineEdit();
    distanceInput->setPlaceholderText("Distance travelled in km");
    distanceInput->setValidator(
        new QDoubleValidator(0.0, 100000.0, 2, distanceInput)
        );

    transportForm->addRow("Transport Type:", transportType);
    transportForm->addRow("Distance (km):", distanceInput);

    transportLayout->addWidget(transportTitle);
    transportLayout->addLayout(transportForm);

    containerLayout->addWidget(transportCard);

    // ELECTRICITY

    QFrame *electricityCard = new QFrame();
    electricityCard->setObjectName("contentCard");

    QVBoxLayout *electricityLayout = new QVBoxLayout(electricityCard);

    QLabel *electricityTitle = new QLabel("3. Electricity Consumption");
    electricityTitle->setObjectName("sectionTitle");

    QFormLayout *electricityForm = new QFormLayout();
    electricityForm->setSpacing(12);

    electricityInput = new QLineEdit();

    electricityInput->setPlaceholderText(
        "Electricity used in kWh"
        );

    electricityInput->setValidator(
        new QDoubleValidator(0.0, 100000.0, 2, electricityInput)
        );

    electricityForm->addRow(
        "Electricity (kWh):",
        electricityInput
        );

    electricityLayout->addWidget(electricityTitle);
    electricityLayout->addLayout(electricityForm);

    containerLayout->addWidget(electricityCard);

    // FOOD

    QFrame *foodCard = new QFrame();
    foodCard->setObjectName("contentCard");

    QVBoxLayout *foodLayout = new QVBoxLayout(foodCard);

    QLabel *foodTitle = new QLabel("4. Food Consumption");
    foodTitle->setObjectName("sectionTitle");

    QFormLayout *foodForm = new QFormLayout();
    foodForm->setSpacing(12);

    foodType = new QComboBox();

    foodType->addItems({
        "Plant-based",
        "Vegetarian",
        "Mixed Diet",
        "Meat-heavy"
    });

    foodServingsInput = new QLineEdit();

    foodServingsInput->setPlaceholderText(
        "Number of servings today"
        );

    foodServingsInput->setValidator(
        new QIntValidator(0, 1000, foodServingsInput)
        );

    foodForm->addRow("Diet Category:", foodType);
    foodForm->addRow("Servings:", foodServingsInput);

    foodLayout->addWidget(foodTitle);
    foodLayout->addLayout(foodForm);

    containerLayout->addWidget(foodCard);

    scroll->setWidget(formContainer);

    pageLayout->addWidget(scroll, 1);

    // ACTIONS

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *clearButton = new QPushButton("Clear Form");
    clearButton->setObjectName("secondaryButton");

    QPushButton *calculateButton = new QPushButton(
        "Validate & Continue"
        );

    calculateButton->setObjectName("primaryButton");

    clearButton->setMinimumHeight(45);
    calculateButton->setMinimumHeight(45);

    connect(clearButton, &QPushButton::clicked,
            this, &MainWindow::clearForm);

    connect(calculateButton, &QPushButton::clicked,
            this, &MainWindow::calculateEmissions);

    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(calculateButton);

    pageLayout->addLayout(buttonLayout);

    statusLabel = new QLabel();
    statusLabel->setObjectName("statusLabel");
    statusLabel->setWordWrap(true);

    pageLayout->addWidget(statusLabel);
}

void MainWindow::setupSuggestionsPage()
{
    QVBoxLayout *layout = new QVBoxLayout(suggestionsPage);
    layout->setContentsMargins(35, 30, 35, 30);

    QLabel *heading = new QLabel("Smart Suggestions");
    heading->setObjectName("pageTitle");

    QLabel *message = new QLabel(
        "Personalized carbon-reduction recommendations will be "
        "generated in Module 4.");
    message->setObjectName("bodyText");
    message->setWordWrap(true);

    layout->addWidget(heading);
    layout->addWidget(message);
    layout->addStretch();
}

void MainWindow::setupReportsPage()
{
    QVBoxLayout *layout = new QVBoxLayout(reportsPage);

    layout->setContentsMargins(35, 30, 35, 30);

    QLabel *heading = new QLabel("My Reports");
    heading->setObjectName("pageTitle");

    QLabel *message = new QLabel(
        "Your emission history and reports will appear here "
        "after Data Management is implemented in Module 5."
        );

    message->setObjectName("bodyText");
    message->setWordWrap(true);

    layout->addWidget(heading);
    layout->addWidget(message);
    layout->addStretch();
}

void MainWindow::calculateEmissions()
{
    // Validate the user's name.
    const QString name = nameInput->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Please enter your name.");
        nameInput->setFocus();
        return;
    }

    // Read inputs. Empty activity fields represent zero activity.
    const QString distanceText = distanceInput->text().trimmed();
    const QString electricityText = electricityInput->text().trimmed();
    const QString servingsText = foodServingsInput->text().trimmed();

    bool distanceOk = distanceText.isEmpty();
    bool electricityOk = electricityText.isEmpty();
    bool servingsOk = servingsText.isEmpty();

    const double distance = distanceText.isEmpty()
                                ? 0.0 : distanceText.toDouble(&distanceOk);
    const double electricity = electricityText.isEmpty()
                                   ? 0.0 : electricityText.toDouble(&electricityOk);
    const int servings = servingsText.isEmpty()
                             ? 0 : servingsText.toInt(&servingsOk);

    if (!distanceOk || distance < 0.0 || distance > 100000.0) {
        QMessageBox::warning(this, "Invalid Distance",
                             "Enter a distance between 0 and 100000 km.");
        distanceInput->setFocus();
        return;
    }

    if (!electricityOk || electricity < 0.0 || electricity > 100000.0) {
        QMessageBox::warning(this, "Invalid Electricity Usage",
                             "Enter electricity usage between 0 and 100000 kWh.");
        electricityInput->setFocus();
        return;
    }

    if (!servingsOk || servings < 0 || servings > 1000) {
        QMessageBox::warning(this, "Invalid Servings",
                             "Enter a number of servings between 0 and 1000.");
        foodServingsInput->setFocus();
        return;
    }

    // Start a fresh calculation for the current form values.
    analyzer.clearActivities();
    int activityCount = 0;

    // Illustrative factors for development; replace with documented factors
    // before presenting the values as real-world estimates.
    if (distance > 0.0) {
        const QString selectedTransport = transportType->currentText();
        double transportFactor = 0.0;

        if (selectedTransport == "Car")
            transportFactor = 0.15;
        else if (selectedTransport == "Motorcycle")
            transportFactor = 0.10;
        else if (selectedTransport == "Bus")
            transportFactor = 0.08;
        else if (selectedTransport == "Train")
            transportFactor = 0.04;
        else if (selectedTransport == "Electric Vehicle")
            transportFactor = 0.05;
        // Bicycle and Walking remain at 0.0 in this illustrative model.

        analyzer.addActivity(std::make_unique<TransportActivity>(
            selectedTransport, distance, transportFactor));
        ++activityCount;
    }

    if (electricity > 0.0) {
        constexpr double electricityFactor = 0.70; // illustrative kg CO2e/kWh

        analyzer.addActivity(std::make_unique<ElectricityActivity>(
            "Electricity Consumption", electricity, electricityFactor));
        ++activityCount;
    }

    if (servings > 0) {
        const QString selectedDiet = foodType->currentText();
        double foodFactor = 0.0;

        if (selectedDiet == "Plant-based")
            foodFactor = 0.50;
        else if (selectedDiet == "Vegetarian")
            foodFactor = 0.80;
        else if (selectedDiet == "Mixed Diet")
            foodFactor = 1.50;
        else if (selectedDiet == "Meat-heavy")
            foodFactor = 2.50;

        analyzer.addActivity(std::make_unique<FoodActivity>(
            "Food Consumption", selectedDiet, servings, foodFactor));
        ++activityCount;
    }

    // CarbonAnalyzer calls calculateEmission() through Activity pointers,
    // demonstrating runtime polymorphism.
    const double totalEmissions = analyzer.calculateTotalEmissions();

    if (activitiesValue != nullptr)
        activitiesValue->setText(QString::number(activityCount));

    statusLabel->setText(
        QString("Total Carbon Emissions: %1 kg CO2e")
            .arg(totalEmissions, 0, 'f', 2));

    QMessageBox::information(
        this,
        "Carbon Emission Results",
        QString("Calculation complete!\n\n"
                "User: %1\n"
                "Transport: %2\n"
                "Distance: %3 km\n"
                "Electricity: %4 kWh\n"
                "Diet category: %5\n"
                "Servings: %6\n"
                "Activities recorded: %7\n\n"
                "Total carbon emissions: %8 kg CO2e\n\n"
                "Emission factors are illustrative development values.")
            .arg(name)
            .arg(transportType->currentText())
            .arg(distance, 0, 'f', 2)
            .arg(electricity, 0, 'f', 2)
            .arg(foodType->currentText())
            .arg(servings)
            .arg(activityCount)
            .arg(totalEmissions, 0, 'f', 2));
}

void MainWindow::clearForm()
{
    nameInput->clear();
    distanceInput->clear();
    electricityInput->clear();
    foodServingsInput->clear();

    transportType->setCurrentIndex(0);
    foodType->setCurrentIndex(0);

    statusLabel->clear();

    nameInput->setFocus();
}

void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background-color: #F4F7F3;
        }

        QWidget {
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 10pt;
            color: #26352D;
        }

        QWidget#sidebar {
            background-color: #173F32;
        }

        QLabel#logo {
            color: white;
            font-size: 19pt;
            font-weight: bold;
        }

        QLabel#sidebarSubtitle {
            color: #B8D5C3;
            font-size: 8pt;
            letter-spacing: 1px;
        }

        QPushButton#navButton {
            background-color: transparent;
            color: #E5F0E8;
            border: none;
            border-radius: 7px;
            padding: 10px;
            text-align: left;
        }

        QPushButton#navButton:hover {
            background-color: #2B5948;
        }

        QLabel#sidebarFooter {
            color: #B8D5C3;
            font-size: 10pt;
            padding: 8px;
        }

        QLabel#pageTitle {
            color: #173F32;
            font-size: 25pt;
            font-weight: bold;
        }

        QLabel#pageSubtitle {
            color: #718078;
            font-size: 11pt;
        }

        QFrame#statCard,
        QFrame#contentCard {
            background-color: white;
            border: 1px solid #E1E9E1;
            border-radius: 12px;
        }

        QLabel#cardTitle {
            color: #718078;
            font-size: 10pt;
        }

        QLabel#cardValue {
            color: #1E6548;
            font-size: 21pt;
            font-weight: bold;
        }

        QLabel#cardDescription {
            color: #87938B;
            font-size: 9pt;
        }

        QLabel#sectionTitle {
            color: #1D563E;
            font-size: 13pt;
            font-weight: bold;
        }

        QLabel#bodyText {
            color: #596A60;
            font-size: 11pt;
        }

        QLineEdit,
        QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #D6E0D7;
            border-radius: 6px;
            padding: 10px;
            min-height: 20px;
        }

        QLineEdit:focus,
        QComboBox:focus {
            border: 1px solid #27845A;
        }

        QPushButton#primaryButton {
            background-color: #23764F;
            color: white;
            border: none;
            border-radius: 7px;
            padding: 10px 20px;
            font-weight: bold;
        }

        QPushButton#primaryButton:hover {
            background-color: #185D3D;
        }

        QPushButton#secondaryButton {
            background-color: white;
            color: #23764F;
            border: 1px solid #23764F;
            border-radius: 7px;
            padding: 10px 20px;
        }

        QPushButton#secondaryButton:hover {
            background-color: #EAF4EC;
        }

        QLabel#statusLabel {
            color: #23764F;
            padding: 4px;
        }

        QScrollArea {
            background: transparent;
        }

        QScrollArea QWidget {
            background-color: transparent;
        }
        QMessageBox {
            background-color: #FFFFFF;
        }

        QMessageBox QLabel {
            color: #26352D;
            background-color: #FFFFFF;
        }

        QMessageBox QPushButton {
            background-color: #23764F;
            color: #FFFFFF;
            border: 1px solid #23764F;
            border-radius: 5px;
            padding: 8px 20px;
            min-width: 70px;
        }

        QMessageBox QPushButton:hover {
            background-color: #185D3D;
        }
    )");
}
// Navigate to the Dashboard page
void MainWindow::showDashboard()
{
    pages->setCurrentWidget(dashboardPage);
}

// Navigate to the Track Emissions page
void MainWindow::showTrackEmissions()
{
    pages->setCurrentWidget(inputPage);
}

// Navigate to the Reports page
void MainWindow::showReports()
{
    pages->setCurrentWidget(reportsPage);
}

// Navigate to the Smart Suggestions page
void MainWindow::showSuggestions()
{
    pages->setCurrentWidget(suggestionsPage);
}