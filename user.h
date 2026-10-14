#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString name;
    int age;

public:
    User();
    User(const QString& userName, int userAge);

    void setName(const QString& userName);
    QString getName() const;

    void setAge(int userAge);
    int getAge() const;
};

#endif // USER_H