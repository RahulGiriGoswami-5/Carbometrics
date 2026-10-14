#ifndef ELECTRICITYADVISORYRULE_H
#define ELECTRICITYADVISORYRULE_H
#include "advisoryrule.h"
class ElectricityAdvisoryRule final : public AdvisoryRule {
public:
    bool isApplicable(double emissions) const override;
    QString getRecommendation(double emissions) const override;
    QString getCategoryName() const override;
};
#endif