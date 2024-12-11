#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QSqlDatabase>

class DatabaseHelper
{
public:
    DatabaseHelper();
    static QSqlDatabase connectToDatabase(); // Méthode pour se connecter à Oracle
};

#endif // DATABASEHELPER_H
