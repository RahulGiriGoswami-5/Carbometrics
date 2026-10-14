#include "transportadvisoryrule.h"
bool TransportAdvisoryRule::isApplicable(double emissions) const { return emissions >= 1.0; }
QString TransportAdvisoryRule::getCategoryName() const { return QStringLiteral("Transport"); }
QString TransportAdvisoryRule::getRecommendation(double emissions) const {
    Q_UNUSED(emissions);
    return QStringLiteral(
        "TRANSPORT — 30-DAY REDUCTION PLAN\n"
        "Goal: reduce transport emissions by 10% in 30 days.\n\n"
        "START TODAY\n"
        "• Walk or cycle for short journeys when safe and practical.\n"
        "• Combine errands into one trip and avoid unnecessary idling.\n\n"
        "DURING WEEK 1\n"
        "• Replace two solo car/taxi journeys with public transport, carpooling, "
        "walking or cycling where feasible.\n"
        "• Compare practical commute options for cost, time and emissions.\n\n"
        "WEEKS 2–4\n"
        "• Plan routes in advance and share rides where possible. Keep vehicles "
        "maintained; consider a lower-emission vehicle only when replacement is due.\n\n"
        "TRACKING\n"
        "Record distance and travel mode weekly. Compare against your baseline "
        "using the same calculator and aim for a 10% reduction by day 30."
        );
}
