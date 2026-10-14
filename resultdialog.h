#pragma once
#include <QDialog>
#include <QString>
#include <QStringList>

struct ResultData
{
    QString userName;
    QString transportType;
    QString dietCategory;
    QString category;          // Low / Moderate / High
    QString analysis;          // text from FootprintAnalyzer::getAnalysis
    QStringList recommendations; // from AdvisorySystem

    double distance = 0.0;
    double electricity = 0.0;
    double transportEmissions = 0.0;
    double electricityEmissions = 0.0;
    double foodEmissions = 0.0;
    double total = 0.0;
    int servings = 0;
    int activityCount = 0;
    bool saved = true;
};

class ResultDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResultDialog(const ResultData &data, QWidget *parent = nullptr);
};