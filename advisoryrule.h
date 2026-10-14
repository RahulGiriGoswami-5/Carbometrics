#ifndef ADVISORYRULE_H
#define ADVISORYRULE_H

#include <QString>

class AdvisoryRule
{
public:
    virtual ~AdvisoryRule() = default;

    virtual bool isApplicable(double emissions) const = 0;

    virtual QString getRecommendation() const = 0;
};

#endif // ADVISORYRULE_H