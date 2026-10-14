#include "foodadvisoryrule.h"

bool FoodAdvisoryRule::isApplicable(double emissions) const
{
    // Illustrative project rule.
    return emissions >= 2.0;
}

QString FoodAdvisoryRule::getRecommendation() const
{
    return "Consider incorporating more plant-based meals "
           "into your diet where suitable. Compare the "
           "estimated emissions of different meal choices "
           "to identify possible opportunities for reduction.";
}