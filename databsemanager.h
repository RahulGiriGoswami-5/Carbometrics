#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QVector>

#include "emissionrecord.h"

class DatabaseManager
{
private:
    QSqlDatabase database;
    QString errorMessage;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase();
    bool createTables();

    bool saveRecord(const EmissionRecord &record);

    QVector<EmissionRecord> getAllRecords() const;

    bool deleteRecord(int recordId);

    QString lastError() const;
};

#endif // DATABASEMANAGER_H