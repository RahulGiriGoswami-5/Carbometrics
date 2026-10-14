#ifndef ELECTRICITYADVISORYRULE_H
#define ELECTRICITYADVISORYRULE_H

#include "advisoryrule.h"

class ElectricityAdvisoryRule : public AdvisoryRule
{
public:
    bool isApplicable(double emissions) const override;

    QString getRecommendation() const override;
};

#endif // ELECTRICITYADVISORYRULE_H