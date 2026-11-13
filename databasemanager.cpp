#include "databasemanager.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QDateTime>
#include <QDir>
#include <QRandomGenerator>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>

namespace {
constexpr int PasswordIterations = 120000;
constexpr int PasswordKeyLength = 32;

bool hasColumn(const QSqlDatabase &db, const QString &table, const QString &column)
{
    QSqlQuery q(db);
    if (!q.exec("PRAGMA table_info(" + table + ")")) return false;
    while (q.next()) if (q.value("name").toString() == column) return true;
    return false;
}
}

DatabaseManager::DatabaseManager()
{
    const QString connectionName = "CarboMetricsConnection";
    if (QSqlDatabase::contains(connectionName)) {
        database = QSqlDatabase::database(connectionName);
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        database.setDatabaseName(QDir(QCoreApplication::applicationDirPath()).filePath("CarboMetrics.db"));
    }
}

DatabaseManager::~DatabaseManager()
{
    if (database.isOpen()) database.close();
}

bool DatabaseManager::openDatabase()
{
    if (database.isOpen()) return true;
    if (!database.open()) { errorMessage = database.lastError().text(); return false; }
    QSqlQuery pragma(database);
    pragma.exec("PRAGMA foreign_keys = ON");
    return true;
}

bool DatabaseManager::createTables()
{
    if (!openDatabase()) return false;
    QSqlQuery q(database);
    const QStringList statements = {
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT, full_name TEXT NOT NULL, "
        "username TEXT NOT NULL COLLATE NOCASE UNIQUE, email TEXT NOT NULL COLLATE NOCASE UNIQUE, "
        "password_hash BLOB NOT NULL, password_salt BLOB NOT NULL, profile_photo TEXT DEFAULT '')",
        "CREATE TABLE IF NOT EXISTS emission_records ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL DEFAULT 0, "
        "user_name TEXT NOT NULL, recorded_at TEXT NOT NULL, transport_type TEXT, distance REAL, "
        "transport_emissions REAL, electricity REAL, electricity_emissions REAL, food_category TEXT, "
        "servings INTEGER, food_emissions REAL, total_emissions REAL NOT NULL, footprint_category TEXT, "
        "FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE CASCADE)"
    };
    for (const QString &sql : statements) {
        if (!q.exec(sql)) { errorMessage = q.lastError().text(); return false; }
    }
    // Safe migration for databases created by older CarboMetrics versions.
    if (!hasColumn(database, "emission_records", "user_id")) {
        if (!q.exec("ALTER TABLE emission_records ADD COLUMN user_id INTEGER NOT NULL DEFAULT 0")) {
            errorMessage = q.lastError().text(); return false;
        }
    }
    if (!q.exec("CREATE INDEX IF NOT EXISTS idx_emission_user ON emission_records(user_id, id DESC)")) {
        errorMessage = q.lastError().text(); return false;
    }
    return true;
}

QByteArray DatabaseManager::hashPassword(const QString &password, const QByteArray &salt)
{
    // PBKDF2-HMAC-SHA256 with a unique random salt for every account.
    return QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256, password.toUtf8(), salt,
        PasswordIterations, PasswordKeyLength);
}

bool DatabaseManager::registerUser(const QString &fullName, const QString &username,
                                   const QString &email, const QString &password,
                                   UserProfile *createdUser)
{
    if (!openDatabase() || !createTables()) return false;
    if (fullName.trimmed().isEmpty() || username.trimmed().isEmpty() || email.trimmed().isEmpty()) {
        errorMessage = "Please complete all required fields."; return false;
    }
    if (username.trimmed().size() < 3 || password.size() < 8) {
        errorMessage = "Username must be at least 3 characters and password at least 8 characters."; return false;
    }
    if (!email.contains('@') || !email.contains('.')) { errorMessage = "Please enter a valid email address."; return false; }

    QByteArray salt(16, '\0');
    for (int i = 0; i < salt.size(); ++i) salt[i] = static_cast<char>(QRandomGenerator::system()->generate() & 0xff);
    QSqlQuery q(database);
    q.prepare("INSERT INTO users(full_name, username, email, password_hash, password_salt) VALUES(:name,:username,:email,:hash,:salt)");
    q.bindValue(":name", fullName.trimmed());
    q.bindValue(":username", username.trimmed());
    q.bindValue(":email", email.trimmed());
    q.bindValue(":hash", hashPassword(password, salt));
    q.bindValue(":salt", salt);
    if (!q.exec()) {
        errorMessage = q.lastError().text();
        if (errorMessage.contains("UNIQUE", Qt::CaseInsensitive)) errorMessage = "That username or email is already registered.";
        return false;
    }
    if (createdUser) {
        createdUser->userId = q.lastInsertId().toInt();
        createdUser->fullName = fullName.trimmed();
        createdUser->username = username.trimmed();
        createdUser->email = email.trimmed();
    }
    return true;
}

