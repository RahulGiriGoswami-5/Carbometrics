#include "TransportActivity.h"

TransportActivity::TransportActivity(
    const QString& name,
    double distanceKm,
    double factor
    )
    : Activity(name),
    distance(0.0),
    emissionFactor(0.0)
{
    setDistance(distanceKm);

    if (factor >= 0.0)
    {
        emissionFactor = factor;
    }
}

void TransportActivity::setDistance(double distanceKm)
{
    if (distanceKm >= 0.0)
    {
        distance = distanceKm;
    }
}

double TransportActivity::getDistance() const
{
    return distance;
}

double TransportActivity::calculateEmission() const
{
    return distance * emissionFactor;
}