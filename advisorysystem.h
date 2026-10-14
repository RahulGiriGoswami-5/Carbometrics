#ifndef ADVISORYSYSTEM_H
#define ADVISORYSYSTEM_H

#include <QStringList>
#include <vector>
#include <memory>

#include "advisoryrule.h"

class AdvisorySystem
{
private:
    std::vector<std::unique_ptr<AdvisoryRule>> rules;

public:
    AdvisorySystem();

    QStringList generateRecommendations(
        double transportEmissions,
        double electricityEmissions,
        double foodEmissions
        ) const;

    int getRuleCount() const;
};

#endif // ADVISORYSYSTEM_H