bool DatabaseManager::authenticateUser(const QString &username, const QString &password,
                                       UserProfile *authenticatedUser)
{
    if (!openDatabase() || !createTables()) return false;
    QSqlQuery q(database);
    q.prepare("SELECT user_id, full_name, username, email, profile_photo, password_hash, password_salt FROM users WHERE username = :username COLLATE NOCASE");
    q.bindValue(":username", username.trimmed());
    if (!q.exec()) { errorMessage = q.lastError().text(); return false; }
    if (!q.next()) { errorMessage = "Incorrect username or password."; return false; }
    const QByteArray expected = q.value("password_hash").toByteArray();
    const QByteArray actual = hashPassword(password, q.value("password_salt").toByteArray());
    if (expected != actual) { errorMessage = "Incorrect username or password."; return false; }
    if (authenticatedUser) {
        authenticatedUser->userId = q.value("user_id").toInt();
        authenticatedUser->fullName = q.value("full_name").toString();
        authenticatedUser->username = q.value("username").toString();
        authenticatedUser->email = q.value("email").toString();
        authenticatedUser->profilePhoto = q.value("profile_photo").toString();
    }
    return true;
}

bool DatabaseManager::getUserProfile(int userId, UserProfile *profile) const
{
    if (!database.isOpen() || !profile) return false;
    QSqlQuery q(database);
    q.prepare("SELECT user_id, full_name, username, email, profile_photo FROM users WHERE user_id=:id");
    q.bindValue(":id", userId);
    if (!q.exec() || !q.next()) return false;
    profile->userId = q.value("user_id").toInt();
    profile->fullName = q.value("full_name").toString();
    profile->username = q.value("username").toString();
    profile->email = q.value("email").toString();
    profile->profilePhoto = q.value("profile_photo").toString();
    return true;
}

bool DatabaseManager::updateUserProfile(int userId, const QString &fullName,
                                        const QString &email, const QString &profilePhoto)
{
    if (!openDatabase()) return false;
    if (fullName.trimmed().isEmpty() || email.trimmed().isEmpty() || !email.contains('@')) {
        errorMessage = "Enter a valid name and email address."; return false;
    }
    QSqlQuery q(database);
    q.prepare("UPDATE users SET full_name=:name, email=:email, profile_photo=:photo WHERE user_id=:id");
    q.bindValue(":name", fullName.trimmed()); q.bindValue(":email", email.trimmed());
    q.bindValue(":photo", profilePhoto); q.bindValue(":id", userId);
    if (!q.exec()) { errorMessage = q.lastError().text(); if (errorMessage.contains("UNIQUE", Qt::CaseInsensitive)) errorMessage = "That email is already used by another account."; return false; }
    return q.numRowsAffected() == 1;
}

