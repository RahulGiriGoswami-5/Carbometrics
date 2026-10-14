#ifndef FOOTPRINTANALYZER_H
#define FOOTPRINTANALYZER_H

#include <QString>

class FootprintAnalyzer
{
public:
    enum class Category
    {
        Low,
        Moderate,
        High,
        Invalid
    };

    FootprintAnalyzer();

    Category classifyFootprint(double totalEmissions) const;

    QString getCategoryName(Category category) const;

    QString getAnalysis(double totalEmissions) const;

    void setThresholds(double lowLimit, double moderateLimit);

private:
    double lowThreshold;
    double moderateThreshold;
};

#endif // FOOTPRINTANALYZER_H