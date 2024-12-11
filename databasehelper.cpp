#include "databasehelper.h"
#include <QSqlError>
#include <QDebug>

DatabaseHelper::DatabaseHelper()
{
    // Constructeur par défaut (peut rester vide)
}

QSqlDatabase DatabaseHelper::connectToDatabase()
{
    if (QSqlDatabase::contains("OracleConnection")) {
        QSqlDatabase db = QSqlDatabase::database("OracleConnection");
        if (db.isOpen()) {
            qDebug() << "Connexion à Oracle déjà établie.";
            return db;
        }
    }

    // Testez QOCI en premier (pilote Oracle natif)
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI", "OracleConnection");
    db.setHostName("localhost");
    db.setDatabaseName("bd_smartstays");
    db.setUserName("bd_smartstays");
    db.setPassword("esprit24");

    if (!db.open()) {
        qDebug() << "Erreur avec QOCI, tentative avec QODBC : " << db.lastError().text();

        // Si QOCI échoue, essayez avec QODBC
        db = QSqlDatabase::addDatabase("QODBC", "OracleConnection");
        db.setDatabaseName("DSN_ORACLE"); // Remplacez "DSN_ORACLE" par le nom de votre DSN ODBC
        db.setUserName("bd_smartstays");
        db.setPassword("esprit24");

        if (!db.open()) {
            qDebug() << "Erreur avec QODBC également : " << db.lastError().text();
        } else {
            qDebug() << "Connexion réussie avec QODBC.";
        }
    } else {
        qDebug() << "Connexion réussie avec QOCI.";
    }

    return db;
}