bool DatabaseManager::changePassword(int userId, const QString &currentPassword, const QString &newPassword)
{
    if (!openDatabase()) return false;
    if (newPassword.size() < 8) { errorMessage = "New password must contain at least 8 characters."; return false; }
    QSqlQuery q(database);
    q.prepare("SELECT password_hash, password_salt FROM users WHERE user_id=:id"); q.bindValue(":id", userId);
    if (!q.exec() || !q.next()) { errorMessage = "Account not found."; return false; }
    if (hashPassword(currentPassword, q.value("password_salt").toByteArray()) != q.value("password_hash").toByteArray()) {
        errorMessage = "Current password is incorrect."; return false;
    }
    QByteArray salt(16, '\0');
    for (int i = 0; i < salt.size(); ++i) salt[i] = static_cast<char>(QRandomGenerator::system()->generate() & 0xff);
    QSqlQuery update(database);
    update.prepare("UPDATE users SET password_hash=:hash, password_salt=:salt WHERE user_id=:id");
    update.bindValue(":hash", hashPassword(newPassword, salt)); update.bindValue(":salt", salt); update.bindValue(":id", userId);
    if (!update.exec()) { errorMessage = update.lastError().text(); return false; }
    return true;
}

bool DatabaseManager::saveRecord(const EmissionRecord &record)
{
    if (!openDatabase()) return false;
    QSqlQuery q(database);
    q.prepare("INSERT INTO emission_records(user_id,user_name,recorded_at,transport_type,distance,transport_emissions,electricity,electricity_emissions,food_category,servings,food_emissions,total_emissions,footprint_category) VALUES(:uid,:name,:date,:transport,:distance,:transportEmissions,:electricity,:electricityEmissions,:food,:servings,:foodEmissions,:total,:category)");
    q.bindValue(":uid", record.userId); q.bindValue(":name", record.userName);
    q.bindValue(":date", record.recordedAt.isEmpty() ? QDateTime::currentDateTime().toString(Qt::ISODate) : record.recordedAt);
    q.bindValue(":transport", record.transportType); q.bindValue(":distance", record.distance);
    q.bindValue(":transportEmissions", record.transportEmissions); q.bindValue(":electricity", record.electricity);
    q.bindValue(":electricityEmissions", record.electricityEmissions); q.bindValue(":food", record.foodCategory);
    q.bindValue(":servings", record.servings); q.bindValue(":foodEmissions", record.foodEmissions);
    q.bindValue(":total", record.totalEmissions); q.bindValue(":category", record.footprintCategory);
    if (!q.exec()) { errorMessage = q.lastError().text(); return false; }
    return true;
}

QVector<EmissionRecord> DatabaseManager::getAllRecords(int userId) const
{
    QVector<EmissionRecord> records;
    if (!database.isOpen()) return records;
    QSqlQuery q(database);
    q.prepare("SELECT id,user_id,user_name,recorded_at,transport_type,distance,transport_emissions,electricity,electricity_emissions,food_category,servings,food_emissions,total_emissions,footprint_category FROM emission_records WHERE user_id=:uid ORDER BY id DESC");
    q.bindValue(":uid", userId);
    if (!q.exec()) return records;
    while (q.next()) {
        EmissionRecord r;
        r.id=q.value("id").toInt(); r.userId=q.value("user_id").toInt(); r.userName=q.value("user_name").toString();
        r.recordedAt=q.value("recorded_at").toString(); r.transportType=q.value("transport_type").toString();
        r.distance=q.value("distance").toDouble(); r.transportEmissions=q.value("transport_emissions").toDouble();
        r.electricity=q.value("electricity").toDouble(); r.electricityEmissions=q.value("electricity_emissions").toDouble();
        r.foodCategory=q.value("food_category").toString(); r.servings=q.value("servings").toInt();
        r.foodEmissions=q.value("food_emissions").toDouble(); r.totalEmissions=q.value("total_emissions").toDouble();
        r.footprintCategory=q.value("footprint_category").toString(); records.append(r);
    }
    return records;
}

bool DatabaseManager::deleteRecord(int recordId, int userId)
{
    if (!openDatabase()) return false;
    QSqlQuery q(database); q.prepare("DELETE FROM emission_records WHERE id=:id AND user_id=:uid");
    q.bindValue(":id", recordId); q.bindValue(":uid", userId);
    if (!q.exec()) { errorMessage=q.lastError().text(); return false; }
    return q.numRowsAffected() > 0;
}

QString DatabaseManager::lastError() const { return errorMessage; }
