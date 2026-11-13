#ifndef CARBOMETRICS_DATABASEMANAGER_H
#define CARBOMETRICS_DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QByteArray>
#include <QString>
#include <QVector>
#include "emissionrecord.h"

struct UserProfile
{
    int userId = 0;
    QString fullName;
    QString username;
    QString email;
    QString profilePhoto;
};

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase();
    bool createTables();

    bool registerUser(const QString &fullName, const QString &username,
                      const QString &email, const QString &password,
                      UserProfile *createdUser = nullptr);
    bool authenticateUser(const QString &username, const QString &password,
                          UserProfile *authenticatedUser);
    bool getUserProfile(int userId, UserProfile *profile) const;
    bool updateUserProfile(int userId, const QString &fullName,
                           const QString &email, const QString &profilePhoto);
    bool changePassword(int userId, const QString &currentPassword,
                        const QString &newPassword);

    bool saveRecord(const EmissionRecord &record);
    QVector<EmissionRecord> getAllRecords(int userId) const;
    bool deleteRecord(int recordId, int userId);

    QString lastError() const;

private:
    QSqlDatabase database;
    QString errorMessage;
    static QByteArray hashPassword(const QString &password, const QByteArray &salt);
};

#endif // CARBOMETRICS_DATABASEMANAGER_H
