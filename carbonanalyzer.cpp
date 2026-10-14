#include "CarbonAnalyzer.h"

#include <utility>

void CarbonAnalyzer::addActivity(
    std::unique_ptr<Activity> activity)
{
    if (activity)
    {
        activities.push_back(std::move(activity));
    }
}

double CarbonAnalyzer::calculateTotalEmissions() const
{
    double total = 0.0;

    for (const auto& activity : activities)
    {
        total += activity->calculateEmission();
    }

    return total;
}

void CarbonAnalyzer::clearActivities()
{
    activities.clear();
}

int CarbonAnalyzer::getActivityCount() const
{
    return static_cast<int>(activities.size());
}