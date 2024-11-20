#include "mainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include "connection.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection connection;
     bool test=connection.createconnection();
     MainWindow w;
     if (test){ QMessageBox::information (nullptr, QObject::tr("OK"),
                                          QObject::tr("Connection successful \n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);

      w.show();
     }
     else{
         QMessageBox::critical (nullptr, QObject::tr("database is not open"),
                                    QObject::tr(" connection failed  \n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
     };

    return a.exec();
}
