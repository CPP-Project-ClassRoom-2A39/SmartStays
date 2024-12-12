#include "connection.h"

Connection::Connection()
{

}
/*
bool Connection::createconnect() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db.setDatabaseName("projetcpp"); // Nom de la source de données
        db.setUserName("PROJET");        // Nom de l'utilisateur
        db.setPassword("esprit24");
    }
    return db.open();
}*/
void Connection::closeConnection(){db.close();}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projet"); // Nom de la source de données
db.setUserName("projetCPP2A39");        // Nom de l'utilisateur
db.setPassword("esprit");

if (db.open())
test=true;
    return  test;
}
