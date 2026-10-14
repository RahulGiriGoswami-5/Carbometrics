#ifndef TRANSPORTACTIVITY_H
#define TRANSPORTACTIVITY_H

#include "Activity.h"

class TransportActivity : public Activity
{
private:
    double distance;
    double emissionFactor;

public:
    TransportActivity(
        const QString& name,
        double distanceKm,
        double factor
        );

    void setDistance(double distanceKm);
    double getDistance() const;

    double calculateEmission() const override;
};

#endif // TRANSPORTACTIVITY_H