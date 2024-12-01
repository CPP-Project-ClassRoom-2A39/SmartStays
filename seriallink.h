#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>

class seriallink : public QObject
{
    Q_OBJECT

public:
    explicit seriallink(QObject *parent = nullptr);
    ~seriallink();

    void openConnection();
    void closeConnection();
    void write(const char *messageToWrite);
    bool isOpen() const;
    bool isWritable();
signals:
    void connectionStatus(bool connected);  // Déclaration du signal connectionStatus
private slots:
    void updateGUI(const QByteArray &data);  // Fonction qui sera appelée quand il y a de nouvelles données sur le port

private:
    QSerialPort _serial;  // Déclaration de l'objet QSerialPort
};

#endif // SERIALLINK_H
