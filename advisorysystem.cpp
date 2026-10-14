
#include "advisorysystem.h"

#include "transportadvisoryrule.h"
#include "electricityadvisoryrule.h"
#include "foodadvisoryrule.h"

#include <cmath>
#include <memory>

// Constructor: register all available advisory rules.
// The order must match the emissions array below.
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

// Generate recommendations for the category with the highest emissions.
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

    // Find the category with the highest valid emissions.
    int highestIndex = -1;
    double highestEmissions = -1.0;

    for (int i = 0; i < 3; ++i) {

        // Ignore invalid or negative emission values.
        if (!std::isfinite(activityEmissions[i]) ||
            activityEmissions[i] < 0.0) {
            continue;
        }

        if (activityEmissions[i] > highestEmissions) {
            highestEmissions = activityEmissions[i];
            highestIndex = i;
        }
    }

    // Generate an advisory only for the highest-emission category.
    if (highestIndex >= 0 &&
        highestIndex < static_cast<int>(rules.size()) &&
        rules[highestIndex]->isApplicable(highestEmissions)) {

        recommendations.append(
            rules[highestIndex]->getRecommendation(
                highestEmissions
                )
            );
    }
    else {
        // Fallback when no valid category triggers its rule.
        recommendations.append(
            QStringLiteral(
                "No specific advisory was triggered by your current "
                "emission values. Continue recording your activities "
                "and review your transport, electricity and food habits. "
                "Your advisory thresholds may need adjustment to match "
                "your application's emission units."
                )
            );
    }

    return recommendations;
}

// Return the number of registered advisory rules.
int AdvisorySystem::getRuleCount() const
{
    return static_cast<int>(rules.size());
}
