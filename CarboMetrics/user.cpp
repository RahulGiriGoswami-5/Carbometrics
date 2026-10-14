#include "User.h"

User::User()
    : name(""), age(0)
{
}

User::User(const QString& userName, int userAge)
    : name(userName), age(0)
{
    setAge(userAge);
}

void User::setName(const QString& userName)
{
    name = userName;
}

QString User::getName() const
{
    return name;
}

void User::setAge(int userAge)
{
    if (userAge >= 0 && userAge <= 120)
    {
        age = userAge;
    }
}

int User::getAge() const
{
    return age;
}