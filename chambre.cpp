#include "chambre.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Chambre::Chambre() {}

Chambre::Chambre(int num, QString type, QString etat, float prix, int etage)
{
    this->num = num;
    this->type = type;
    this->etat = etat;
    this->prix = prix;
    this->etage = etage;
}

bool Chambre::ajouter()
{
    QSqlQuery query;

    // Pr�parer la requ�te SQL
    if (!query.prepare("INSERT INTO CHAMBRES (NUM, TYPE, ETAT, PRIX, ETAGE) VALUES (:num, :type, :etat, :prix, :etage)")) {
        qDebug() << "Erreur de pr�paration de la requ�te : " << query.lastError().text();
        return false;
    }

    // Lier les valeurs aux param�tres
    query.bindValue(":num", num);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":prix", prix);
    query.bindValue(":etage", etage);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ex�cution de la requ�te : " << query.lastError().text();
        return false;
    } else {

        return true;
    }
}
bool Chambre::supprimer(int num)
{
    QSqlQuery query;
    QString res = QString::number(num);

    query.prepare("Delete from CHAMBRES where NUM= :num");
    query.bindValue(":num", res);

    return query.exec();
}
QSqlQueryModel * Chambre::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();

    model->setQuery("select * from CHAMBRES");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("NUM"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ETAT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ETAGE"));


    return model;
}
bool Chambre::modifier(int num,QString etat) {
    QSqlQuery query;

    // Pr�pare la requ�te SQL pour mettre � jour l'�tat de la chambre avec le num�ro sp�cifique
    query.prepare("UPDATE CHAMBRES SET ETAT = :etat WHERE NUM = :num");
    query.bindValue(":num", num);  // Utilise la variable membre 'num' de l'objet Chambre
    query.bindValue(":etat", etat); // Utilise la variable membre 'etat' de l'objet Chambre

    // Ex�cute la requ�te et v�rifie si elle a r�ussi
    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise � jour de l'�tat de la chambre : " << query.lastError().text();
        return false;
    }

    return true;
}


