#include "FoodActivity.h"

FoodActivity::FoodActivity(
    const QString& name,
    const QString& category,
    int numberOfServings,
    double factor
    )
    : Activity(name),
    foodCategory(category),
    servings(0),
    emissionFactor(0.0)
{
    setServings(numberOfServings);

    if (factor >= 0.0)
    {
        emissionFactor = factor;
    }
}

void FoodActivity::setServings(int numberOfServings)
{
    if (numberOfServings >= 0 && numberOfServings <= 1000)
    {
        servings = numberOfServings;
    }
}

int FoodActivity::getServings() const
{
    return servings;
}

QString FoodActivity::getFoodCategory() const
{
    return foodCategory;
}

double FoodActivity::calculateEmission() const
{
    return servings * emissionFactor;
}