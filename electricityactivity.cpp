#include "ElectricityActivity.h"

ElectricityActivity::ElectricityActivity(
    const QString& name,
    double consumptionKWh,
    double factor
    )
    : Activity(name),
    electricityUsed(0.0),
    emissionFactor(0.0)
{
    setConsumption(consumptionKWh);

    if (factor >= 0.0)
    {
        emissionFactor = factor;
    }
}

void ElectricityActivity::setConsumption(double consumptionKWh)
{
    if (consumptionKWh >= 0.0)
    {
        electricityUsed = consumptionKWh;
    }
}

double ElectricityActivity::getConsumption() const
{
    return electricityUsed;
}

double ElectricityActivity::calculateEmission() const
{
    return electricityUsed * emissionFactor;
}