#include "service.h"
#include <QDebug>

service::service() : id(0), type(""), prix(0) {}

service::service(int ID, QString TYPE, int PRIX)
    : id(ID), type(TYPE), prix(PRIX) {}

int service::get_id() { return id; }
QString service::get_type() { return type; }
int service::get_prix() { return prix; }

bool service::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO service (ID, TYPE, PRIX) "
                  "VALUES (:ID, :TYPE, :PRIX)");
    query.bindValue(":ID", id);
    query.bindValue(":TYPE", type);
    query.bindValue(":PRIX", prix);
    return query.exec();
}

QSqlQueryModel* service::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM service");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX"));
    return model;
}

bool service::supprimer(int ID) {
    QSqlQuery query;
    query.prepare("DELETE FROM service WHERE ID = :ID");
    query.bindValue(":ID", ID);
    return query.exec();
}

bool service::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE service SET TYPE = :TYPE, PRIX = :PRIX "
                  "WHERE ID = :ID");
    query.bindValue(":ID", id);
    query.bindValue(":TYPE", type);
    query.bindValue(":PRIX", prix);
    return query.exec();
}

QSqlQueryModel* service::chercher(QString column,QString text)
{
QSqlQueryModel* model = new QSqlQueryModel();
model->setQuery("SELECT * FROM service WHERE "+column+" LIKE '%" + text + "%' ");//LIKE  !!!  %2%  //yessine   ss   ,sequence !!


return model;
}

QSqlQueryModel* service::tri(QString column,QString choix)
{
QSqlQueryModel* model = new QSqlQueryModel();
model->setQuery("SELECT * FROM service ORDER BY "+column +" "+choix);   // tri = order by  ,NOM


return model;
}
int service::countType(const QString& type)
{
    int count = 0;

    QSqlQuery query;
    query.prepare("SELECT COUNT(TYPE) FROM SERVICE WHERE TYPE = :type");
    query.bindValue(":type", type);

    if (query.exec() && query.next()) {
        count = query.value(0).toInt();
    }

    return count;
}
