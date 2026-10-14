#include "databasemanager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>

DatabaseManager::DatabaseManager()
{
    const QString connectionName = "CarboMetricsConnection";

    if (QSqlDatabase::contains(connectionName)) {
        database = QSqlDatabase::database(connectionName);
    } else {
        database = QSqlDatabase::addDatabase(
            "QSQLITE",
            connectionName
            );

        const QString databasePath =
            QDir(QCoreApplication::applicationDirPath())
              .filePath("CarboMetrics.db");

        database.setDatabaseName(databasePath);
    }
}

DatabaseManager::~DatabaseManager()
{
    if (database.isOpen()) {
        database.close();
    }
}

bool DatabaseManager::openDatabase()
{
    if (database.isOpen()) {
        return true;
    }

    if (!database.open()) {
        errorMessage = database.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createTables()
{
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);

    const QString sql =
        "CREATE TABLE IF NOT EXISTS emission_records ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_name TEXT NOT NULL, "
        "recorded_at TEXT NOT NULL, "
        "transport_type TEXT, "
        "distance REAL, "
        "transport_emissions REAL, "
        "electricity REAL, "
        "electricity_emissions REAL, "
        "food_category TEXT, "
        "servings INTEGER, "
        "food_emissions REAL, "
        "total_emissions REAL NOT NULL, "
        "footprint_category TEXT"
        ")";

    if (!query.exec(sql)) {
        errorMessage = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::saveRecord(const EmissionRecord &record)
{
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);

    query.prepare(
        "INSERT INTO emission_records ("
        "user_name, recorded_at, transport_type, distance, "
        "transport_emissions, electricity, electricity_emissions, "
        "food_category, servings, food_emissions, "
        "total_emissions, footprint_category"
        ") VALUES ("
        ":user_name, :recorded_at, :transport_type, :distance, "
        ":transport_emissions, :electricity, :electricity_emissions, "
        ":food_category, :servings, :food_emissions, "
        ":total_emissions, :footprint_category"
        ")"
        );

    query.bindValue(":user_name", record.userName);

    query.bindValue(
        ":recorded_at",
        record.recordedAt.isEmpty()
            ? QDateTime::currentDateTime().toString(Qt::ISODate)
            : record.recordedAt
        );

    query.bindValue(":transport_type", record.transportType);
    query.bindValue(":distance", record.distance);
    query.bindValue(":transport_emissions", record.transportEmissions);

    query.bindValue(":electricity", record.electricity);
    query.bindValue(":electricity_emissions", record.electricityEmissions);

    query.bindValue(":food_category", record.foodCategory);
    query.bindValue(":servings", record.servings);
    query.bindValue(":food_emissions", record.foodEmissions);

    query.bindValue(":total_emissions", record.totalEmissions);
    query.bindValue(":footprint_category", record.footprintCategory);

    if (!query.exec()) {
        errorMessage = query.lastError().text();
        return false;
    }

    return true;
}

QVector<EmissionRecord> DatabaseManager::getAllRecords() const
{
    QVector<EmissionRecord> records;

    if (!database.isOpen()) {
        return records;
    }

    QSqlQuery query(database);

    if (!query.exec(
            "SELECT id, user_name, recorded_at, transport_type, "
            "distance, transport_emissions, electricity, "
            "electricity_emissions, food_category, servings, "
            "food_emissions, total_emissions, footprint_category "
            "FROM emission_records ORDER BY id DESC")) {
        return records;
    }

    while (query.next()) {
        EmissionRecord record;

        record.id = query.value("id").toInt();
        record.userName = query.value("user_name").toString();
        record.recordedAt = query.value("recorded_at").toString();

        record.transportType = query.value("transport_type").toString();
        record.distance = query.value("distance").toDouble();
        record.transportEmissions =
            query.value("transport_emissions").toDouble();

        record.electricity = query.value("electricity").toDouble();
        record.electricityEmissions =
            query.value("electricity_emissions").toDouble();

        record.foodCategory = query.value("food_category").toString();
        record.servings = query.value("servings").toInt();
        record.foodEmissions =
            query.value("food_emissions").toDouble();

        record.totalEmissions =
            query.value("total_emissions").toDouble();

        record.footprintCategory =
            query.value("footprint_category").toString();

        records.append(record);
    }

    return records;
}

bool DatabaseManager::deleteRecord(int recordId)
{
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(database);

    query.prepare(
        "DELETE FROM emission_records WHERE id = :id"
        );

    query.bindValue(":id", recordId);

    if (!query.exec()) {
        errorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

QString DatabaseManager::lastError() const
{
    return errorMessage;
}