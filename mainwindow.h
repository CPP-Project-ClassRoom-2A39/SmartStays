#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QDate>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "seriallink.h"  // Inclure seriallink pour la communication série
#include "arduino.h"     // Inclure Arduino pour la gestion des données Arduino
#include <QSqlTableModel> // Si nécessaire pour une meilleure organisation


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addToHist(const QString &action, int id);
    void updateGUI(QByteArray data);


private slots:
    // Slots pour les actions liées à l'interface
    void displayArduinoConnection(bool connected);
    void on_btA_clicked();
    void on_BTM_clicked();
    void on_BTRCH_clicked();
    void on_BTSUP_clicked();
    void on_RBdtdebut_toggled(bool checked);
    void on_RBdf_toggled(bool checked);
    void on_RBnom_toggled(bool checked);
    void on_RBC_clicked();
    void on_RBdc_clicked();
    void on_BTRCH_7_clicked();
    void on_BTRCH_8_clicked();
    void on_mail_clicked();
    void on_Histo_clicked();
    void on_buttonOn_clicked();
    void on_buttonOff_clicked();
    void update_label();
    void updateGUI(const QByteArray &data);
    void loadReservations(); // Méthode pour charger les réservations


private:
    // Méthodes pour l'historique
    //void addToHist();
    void writeToHist(const QString &fileN);
    void afficherHistor();
    void checkArduinoPort();

    // Attributs de l'interface
    Ui::MainWindow *ui;
    QByteArray data;
    Arduino A;           // Objet Arduino pour gérer la communication
    seriallink *arduino; // Communication avec Arduino
    QString etattrie;
    QList<QString> historyList;
    QString historiqueFileName = "C:/Users/PC/Desktop/Atelier_Connexion1/histo.txt";
public slots:

    // Assurez-vous que la signature de la fonction est comme suit :


};

#endif // MAINWINDOW_H
