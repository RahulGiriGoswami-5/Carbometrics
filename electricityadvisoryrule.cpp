#include "electricityadvisoryrule.h"

bool ElectricityAdvisoryRule::isApplicable(double emissions) const
{
    // Illustrative project rule.
    return emissions >= 2.0;
}

QString ElectricityAdvisoryRule::getRecommendation() const
{
    return "Your recorded electricity emissions are relatively "
           "high under the project's advisory rules. Consider "
           "switching off unused appliances and reducing "
           "unnecessary electricity consumption.";
}