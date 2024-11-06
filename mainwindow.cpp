#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"chambre.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(c.afficher());

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    int num = ui->lineEdit_1->text().toInt();
    QString type = ui->lineEdit_2->text();
    QString etat = ui->lineEdit_4->text();
    float prix = ui->lineEdit_6->text().toFloat();
    int etage = ui->lineEdit_3->text().toInt();
    Chambre c(num,type,etat,prix,etage);
    bool test = c.ajouter();

    if (test) {
        ui->tableView->setModel(c.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
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
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_modifier_clicked()
{
    int num = ui->lineEdit_8->text().toInt(); // Récupère le numéro de chambre
    QString etat = ui->lineEdit_7->text();    // Récupère le nouvel état

    // Créer un objet Chambre temporaire (si nécessaire, mais ici vous pourriez appeler directement la méthode)
    Chambre c;  // L'objet 'c' n'a pas besoin d'être initialisé avec 'num' et 'etat' ici

    // Appeler la méthode pour modifier l'état de la chambre
    bool test = c.modifier(num, etat);  // Appelle la méthode qui modifie l'état selon le numéro de la chambre

     if (test)
    {
        ui->tableView->setModel(c.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Modification effectuée\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Modification non effectuée.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}
