#include "connection.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
Connection::Connection() {}

bool Connection::createconnect() {
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A39"); // insérer le nom de la source de données
    db.setUserName("system");                // insérer le nom de l'utilisateur
    db.setPassword("esprit24");              // insérer le mot de passe de cet utilisateur

    if (db.open()) {
        test = true;
    } else {
        qDebug() << "Database connection error:" << db.lastError().text();
    }

    return test;
}

void Connection::closeConnection() {
    db.close();
}

