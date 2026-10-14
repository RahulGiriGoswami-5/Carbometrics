#include "electricityadvisoryrule.h"
bool ElectricityAdvisoryRule::isApplicable(double emissions) const { return emissions >= 2.0; }
QString ElectricityAdvisoryRule::getCategoryName() const { return QStringLiteral("Electricity"); }
QString ElectricityAdvisoryRule::getRecommendation(double emissions) const {
    Q_UNUSED(emissions);
    return QStringLiteral(
        "ELECTRICITY — 30-DAY REDUCTION PLAN\n"
        "Goal: reduce electricity-related emissions by 10% in 30 days.\n\n"
        "START TODAY\n"
        "• Switch off unused lights, fans and appliances.\n"
        "• Turn off standby devices when practical and use daylight when comfortable.\n\n"
        "DURING WEEK 1\n"
        "• Identify your three most-used appliances. Use efficient settings and "
        "power-saving modes.\n"
        "• If using air conditioning, clean filters and choose a comfortable, "
        "energy-conscious setting.\n\n"
        "WEEKS 2–4\n"
        "• Replace frequently used bulbs with LEDs when due.\n"
        "• Check your meter or electricity bill weekly; investigate unusual increases.\n"
        "• Compare renewable electricity options if your utility offers them.\n\n"
        "TRACKING\n"
        "Record kWh weekly if available and compare with a similar baseline period. "
        "Aim for 10% lower use by day 30; weather and occupancy can affect results."
        );
}
