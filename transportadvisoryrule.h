#ifndef TRANSPORTADVISORYRULE_H
#define TRANSPORTADVISORYRULE_H
#include "advisoryrule.h"
class TransportAdvisoryRule final : public AdvisoryRule {
public:
    bool isApplicable(double emissions) const override;
    QString getRecommendation(double emissions) const override;
    QString getCategoryName() const override;
};
#endif