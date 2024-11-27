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
 void handleArduinoData(); // Slot pour traiter les données reçues d'Arduino
 void handleFireAlert();
  void setupArduino();
private:
    void setupOccupiedDays();
    Ui::MainWindow *ui;
    Chambre c;
    Chambre *chambre;
    Arduino myArduino;

};

#endif // MAINWINDOW_H
