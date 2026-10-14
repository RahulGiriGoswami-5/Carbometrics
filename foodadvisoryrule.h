#ifndef FOODADVISORYRULE_H
#define FOODADVISORYRULE_H

#include "advisoryrule.h"

class FoodAdvisoryRule : public AdvisoryRule
{
public:
    bool isApplicable(double emissions) const override;

    QString getRecommendation() const override;
};

#endif // FOODADVISORYRULE_H