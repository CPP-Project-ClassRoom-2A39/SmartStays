#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class Arduino
{
public:
    Arduino();
    int connect_arduino(); // Permet de connecter le PC à Arduino
    int close_arduino();   // Permet de fermer la connexion
    int write_to_arduino(QByteArray); // Envoyer des données vers Arduino
    QByteArray read_from_arduino();   // Recevoir des données de la carte Arduino
    QSerialPort* getserial();         // Accesseur
    QString getarduino_port_name();   // Obtenir le nom du port Arduino

private:
    QSerialPort* serial; // Cet objet rassemble des informations sur la voie série
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67; // Correction ici
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data; // Contient les données lues depuis Arduino
};

#endif // ARDUINO_H
