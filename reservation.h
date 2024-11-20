#ifndef RESERVATION_H
#define RESERVATION_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMap>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QDialog>
#include <QVBoxLayout>

class Reservation
{
    int id;
    int num;
    QString type;
    QDate dateDebut, dateFin;


public:
    Reservation();
    Reservation(int, QDate, QDate, QString);

    int getId() { return id; }
    int getNum() { return num; }
    QDate getDateDebut() { return dateDebut; }
    QDate getDateFin() { return dateFin; }
    QString getType() { return type; }

    void setId(int i) { id = i; }
    void setNum(int n) { num = n; }
    void setDateDebut(QDate debut) { dateDebut = debut; }
    void setDateFin(QDate fin) { dateFin = fin; }
    void setType(QString t) { type = t; }

    bool ajouter();
    bool supprimer(int);
    bool modifier(int);
    QSqlQueryModel* afficher();
    QSqlQueryModel* trier(QString condition, bool ascendant);
    void recherche(int id);
    QMap<QString, int> statistiquesParType();


};

#endif // RESERVATION_H
