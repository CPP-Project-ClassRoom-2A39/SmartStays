#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"chambre.h"
#include "arduino.h"
#include <QDebug>
#include <QMessageBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QCompleter>
#include <QStringList>
#include <QCompleter>
#include <QStringList>
#include <QStandardItemModel>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QFileInfo>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QDate>
#include <QList>
#include <QBrush>
#include <QColor>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QUrl>
#include <QDesktopServices>
#include"arduino.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(c.afficher());  
      // Configurer la connexion � Arduino
      if (myArduino.connect_arduino() == 0) {
          qDebug() << "Connexion r�ussie � Arduino sur le port" << myArduino.getarduino_port_name();

          // Connecter le signal de donn�es s�rie d'Arduino au slot handleArduinoData
          connect(myArduino.getserial(), &QSerialPort::readyRead, this, &MainWindow::handleArduinoData);
      } else {
          QMessageBox::critical(this, "Erreur", "Impossible de connecter � Arduino.");
      }
    //controle de saisie
ui->prix->setValidator(new QIntValidator(0, 99999, this));
ui->num->setValidator(new QIntValidator(0, 99999, this));
ui->etage->setValidator(new QIntValidator(0, 99, this));
ui->lineEdit_supprimer->setValidator(new QIntValidator(0, 99999, this));
ui->lineEdit_8->setValidator(new QIntValidator(0, 99999, this));

connect(ui->pushButton_ajouter, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->pushButton_supprimer, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_clicked);
    connect(ui->pushButton_modifier, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifier_clicked);
    connect(ui->pushButton_trier, &QPushButton::clicked, this, &MainWindow::on_pushButton_trier_clicked);
    connect(ui->pushButton_rechercher, &QPushButton::clicked, this, &MainWindow::on_pushButton_rechercher_clicked);
    connect(ui->exporter, &QPushButton::clicked, this, &MainWindow::on_exporter_clicked);
QStringList typeOptions = {"double", "simple", "suite"};
QStringList etatOptions = {"libre", "occup�e"};

// CONTROLE SAISIE TYPE
QCompleter *typeCompleter = new QCompleter(typeOptions, this);
typeCompleter->setCaseSensitivity(Qt::CaseInsensitive);

ui->type->setCompleter(typeCompleter);
ui->lineEdit_chercher->setCompleter(typeCompleter);

connect(ui->type, &QLineEdit::editingFinished, [=]() {
    if (!typeOptions.contains(ui->type->text(), Qt::CaseInsensitive)) {
        ui->type->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'double', 'simple' ou 'suite'.");
    }

});
connect(ui->lineEdit_chercher, &QLineEdit::editingFinished, [=]() {
    if (!typeOptions.contains(ui->lineEdit_chercher->text(), Qt::CaseInsensitive)) {
        ui->lineEdit_chercher->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'double', 'simple' ou 'suite'.");
    }
});

// Controle saisie etat
QCompleter *etatCompleter = new QCompleter(etatOptions, this);
etatCompleter->setCaseSensitivity(Qt::CaseInsensitive);

ui->etat->setCompleter(etatCompleter);
ui->lineEdit_7->setCompleter(etatCompleter);

// V�rification de la saisie pour 'etat'
connect(ui->etat, &QLineEdit::editingFinished, [=]() {
    if (!etatOptions.contains(ui->etat->text(), Qt::CaseInsensitive)) {
        ui->etat->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'libre' ou 'occup�e'.");
    }
});

// V�rification de la saisie pour 'lineEdit_7'
connect(ui->lineEdit_7, &QLineEdit::editingFinished, [=]() {
    if (!etatOptions.contains(ui->lineEdit_7->text(), Qt::CaseInsensitive)) {
        ui->lineEdit_7->clear();
        QMessageBox::warning(this, "Entr�e invalide", "Veuillez entrer 'libre' ou 'occup�e'.");
    }
});
    connect(ui->exporter, &QPushButton::clicked, this, &MainWindow::on_exporter_clicked);
    QPushButton* stat = new QPushButton("Afficher Statistiques", this);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::on_stat_clicked);

