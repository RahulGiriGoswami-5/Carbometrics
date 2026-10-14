#ifndef EMISSIONRECORD_H
#define EMISSIONRECORD_H

#include <QString>

struct EmissionRecord
{
    int id = 0;

    QString userName;
    QString recordedAt;

    QString transportType;
    double distance = 0.0;
    double transportEmissions = 0.0;

    double electricity = 0.0;
    double electricityEmissions = 0.0;

    QString foodCategory;
    int servings = 0;
    double foodEmissions = 0.0;

    double totalEmissions = 0.0;
    QString footprintCategory;
};

#endif // EMISSIONRECORD_H