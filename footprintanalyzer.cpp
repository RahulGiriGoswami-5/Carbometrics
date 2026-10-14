#include "footprintanalyzer.h"

FootprintAnalyzer::FootprintAnalyzer()
    : lowThreshold(3.0),
    moderateThreshold(7.0)
{
}

FootprintAnalyzer::Category
FootprintAnalyzer::classifyFootprint(double totalEmissions) const
{
    if (totalEmissions < 0.0)
    {
        return Category::Invalid;
    }

    if (totalEmissions < lowThreshold)
    {
        return Category::Low;
    }

    if (totalEmissions < moderateThreshold)
    {
        return Category::Moderate;
    }

    return Category::High;
}

QString FootprintAnalyzer::getCategoryName(Category category) const
{
    switch (category)
    {
    case Category::Low:
        return "Low";

    case Category::Moderate:
        return "Moderate";

    case Category::High:
        return "High";

    case Category::Invalid:
        return "Invalid";
    }

    return "Invalid";
}

QString FootprintAnalyzer::getAnalysis(double totalEmissions) const
{
    const Category category = classifyFootprint(totalEmissions);

    switch (category)
    {
    case Category::Low:
        return "Your recorded activities fall within "
               "CarboMetrics' low-emission category. "
               "Continue maintaining lower-emission habits.";

    case Category::Moderate:
        return "Your recorded activities fall within "
               "CarboMetrics' moderate-emission category. "
               "Review your activities to identify "
               "opportunities for reducing emissions.";

    case Category::High:
        return "Your recorded activities fall within "
               "CarboMetrics' high-emission category. "
               "Consider reviewing your higher-emission activities.";

    case Category::Invalid:
        return "The emission total is invalid. "
               "Please provide a non-negative value.";
    }

    return "Unable to analyze the carbon footprint.";
}

void FootprintAnalyzer::setThresholds(
    double lowLimit,
    double moderateLimit)
{
    if (lowLimit >= 0.0 && moderateLimit > lowLimit)
    {
        lowThreshold = lowLimit;
        moderateThreshold = moderateLimit;
    }
}