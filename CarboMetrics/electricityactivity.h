#ifndef ELECTRICITYACTIVITY_H
#define ELECTRICITYACTIVITY_H

#include "Activity.h"

class ElectricityActivity : public Activity
{
private:
    double electricityUsed;
    double emissionFactor;

public:
    ElectricityActivity(
        const QString& name,
        double consumptionKWh,
        double factor
        );

    void setConsumption(double consumptionKWh);
    double getConsumption() const;

    double calculateEmission() const override;
};

#endif // ELECTRICITYACTIVITY_H