/*ui->setupUi(this);
   // connect(ui->calendarButton, &QPushButton::clicked, this, &MainWindow::on_calendarButton_clicked);
   QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        setCentralWidget(centralWidget);

        // Bouton pour afficher le calendrier
        QPushButton *calendarButton = new QPushButton("Afficher le calendrier", this);
        layout->addWidget(calendarButton);

        // Placeholder pour le calendrier
        calendar = new QCalendarWidget(this);
        calendar->hide(); // Masquer au d�but
        layout->addWidget(calendar);

        // Connecter le bouton � l'action pour afficher le calendrier
        connect(calendarButton, &QPushButton::clicked, this, &MainWindow::showCalendar);*/
    // Configurer les jours occup�s
    setupOccupiedDays();
   // ui->setupUi(this);

    // Connecter le bouton au slot
    connect(ui->buttonAfficherCarte, &QPushButton::clicked, this, &MainWindow::openMap);

    }

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_ajouter_clicked()
{
    int num = ui->num->text().toInt();
    QString type = ui->type->text();
    QString etat = ui->etage->text();
    float prix = ui->prix->text().toFloat();
    int etage = ui->etage->text().toInt();
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
    int num = ui->lineEdit_8->text().toInt();
    QString etat = ui->lineEdit_7->text();

    Chambre c;

    bool test = c.modifier(num, etat);

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
void MainWindow::on_pushButton_trier_clicked()
{
    Chambre c;
    QString sortOption = "NUM (DES)"; // Vous pouvez changer cette option selon le type de tri d�sir�

    QSqlQueryModel *model = c.trier(sortOption); // Appelle la m�thode de tri de la classe Chambre

    // V�rifie si le mod�le contient des r�sultats
    if (model && model->rowCount() > 0)
    {
        ui->tableView->setModel(model); // Met � jour la QTableView avec le mod�le tri�
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Tri effectu� avec succ�s.\n"
                                             "Les r�sultats sont affich�s."),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Le tri n'a pas pu �tre effectu� ou aucun r�sultat trouv�."),
                              QMessageBox::Ok);
    }
}
void MainWindow::on_pushButton_rechercher_clicked()
{
    QString type = ui->lineEdit_chercher->text(); // Retrieve the type input from QLineEdit
    Chambre c;
    QSqlQueryModel *model = c.rechercher(type);  // Assuming you have a method rechercherParType to filter by type

    if (model->rowCount() > 0) {
        ui->tableView->setModel(model); // Display the results in the Rechercher par type QTableView
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Recherche effectu�e. Les chambres correspondantes au type sont affich�es."),
                                 QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Aucune chambre trouv�e pour ce type."),
                              QMessageBox::Ok);
    }
}
void MainWindow::on_exporter_clicked() {
    // Exemple de liste de chambres
    std::vector<Chambre> chambres = {
        Chambre(0, "suite", "libre", 1000.0, 0),
        Chambre(210, "suite", "occup�e", 1000.0, 2),
        Chambre(204, "double", "occup�e", 400.0, 2),
        Chambre(4, "suite", "libre", 1000.0, 0),
        Chambre(409, "suite", "libre", 1000.0, 4)
    };

    // Appeler la fonction d'exportation
    Chambre::exportPDF(chambres);
}
void MainWindow::on_stat_clicked()
{
    // Exemple de donn�es
    std::vector<Chambre> chambres = {
        Chambre(0, "suite", "libre", 1000.0, 0),
        Chambre(210, "suite", "occup�e", 1000.0, 2),
        Chambre(204, "double", "occup�e", 400.0, 2),
        Chambre(4, "suite", "libre", 1000.0, 0),
        Chambre(409, "suite", "olibre", 1000.0, 4)
    };

    Chambre c; // Cr�ation d'un objet Chambre
    c.statistiques(chambres); // Appel de la fonction statistiques()
}
/*void MainWindow::on_calendarButton_clicked() {
    Chambre c; // Instance de la classe Chambre
    QList<QPair<QString, QString>> chambres = c.calendrier(); // R�cup�rer les donn�es de la base

    // V�rification : Si aucune donn�e r�cup�r�e, afficher un message d'avertissement
    if (chambres.isEmpty()) {
        QMessageBox::warning(this, "Calendrier", "Aucune chambre trouv�e pour le calendrier.");
        return;
    }

    // Cr�er un mod�le pour le tableau
    QStandardItemModel *model = new QStandardItemModel(this);

    // D�finir les en-t�tes des colonnes (Num�ro de chambre + �tat pour chaque jour)
    QStringList headers = {"Num ", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
    model->setColumnCount(headers.size());
    model->setHorizontalHeaderLabels(headers);

    // Remplir les donn�es dans le mod�le
    for (const auto &chambre : chambres) {
        QList<QStandardItem *> rowData;

        // Ajouter le num�ro de chambre
        rowData.append(new QStandardItem(chambre.first)); // Num�ro de chambre

        // Ajouter l'�tat pour chaque jour (par d�faut, utiliser le m�me �tat pour tous les jours)
        for (int i = 1; i < headers.size(); ++i) {
            rowData.append(new QStandardItem(chambre.second)); // �tat (libre/occup�e)
        }

        // Ajouter la ligne au mod�le
        model->appendRow(rowData);
    }

    // Lier le mod�le au QTableView existant
    ui->calendarTableView->setModel(model); // Utilisez le `tableView` existant pour afficher le calendrier

    // Ajuster les tailles des colonnes pour bien afficher les donn�es
    ui->calendarTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->calendarTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Afficher un message de succ�s dans la console
    qDebug() << "Calendrier mis � jour. Nombre de lignes :" << model->rowCount();
}
*/

