#include "arduino.h"

Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort* Arduino::getserial()
{
    return serial;
}

int Arduino::connect_arduino()
{
    arduino_is_available = false;  // Initialisation de la disponibilité
    arduino_port_name = "";
    // Recherche du port Arduino via son vendor et product ID
    //est connecte
    foreach (const QSerialPortInfo& serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }

    qDebug() << "arduino_port_name is :" << arduino_port_name;

    if (arduino_is_available) {
        // Configuration de la communication (débit, bits, etc.)
        serial->setPortName("COM6");
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);   // Débit : 9600 bits/s
            serial->setDataBits(QSerialPort::Data8);      // Longueur des données : 8 bits
            serial->setParity(QSerialPort::NoParity);     // Pas de bit de parité
            serial->setStopBits(QSerialPort::OneStop);    // 1 bit de stop
            serial->setFlowControl(QSerialPort::NoFlowControl);
            qDebug() << "Connexion réussie avec Arduino.";
            return 0; // Succès
        } else {
        qDebug() << "Erreur lors de l'ouverture du port série : " << serial->errorString();
        return 1; // Échec d'ouverture
    }
    }    qDebug() << "Arduino non disponible.";
    return -1; // Arduino non disponible
}
int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0; // Fermé avec succès
    }
    return 1; // Échec de fermeture
}

QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll(); // Récupérer les données reçues
        return data;
    }
    return QByteArray(); // Retourne une chaîne vide si non lisible
}

int Arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d); // Envoyer des données vers Arduino
        return 0;         // Écriture réussie
    } else {
        qDebug() << "Couldn't write to serial!";
        return 1;         // Échec d'écriture
    }
}
