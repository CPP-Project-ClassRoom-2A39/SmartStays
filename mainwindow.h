#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
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
    void afficherHistor();
private:
    void addToHist(const QString &action,int id);
    void writeToHist(const QString &fileN);
    QList <QString> historyList;
    QString historiqueFileName ="C:/Users/PC/Desktop/Atelier_Connexion1/histo.txt";
private:
    Ui::MainWindow *ui;
    QString etattrie;
};
#endif // MAINWINDOW_H
