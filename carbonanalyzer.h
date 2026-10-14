#ifndef CARBONANALYZER_H
#define CARBONANALYZER_H

#include <vector>
#include <memory>

#include "Activity.h"

class CarbonAnalyzer
{
private:
    std::vector<std::unique_ptr<Activity>> activities;

public:
    CarbonAnalyzer() = default;

    void addActivity(std::unique_ptr<Activity> activity);

    double calculateTotalEmissions() const;

    void clearActivities();

    int getActivityCount() const;
};

#endif // CARBONANALYZER_H