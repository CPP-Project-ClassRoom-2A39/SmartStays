#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "connection.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "chambre.h"
#include <QApplication>
#include <QDesktopServices>
#include <QTextBrowser>
#include <QApplication>
#include <QProcess>
#include <QUrl>
#include <QFile>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QDate>
#include <QList>
#include <QBrush>
#include <QColor>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createconnect();
    MainWindow w;

    if (test) {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                 QObject::tr("Connection successful.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }



    return a.exec();
}
