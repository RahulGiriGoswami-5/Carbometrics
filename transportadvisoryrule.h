#ifndef TRANSPORTADVISORYRULE_H
#define TRANSPORTADVISORYRULE_H

#include "advisoryrule.h"

class TransportAdvisoryRule : public AdvisoryRule
{
public:
    bool isApplicable(double emissions) const override;

    QString getRecommendation() const override;
};

#endif // TRANSPORTADVISORYRULE_H