#include "chambre.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QMessageBox>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QVBoxLayout>
#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QCalendarWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDate>
#include <QSqlDatabase>
#include <QLabel>
#include <QVariant>
#include <QDesktopServices>
#include <QUrl>
#include <QGraphicsItem>
#include <QColor>

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

    if (!query.prepare("INSERT INTO CHAMBRES (NUM, TYPE, ETAT, PRIX, ETAGE) VALUES (:num, :type, :etat, :prix, :etage)")) {
        qDebug() << "Erreur de préparation de la requête : " << query.lastError().text();
        return false;
    }

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

    query.prepare("UPDATE CHAMBRES SET ETAT = :etat WHERE NUM = :num");
    query.bindValue(":num", num);
    query.bindValue(":etat", etat);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour de l'état de la chambre : " << query.lastError().text();
        return false;
    }

    return true;
}
QSqlQueryModel *Chambre::trier(QString x)
{
    QSqlQueryModel * model= new QSqlQueryModel();
    qDebug()<<x;

    if(x=="NUM (DES)")
        model->setQuery("select*  from CHAMBRES order by NUM desc");

    /*else if (x=="Solde (Asc)")
        model->setQuery("select*  from GS_CLIENT order by soldecompte");
    else if (x=="Default")
            model->setQuery("select * from GS_CLIENT");*/


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("NUM"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ETAT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ETAGE"));


        return model;

}
QSqlQueryModel* Chambre::rechercher(QString type)
{
    QSqlQueryModel * model = new QSqlQueryModel();

    QString query = "SELECT * FROM CHAMBRES WHERE type LIKE '" + type + "%'";
    model->setQuery(query);

   model->setHeaderData(0, Qt::Horizontal, QObject::tr("NUM"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ETAT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ETAGE"));

    return model;
}
void Chambre:: exportPDF(const std::vector<Chambre>& chambres) {
    // Sélectionner le chemin de sortie
    //QString filePath = "C:/Users/MSI/Downloads/Atelier_Connexion/Bureau/Atelier_Connexion/Liste_Chambres.pdf";
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty())
        return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Générer le contenu HTML
    QString htmlContent = R"(
    <html>
    <head>
        <style>
            table {
                width: 100%;
                border-collapse: collapse;
            }
            th, td {
                border: 1px solid black;
                padding: 8px;
                text-align: center;
            }
            th {
                background-color: #f2f2f2;
            }
        </style>
    </head>
    <body>
        <h1>Liste des Chambres</h1>
        <table>
            <tr>
                <th>Numéro</th>
                <th>Type</th>
                <th>État</th>
                <th>Prix (€/nuit)</th>
                <th>Étage</th>
            </tr>
)";

for (const Chambre& chambre : chambres) {
    htmlContent += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
                        .arg(chambre.num)
                        .arg(chambre.type)
                        .arg(chambre.etat)
                        .arg(chambre.prix)
                        .arg(chambre.etage);
}

htmlContent += "</table></body></html>";

    // Imprimer le contenu
    QTextDocument document;
    document.setHtml(htmlContent);
    document.print(&printer);

    QMessageBox::information(nullptr, "Exportation PDF", "La liste des chambres a été exportée avec succès !");

}
void Chambre::statistiques(const std::vector<Chambre>& chambres) {
    // Variables pour les statistiques
    int totalChambres = chambres.size();
    int simple = 0, doubles = 0, suites = 0;
    int libres = 0, occupees = 0;
    double sommePrix = 0.0;
    std::map<int, int> distributionEtages; // Clé : étage, Valeur : nombre de chambres

    // Calcul des statistiques
    for (const Chambre& chambre : chambres) {
        // Comptage par type
        if (chambre.type == "simple")
            ++simple;
        else if (chambre.type == "double")
            ++doubles;
        else if (chambre.type == "suite")
            ++suites;

        // Comptage des chambres libres et occupées
        if (chambre.etat == "libre")
            ++libres;
        else if (chambre.etat == "occupée")
            ++occupees;

        // Calcul de la somme des prix
        sommePrix += chambre.prix;

        // Comptage des chambres par étage
        distributionEtages[chambre.etage]++;
    }

    // Calcul du prix moyen
    double prixMoyen = (totalChambres > 0) ? (sommePrix / totalChambres) : 0;

    // Affichage texte des statistiques
    /*QString stats = QString(
        "Statistiques des chambres :\n\n"
        "Nombre total de chambres : %1\n"
        " - Simples : %2\n"
        " - Doubles : %3\n"
        " - Suites : %4\n\n"
        "Chambres disponibles (libres) : %5\n"
        "Chambres occupées : %6\n\n"
        "Prix moyen des chambres : %7 €/nuit\n\n"
        "Répartition par étage :\n"
    ).arg(totalChambres)
     .arg(simples)
     .arg(doubles)
     .arg(suites)
     .arg(libres)
     .arg(occupees)
     .arg(prixMoyen, 0, 'f', 2);

    for (const auto& [etage, count] : distributionEtages) {
        stats += QString(" - Étage %1 : %2 chambre(s)\n").arg(etage).arg(count);
    }

    // Affichage des statistiques sous forme de texte
    QMessageBox::information(nullptr, "Statistiques", stats);
*/
    // Création du graphique circulaire (camembert)
    QPieSeries* series = new QPieSeries();
    series->append("Simples", simple);
    series->append("Doubles", doubles);
    series->append("Suites", suites);

    // Ajouter des labels sur les tranches
    for (QPieSlice* slice : series->slices()) {
        slice->setLabel(QString("%1 : %2").arg(slice->label()).arg(slice->value()));
    }

    // Créer le graphique
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des types de chambres");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Créer une vue pour afficher le graphique
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher le graphique dans une fenêtre modale
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("Graphique des Statistiques");
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);
    dialog->resize(600, 400);
    dialog->exec();
}
/*QList<QPair<QString, QString>> Chambre::calendrier() {
    QList<QPair<QString, QString>> chambres;

    // Vérifiez que la base de données est ouverte
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Erreur : La base de données n'est pas ouverte.";
        return chambres;
    }

    // Exécutez la requête SQL
    QSqlQuery query;
    query.prepare("SELECT NUM, ETAT FROM CHAMBRES");
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        return chambres;
    }

    // Parcourez les résultats
    while (query.next()) {
        QString num = query.value(0).toString();
        QString etat = QString::fromUtf8(query.value(1).toByteArray());
        chambres.append(qMakePair(num, etat));
        qDebug() << "Numéro de chambre:" << num << ", État:" << etat;
    }

    return chambres;
}
*/
