#include "foodadvisoryrule.h"
bool FoodAdvisoryRule::isApplicable(double emissions) const { return emissions >= 2.0; }
QString FoodAdvisoryRule::getCategoryName() const { return QStringLiteral("Food"); }
QString FoodAdvisoryRule::getRecommendation(double emissions) const {
    Q_UNUSED(emissions);
    return QStringLiteral(
        "FOOD — 30-DAY REDUCTION PLAN\n"
        "Goal: reduce food-related emissions by 10% in 30 days without compromising "
        "nutrition, health needs or food access.\n\n"
        "START TODAY\n"
        "• Plan meals and shopping; check what food you already have.\n"
        "• Store food correctly and use leftovers safely to prevent avoidable waste.\n\n"
        "DURING WEEK 1\n"
        "• Try replacing one meat-heavy meal with a balanced meal using beans, lentils, "
        "peas, tofu or another suitable plant protein.\n"
        "• Freeze or share extra portions before they spoil.\n\n"
        "WEEKS 2–4\n"
        "• Add plant-forward meals if suitable for your diet and preferences.\n"
        "• Buy realistic quantities of perishable food. Compost unavoidable scraps "
        "where a suitable service exists; preventing edible waste comes first.\n\n"
        "TRACKING\n"
        "Note meals, substitutions and food waste weekly. Use the same estimator "
        "for baseline and follow-up. Adapt to allergies, health, culture and budget."
        );
}