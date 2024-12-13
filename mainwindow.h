#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chambre.h"
#include <QDebug>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QVector>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QDate>
#include <QList>
#include <QBrush>
#include <QColor>
#include <QTimer>
#include"arduino.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr); // Updated constructor declaration
    ~MainWindow();

    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_rechercher_clicked();
    void on_exporter_clicked();
    void on_stat_clicked();


private slots:
    void openMap();
    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
    // ce slot est lancé à chaque réception d'un message de Arduino
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void onBuzzerTimeout(); // Méthode appelée lorsque le timer expire
    void onPushButtonPressed();
    void onPushButtonReleased();
    void increaseBuzzerVolume();
private:
    void setupOccupiedDays();
    Ui::MainWindow *ui;
    Chambre c;
    Chambre *chambre;
    QByteArray data;  // Données reçues d'Arduino
    Arduino A;        // Objet Arduino pour gérer la communication
    QTimer *buzzerTimer; // Timer pour surveiller l'état
        int buzzerVolume = 10; // Volume initial du buzzerr
        bool offButtonPressed = false;  // Indicateur pour savoir si le bouton OFF est pressé
         QPushButton* offButton; // Référence au bouton OFF


};

#endif // MAINWINDOW_H