void MainWindow::setupOccupiedDays() {
    // Format pour les jours occup�s
    QTextCharFormat occupiedFormat;
    occupiedFormat.setBackground(QBrush(QColor("blue"))); // Fond bleu pour tout le carreau
    occupiedFormat.setForeground(QBrush(QColor("white"))); // Texte blanc pour meilleure lisibilit�

    // Liste des jours occup�s
    QList<QDate> occupiedDays = {
        QDate(2024, 11, 10),
        QDate(2024, 11, 15),
        QDate(2024, 11, 18),
        QDate(2024, 10, 04),
        QDate(2024, 12, 22)


    };

    // Appliquer le format aux jours occup�s
    for (const QDate &date : occupiedDays) {
        ui->calendarWidget->setDateTextFormat(date, occupiedFormat);
    }
}
void MainWindow::openMap()
{
    // D�clare une cha�ne de caract�res contenant le chemin d'acc�s complet au fichier HTML
    QString filePath = "file:///C:/Users/MSI/Downloads/Atelier_Connexion/Bureau/Atelier_Connexion/map.html";
    // Utiliser la classe QDesktopServices pour ouvrir une URL avec l'application par d�faut du syst�me
    //convertir le chemin d'acc�s en objet QUrl pour le passer � la m�thode openUrl
    QDesktopServices::openUrl(QUrl(filePath));
}
Arduino myArduino;

void MainWindow::setupArduino() {
    if (myArduino.connect_arduino() == 0) {
        qDebug() << "Connexion � Arduino r�ussie sur le port" << myArduino.getarduino_port_name();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de connecter � Arduino.");
    }
}
void MainWindow::handleFireAlert() {
    QMessageBox::warning(this, "Alerte Incendie", "Un incendie a �t� d�tect� par le capteur !");
}
void MainWindow::handleArduinoData() {
    // Lire les donn�es re�ues d'Arduino
    QByteArray data = myArduino.getserial()->readAll();
    qDebug() << "Donn�es re�ues d'Arduino : " << data;

    // Exemple : v�rifier si l'Arduino signale un incendie
    if (data.contains("fire")) {
        handleFireAlert(); // Appelle la fonction pour g�rer l'alerte
    }
}
