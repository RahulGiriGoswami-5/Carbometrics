#ifndef FOODACTIVITY_H
#define FOODACTIVITY_H

#include "Activity.h"
#include <QString>

class FoodActivity : public Activity
{
private:
    QString foodCategory;
    int servings;
    double emissionFactor;

public:
    FoodActivity(
        const QString& name,
        const QString& category,
        int numberOfServings,
        double factor
        );

    void setServings(int numberOfServings);
    int getServings() const;

    QString getFoodCategory() const;

    double calculateEmission() const override;
};

#endif // FOODACTIVITY_H