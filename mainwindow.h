#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "carbonanalyzer.h"
#include "footprintanalyzer.h"
#include "advisorysystem.h"
#include "databasemanager.h"

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QStackedWidget;
class QTableWidget;
class QTableWidgetItem;

// Custom chart widgets (defined in chartwidgets.h)
class DonutChart;
class TrendChart;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // Navigation
    void showDashboard();
    void showTrackEmissions();
    void showReports();
    void showSuggestions();

    // Emission calculation and form management
    void calculateEmissions();
    void clearForm();

    // Module 5: Data Management
    void deleteSelectedRecord();

private:
    // GUI setup
    void setupUI();
    void setupDashboard();
    void setupInputPage();
    void setupReportsPage();
    void setupSuggestionsPage();
    void applyStyles();

    // Module 5: Report management
    void refreshReportsTable();

    // Core C++ logic
    CarbonAnalyzer analyzer;
    FootprintAnalyzer footprintAnalyzer;
    AdvisorySystem advisorySystem;

    // Module 5: Database management
    DatabaseManager databaseManager;

    // GUI helper functions
    QWidget *createSidebar();

    QWidget *createStatCard(
        const QString &title,
        const QString &value,
        const QString &description
        );

    // Main page container
    QStackedWidget *pages;

    // Application pages
    QWidget *dashboardPage;
    QWidget *inputPage;
    QWidget *reportsPage;
    QWidget *suggestionsPage;

    // User input fields
    QLineEdit *nameInput;
    QComboBox *transportType;
    QLineEdit *distanceInput;
    QLineEdit *electricityInput;
    QComboBox *foodType;
    QLineEdit *foodServingsInput;

    // Dashboard and status labels
    QLabel *statusLabel;
    QLabel *activitiesValue;
    QLabel *emissionsValue;

    // Module 4: Smart Advisory System output
    QLabel *suggestionsLabel;

    // Module 5: Reports table
    QTableWidget *reportsTable;

    // Charts and report summary.
    // NOTE: keep this order identical to the constructor's initializer
    // list in mainwindow.cpp (avoids -Wreorder warnings).
    DonutChart *dashboardDonut;
    DonutChart *reportsDonut;
    TrendChart *reportsTrend;
    QLabel *reportsSummary;
};

#endif // MAINWINDOW_H