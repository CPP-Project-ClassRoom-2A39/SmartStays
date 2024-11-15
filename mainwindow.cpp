#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"chambre.h"
#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QCompleter>
#include <QStringList>
#include <QCompleter>
#include <QStringList>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(c.afficher());
    //controle de saisie
ui->prix->setValidator(new QIntValidator(0, 99999, this));
ui->num->setValidator(new QIntValidator(0, 99999, this));
ui->etage->setValidator(new QIntValidator(0, 99, this));
ui->lineEdit_supprimer->setValidator(new QIntValidator(0, 99999, this));
ui->lineEdit_8->setValidator(new QIntValidator(0, 99999, this));


QStringList typeOptions = {"double", "simple", "suite"};
QStringList etatOptions = {"libre", "occup�e"};

// CONTROLE SAISIE TYPE
QCompleter *typeCompleter = new QCompleter(typeOptions, this);
typeCompleter->setCaseSensitivity(Qt::CaseInsensitive);

ui->type->setCompleter(typeCompleter);
ui->lineEdit_chercher->setCompleter(typeCompleter);

connect(ui->type, &QLineEdit::editingFinished, [=]() {
    if (!typeOptions.contains(ui->type->text(), Qt::CaseInsensitive)) {
        ui->type->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'double', 'simple' ou 'suite'.");
    }
});

connect(ui->lineEdit_chercher, &QLineEdit::editingFinished, [=]() {
    if (!typeOptions.contains(ui->lineEdit_chercher->text(), Qt::CaseInsensitive)) {
        ui->lineEdit_chercher->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'double', 'simple' ou 'suite'.");
    }
});

// Controle saisie etat
QCompleter *etatCompleter = new QCompleter(etatOptions, this);
etatCompleter->setCaseSensitivity(Qt::CaseInsensitive);

ui->etat->setCompleter(etatCompleter);
ui->lineEdit_7->setCompleter(etatCompleter);

// V�rification de la saisie pour 'etat'
connect(ui->etat, &QLineEdit::editingFinished, [=]() {
    if (!etatOptions.contains(ui->etat->text(), Qt::CaseInsensitive)) {
        ui->etat->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'libre' ou 'occup�e'.");
    }
});

// V�rification de la saisie pour 'lineEdit_7'
connect(ui->lineEdit_7, &QLineEdit::editingFinished, [=]() {
    if (!etatOptions.contains(ui->lineEdit_7->text(), Qt::CaseInsensitive)) {
        ui->lineEdit_7->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'libre' ou 'occup�e'.");
    }
});
connect(ui->exporter, &QPushButton::clicked, this, &MainWindow::on_exporter_clicked);
QPushButton* stat = new QPushButton("Afficher Statistiques", this);
connect(stat, &QPushButton::clicked, this, &MainWindow::on_stat_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    int num = ui->num->text().toInt();
    QString type = ui->type->text();
    QString etat = ui->etage->text();
    float prix = ui->prix->text().toFloat();
    int etage = ui->etage->text().toInt();
    Chambre c(num,type,etat,prix,etage);
    bool test = c.ajouter();

    if (test) {
        ui->tableView->setModel(c.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectu�\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectu�.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_supprimer_clicked()
{
    int num = ui->lineEdit_supprimer->text().toInt();
    bool test = c.supprimer(num);

    if (test)
    {
        ui->tableView->setModel(c.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectu�e\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectu�e.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_modifier_clicked()
{
    int num = ui->lineEdit_8->text().toInt();
    QString etat = ui->lineEdit_7->text();

    Chambre c;

    bool test = c.modifier(num, etat);

     if (test)
    {
        ui->tableView->setModel(c.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Modification effectu�e\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Modification non effectu�e.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_trier_clicked()
{
    Chambre c;
    QString sortOption = "NUM (DES)"; // Vous pouvez changer cette option selon le type de tri d�sir�

    QSqlQueryModel *model = c.trier(sortOption); // Appelle la m�thode de tri de la classe Chambre

    // V�rifie si le mod�le contient des r�sultats
    if (model && model->rowCount() > 0)
    {
        ui->tableView->setModel(model); // Met � jour la QTableView avec le mod�le tri�
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Tri effectu� avec succ�s.\n"
                                             "Les r�sultats sont affich�s."),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Le tri n'a pas pu �tre effectu� ou aucun r�sultat trouv�."),
                              QMessageBox::Ok);
    }
}
void MainWindow::on_pushButton_rechercher_clicked()
{
    QString type = ui->lineEdit_chercher->text(); // Retrieve the type input from QLineEdit
    Chambre c;
    QSqlQueryModel *model = c.rechercher(type);  // Assuming you have a method rechercherParType to filter by type

    if (model->rowCount() > 0) {
        ui->tableView->setModel(model); // Display the results in the Rechercher par type QTableView
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Recherche effectu�e. Les chambres correspondantes au type sont affich�es."),
                                 QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Aucune chambre trouv�e pour ce type."),
                              QMessageBox::Ok);
    }
}
void MainWindow::on_exporter_clicked() {
    // Exemple de liste de chambres
    std::vector<Chambre> chambres = {
        Chambre(0, "suite", "libre", 1000.0, 0),
        Chambre(210, "suite", "occup�e", 1000.0, 2),
        Chambre(204, "double", "occup�e", 400.0, 2),
        Chambre(4, "suite", "libre", 1000.0, 0),
        Chambre(409, "suite", "olibre", 1000.0, 4)
    };

    // Appeler la fonction d'exportation
    Chambre::exportPDF(chambres);
}
void MainWindow::on_stat_clicked()
{
    // Exemple de donn�es
    std::vector<Chambre> chambres = {
        Chambre(0, "suite", "libre", 1000.0, 0),
        Chambre(210, "suite", "occup�e", 1000.0, 2),
        Chambre(204, "double", "occup�e", 400.0, 2),
        Chambre(4, "suite", "libre", 1000.0, 0),
        Chambre(409, "suite", "olibre", 1000.0, 4)
    };

    Chambre c; // Cr�ation d'un objet Chambre
    c.statistiques(chambres); // Appel de la fonction statistiques()
}
