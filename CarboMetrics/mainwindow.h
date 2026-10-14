#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "carbonanalyzer.h"

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void showDashboard();
    void showTrackEmissions();
    void showReports();
    void showSuggestions();

    void calculateEmissions();
    void clearForm();

private:
    void setupUI();
    void setupDashboard();
    void setupInputPage();
    void setupReportsPage();
    void setupSuggestionsPage();
    void applyStyles();
    CarbonAnalyzer analyzer;


    QWidget *createSidebar();
    QWidget *createStatCard(
        const QString &title,
        const QString &value,
        const QString &description
        );

    QStackedWidget *pages;

    QWidget *dashboardPage;
    QWidget *inputPage;
    QWidget *reportsPage;
    QWidget *suggestionsPage;

    QLineEdit *nameInput;
    QComboBox *transportType;
    QLineEdit *distanceInput;
    QLineEdit *electricityInput;
    QComboBox *foodType;
    QLineEdit *foodServingsInput;

    QLabel *statusLabel;
    QLabel *activitiesValue;
    QLabel *emissionsValue;
};

#endif // MAINWINDOW_H