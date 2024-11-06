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
    int num = ui->lineEdit_8->text().toInt(); // R�cup�re le num�ro de chambre
    QString etat = ui->lineEdit_7->text();    // R�cup�re le nouvel �tat

    // Cr�er un objet Chambre temporaire (si n�cessaire, mais ici vous pourriez appeler directement la m�thode)
    Chambre c;  // L'objet 'c' n'a pas besoin d'�tre initialis� avec 'num' et 'etat' ici

    // Appeler la m�thode pour modifier l'�tat de la chambre
    bool test = c.modifier(num, etat);  // Appelle la m�thode qui modifie l'�tat selon le num�ro de la chambre

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
