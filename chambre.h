#ifndef CHAMBRE_H
#define CHAMBRE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QGraphicsItem>

class Chambre
{

    QString type,etat,netat;
    float prix;
    int num,etage,numchambre;

public:
    Chambre();
    Chambre(int,QString,QString,float,int);
    QString gettype(){return type;};
    QString getetat(){return etat;};
    QString getnetat(){return netat;};
    float getprix(){return prix;};
    int getnum(){return num;};
    int getetage(){return etage;};
    void settype(QString t){type=t;};
    void setetat(QString e){etat=e;};
    void setnetat(QString e1){netat=e1;};
    void setprix(float p){prix=p;};
    void setnum(int n){num=n;};
    void setnumchambre(int n1){numchambre=n1;};

    void setetage(int ne){etage=ne;};
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool modifier(int,QString);
    QSqlQueryModel *trier(QString);
    QSqlQueryModel* rechercher(QString);
    static void exportPDF(const std::vector<Chambre>& chambres);
    void statistiques(const std::vector<Chambre>& chambres);
QList<QPair<QString, QString>> calendrier();
void afficherCarte();
};

#endif // CHAMBRE_H
