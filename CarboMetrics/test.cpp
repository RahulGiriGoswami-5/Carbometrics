#include <QCoreApplication>
#include <QDebug>

#include <memory>
#include <cmath>

#include "TransportActivity.h"
#include "ElectricityActivity.h"
#include "FoodActivity.h"
#include "CarbonAnalyzer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CarbonAnalyzer analyzer;

    // Create three different activity objects.
    analyzer.addActivity(
        std::make_unique<TransportActivity>(
            "Car Travel", 10.0, 0.15
            )
        );

    analyzer.addActivity(
        std::make_unique<ElectricityActivity>(
            "Home Electricity", 5.0, 0.70
            )
        );

    analyzer.addActivity(
        std::make_unique<FoodActivity>(
            "Food Consumption", "Plant-based", 3, 0.80
            )
        );

    // Calculate emissions using runtime polymorphism.
    double total = analyzer.calculateTotalEmissions();

    qDebug() << "===== CarboMetrics Engine Test =====";

    qDebug() << "Activities:" << analyzer.getActivityCount();

    qDebug() << "Expected transport emissions: 1.5 kg CO2e";
    qDebug() << "Expected electricity emissions: 3.5 kg CO2e";
    qDebug() << "Expected food emissions: 2.4 kg CO2e";

    qDebug() << "Expected total: 7.4 kg CO2e";
    qDebug() << "Calculated total:" << total << "kg CO2e";

    if (std::abs(total - 7.4) < 0.000001)
    {
        qDebug() << "TEST PASSED";
    }
    else
    {
        qDebug() << "TEST FAILED";
        return 1;
    }

    return 0;
}