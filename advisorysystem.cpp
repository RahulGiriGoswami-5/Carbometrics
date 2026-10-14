#include "advisorysystem.h"

#include "transportadvisoryrule.h"
#include "electricityadvisoryrule.h"
#include "foodadvisoryrule.h"

#include <utility>

// Constructor: register all available advisory rules.
AdvisorySystem::AdvisorySystem()
{
    rules.push_back(
        std::make_unique<TransportAdvisoryRule>()
        );

    rules.push_back(
        std::make_unique<ElectricityAdvisoryRule>()
        );

    rules.push_back(
        std::make_unique<FoodAdvisoryRule>()
        );
}

// Generate recommendations for the recorded activities.
QStringList AdvisorySystem::generateRecommendations(
    double transportEmissions,
    double electricityEmissions,
    double foodEmissions) const
{
    QStringList recommendations;

    const double activityEmissions[] = {
        transportEmissions,
        electricityEmissions,
        foodEmissions
    };

    // Each rule evaluates its corresponding activity.
    for (std::size_t i = 0; i < rules.size(); ++i) {

        if (rules[i]->isApplicable(activityEmissions[i])) {

            recommendations.append(
                rules[i]->getRecommendation()
                );
        }
    }

    // If no individual activity triggers a recommendation,
    // provide a general message.
    if (recommendations.isEmpty()) {

        recommendations.append(
            "Your recorded activities do not trigger any "
            "specific advisory rules. Continue monitoring "
            "your footprint and look for opportunities "
            "to reduce emissions."
            );
    }

    return recommendations;
}

// Return the number of registered advisory rules.
int AdvisorySystem::getRuleCount() const
{
    return static_cast<int>(rules.size());
}