#include "transportadvisoryrule.h"

bool TransportAdvisoryRule::isApplicable(double emissions) const
{
    // Illustrative project rule.
    return emissions >= 1.0;
}

QString TransportAdvisoryRule::getRecommendation() const
{
    return "Transportation contributes significantly to your "
           "recorded emissions. Consider public transport, "
           "carpooling, or cycling where practical.";
}