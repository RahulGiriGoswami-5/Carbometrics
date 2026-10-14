#ifndef FOODADVISORYRULE_H
#define FOODADVISORYRULE_H
#include "advisoryrule.h"
class FoodAdvisoryRule final : public AdvisoryRule {
public:
    bool isApplicable(double emissions) const override;
    QString getRecommendation(double emissions) const override;
    QString getCategoryName() const override;
};
#endif
