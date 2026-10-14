#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QString>

class Activity
{
private:
    QString activityName;

public:
    explicit Activity(const QString& name)
        : activityName(name)
    {
    }

    virtual ~Activity() = default;

    QString getActivityName() const
    {
        return activityName;
    }

    virtual double calculateEmission() const = 0;
};

#endif // ACTIVITY_H