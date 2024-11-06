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

    // Préparer la requête SQL
    if (!query.prepare("INSERT INTO CHAMBRES (NUM, TYPE, ETAT, PRIX, ETAGE) VALUES (:num, :type, :etat, :prix, :etage)")) {
        qDebug() << "Erreur de préparation de la requête : " << query.lastError().text();
        return false;
    }

    // Lier les valeurs aux paramètres
    query.bindValue(":num", num);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":prix", prix);
    query.bindValue(":etage", etage);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
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

    // Prépare la requête SQL pour mettre à jour l'état de la chambre avec le numéro spécifique
    query.prepare("UPDATE CHAMBRES SET ETAT = :etat WHERE NUM = :num");
    query.bindValue(":num", num);  // Utilise la variable membre 'num' de l'objet Chambre
    query.bindValue(":etat", etat); // Utilise la variable membre 'etat' de l'objet Chambre

    // Exécute la requête et vérifie si elle a réussi
    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour de l'état de la chambre : " << query.lastError().text();
        return false;
    }

    return true;
}


