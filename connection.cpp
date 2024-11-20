#include "connection.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection()
{

}

bool Connection::createconnection(){
    bool test=false;
    db = QSqlDatabase::addDatabase ("QODBC");
        db.setDatabaseName("projetcpp");
        db.setUserName("projetCPP2A39");
        db.setPassword("esprit24");
if (db.open()) test=true;

else {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
    }

return test;
}
void Connection::closeconnection(){db.close();}
