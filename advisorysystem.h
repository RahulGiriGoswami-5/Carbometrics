#ifndef ADVISORYSYSTEM_H
#define ADVISORYSYSTEM_H
#include "advisoryrule.h"
#include <QStringList>
#include <memory>
#include <vector>

class AdvisorySystem {
public:
    AdvisorySystem();
    QStringList generateRecommendations(double transportEmissions,
                                        double electricityEmissions,
                                        double foodEmissions) const;
    int getRuleCount() const;
private:
    std::vector<std::unique_ptr<AdvisoryRule>> rules;
};
#endif
