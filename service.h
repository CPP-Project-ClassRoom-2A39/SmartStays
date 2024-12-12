#ifndef SERVICE_H
#define SERVICE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class service
{
public:
    service();
    service(int, QString, int);
    int get_id();
    QString get_type();
    int get_prix();
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* chercher(QString column,QString text);
    QSqlQueryModel* tri(QString column,QString choix);
    int countType(const QString& type);

private:
    int id,prix;
    QString type;
};

#endif // SERVICE_H



