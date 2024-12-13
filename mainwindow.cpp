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
QStringList etatOptions = {"libre", "occupée"};

// CONTROLE SAISIE TYPE
QCompleter *typeCompleter = new QCompleter(typeOptions, this);
typeCompleter->setCaseSensitivity(Qt::CaseInsensitive);

ui->type->setCompleter(typeCompleter);
ui->lineEdit_chercher->setCompleter(typeCompleter);

connect(ui->type, &QLineEdit::editingFinished, [=]() {
    if (!typeOptions.contains(ui->type->text(), Qt::CaseInsensitive)) {
        ui->type->clear();
        QMessageBox::warning(this, "Entrée invalide", "Veuillez entrer 'double', 'simple' ou 'suite'.");
    }

});
connect(ui->lineEdit_chercher, &QLineEdit::editingFinished, [=]() {
    if (!typeOptions.contains(ui->lineEdit_chercher->text(), Qt::CaseInsensitive)) {
        ui->lineEdit_chercher->clear();
        QMessageBox::warning(this, "Entrée invalide", "Veuillez entrer 'double', 'simple' ou 'suite'.");
    }
});

// Controle saisie etat
QCompleter *etatCompleter = new QCompleter(etatOptions, this);
etatCompleter->setCaseSensitivity(Qt::CaseInsensitive);

ui->etat->setCompleter(etatCompleter);
ui->lineEdit_7->setCompleter(etatCompleter);

// Vérification de la saisie pour 'etat'
connect(ui->etat, &QLineEdit::editingFinished, [=]() {
    if (!etatOptions.contains(ui->etat->text(), Qt::CaseInsensitive)) {
        ui->etat->clear();
        QMessageBox::warning(this, "Entrée invalide", "Veuillez entrer 'libre' ou 'occupée'.");
    }
});

// Vérification de la saisie pour 'lineEdit_7'
connect(ui->lineEdit_7, &QLineEdit::editingFinished, [=]() {
    if (!etatOptions.contains(ui->lineEdit_7->text(), Qt::CaseInsensitive)) {
        ui->lineEdit_7->clear();
        QMessageBox::warning(this, "Entrée invalide", "Veuillez entrer 'libre' ou 'occupée'.");
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
        calendar->hide(); // Masquer au début
        layout->addWidget(calendar);

        // Connecter le bouton à l'action pour afficher le calendrier
        connect(calendarButton, &QPushButton::clicked, this, &MainWindow::showCalendar);*/
    // Configurer les jours occupés
    setupOccupiedDays();
    // Connexion à Arduino
        int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
        buzzerTimer = new QTimer(this);  // Créer le timer
            connect(buzzerTimer, &QTimer::timeout, this, &MainWindow::increaseBuzzerVolume);

            // Connecter les événements des boutons
            connect(ui->pushButton, &QPushButton::pressed, this, &MainWindow::onPushButtonPressed);
               connect(ui->pushButton, &QPushButton::released, this, &MainWindow::onPushButtonReleased);
               connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);

               // Autres connexions
               connect(buzzerTimer, &QTimer::timeout, this, &MainWindow::onBuzzerTimeout);


    }

MainWindow::~MainWindow()
{
    delete ui;
    delete buzzerTimer;
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
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
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
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\n"
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
                                 QObject::tr("Modification effectuée\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Modification non effectuée.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_trier_clicked()
{
    Chambre c;
    QString sortOption = "NUM (DES)"; // Vous pouvez changer cette option selon le type de tri désiré

    QSqlQueryModel *model = c.trier(sortOption); // Appelle la méthode de tri de la classe Chambre

    // Vérifie si le modèle contient des résultats
    if (model && model->rowCount() > 0)
    {
        ui->tableView->setModel(model); // Met à jour la QTableView avec le modèle trié
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Tri effectué avec succès.\n"
                                             "Les résultats sont affichés."),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Le tri n'a pas pu être effectué ou aucun résultat trouvé."),
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
                                 QObject::tr("Recherche effectuée. Les chambres correspondantes au type sont affichées."),
                                 QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Aucune chambre trouvée pour ce type."),
                              QMessageBox::Ok);
    }
}
void MainWindow::on_exporter_clicked() {
    // Exemple de liste de chambres
    std::vector<Chambre> chambres = {
        Chambre(0, "suite", "libre", 1000.0, 0),
        Chambre(210, "suite", "occupée", 1000.0, 2),
        Chambre(204, "double", "occupée", 400.0, 2),
        Chambre(4, "suite", "libre", 1000.0, 0),
        Chambre(409, "suite", "libre", 1000.0, 4)
    };

    // Appeler la fonction d'exportation
    Chambre::exportPDF(chambres);
}
void MainWindow::on_stat_clicked()
{
    // Exemple de données
    std::vector<Chambre> chambres = {
        Chambre(0, "suite", "libre", 1000.0, 0),
        Chambre(210, "suite", "occupée", 1000.0, 2),
        Chambre(204, "double", "occupée", 400.0, 2),
        Chambre(4, "suite", "libre", 1000.0, 0),
        Chambre(409, "suite", "libre", 1000.0, 4),
        Chambre(300, "simple", "libre", 300.0, 3)

    };

    Chambre c; // Création d'un objet Chambre
    c.statistiques(chambres); // Appel de la fonction statistiques()
}
/*void MainWindow::on_calendarButton_clicked() {
    Chambre c; // Instance de la classe Chambre
    QList<QPair<QString, QString>> chambres = c.calendrier(); // Récupérer les données de la base

    // Vérification : Si aucune donnée récupérée, afficher un message d'avertissement
    if (chambres.isEmpty()) {
        QMessageBox::warning(this, "Calendrier", "Aucune chambre trouvée pour le calendrier.");
        return;
    }

    // Créer un modèle pour le tableau
    QStandardItemModel *model = new QStandardItemModel(this);

    // Définir les en-têtes des colonnes (Numéro de chambre + État pour chaque jour)
    QStringList headers = {"Num ", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
    model->setColumnCount(headers.size());
    model->setHorizontalHeaderLabels(headers);

    // Remplir les données dans le modèle
    for (const auto &chambre : chambres) {
        QList<QStandardItem *> rowData;

        // Ajouter le numéro de chambre
        rowData.append(new QStandardItem(chambre.first)); // Numéro de chambre

        // Ajouter l'état pour chaque jour (par défaut, utiliser le même état pour tous les jours)
        for (int i = 1; i < headers.size(); ++i) {
            rowData.append(new QStandardItem(chambre.second)); // État (libre/occupée)
        }

        // Ajouter la ligne au modèle
        model->appendRow(rowData);
    }

    // Lier le modèle au QTableView existant
    ui->calendarTableView->setModel(model); // Utilisez le `tableView` existant pour afficher le calendrier

    // Ajuster les tailles des colonnes pour bien afficher les données
    ui->calendarTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->calendarTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Afficher un message de succès dans la console
    qDebug() << "Calendrier mis à jour. Nombre de lignes :" << model->rowCount();
}
*/

void MainWindow::setupOccupiedDays() {
    // Format pour les jours occupés
    QTextCharFormat occupiedFormat;
    occupiedFormat.setBackground(QBrush(QColor("blue"))); // Fond bleu pour tout le carreau
    occupiedFormat.setForeground(QBrush(QColor("white"))); // Texte blanc pour meilleure lisibilité

    // Liste des jours occupés
    QList<QDate> occupiedDays = {
        QDate(2024, 11, 10),
        QDate(2024, 11, 15),
        QDate(2024, 11, 18),
        QDate(2024, 10, 04),
        QDate(2024, 12, 22)


    };

    // Appliquer le format aux jours occupés
    for (const QDate &date : occupiedDays) {
        ui->calendarWidget->setDateTextFormat(date, occupiedFormat);
    }
}
void MainWindow::openMap()
{
    // Déclare une chaîne de caractères contenant le chemin d'accès complet au fichier HTML
    QString filePath = "file:///C:/Users/MSI/Downloads/Atelier_Connexion/Bureau/Atelier_Connexion/map.html";
    // Utiliser la classe QDesktopServices pour ouvrir une URL avec l'application par défaut du système
    //convertir le chemin d'accès en objet QUrl pour le passer à la méthode openUrl
    QDesktopServices::openUrl(QUrl(filePath));
}
void MainWindow::update_label()
{
    // Lire les données reçues d'Arduino
    data = A.read_from_arduino();

    // Mettre à jour les labels en fonction des données reçues
    if (data == "1") {
        ui->label_3->setText("Feu détecté !");
    } else if (data == "0") {
        ui->label_3->setText("Aucun feu détecté.");
    } else {
        ui->label_3->setText("Données : " + QString(data));
    }
}


void MainWindow::onPushButtonPressed() {
    qDebug() << "Le bouton ON est pressé, volume du buzzer va augmenter.";
    buzzerTimer->start(2000); // Démarre l'augmentation du volume
}

void MainWindow::onPushButtonReleased() {
    qDebug() << "Le bouton ON est relâché, arrêt de l'augmentation du volume.";
    buzzerTimer->stop(); // Arrête l'augmentation du volume
}

void MainWindow::increaseBuzzerVolume() {
    // Affiche un message lorsque la fonction est appelée
    qDebug() << "increaseBuzzerVolume appelé.";

    // Augmenter le volume du buzzer (par exemple, augmenter de 1 à chaque appel)
    buzzerVolume++;

    // Vérifie si le volume n'excède pas une valeur maximale (par exemple, 100)
    if (buzzerVolume > 100) {
        buzzerVolume = 100;  // Limite maximale
    }

    // Afficher le volume actuel du buzzer dans la sortie du débogueur
    qDebug() << "Le volume du buzzer augmente à" << buzzerVolume;

    // Tu peux aussi envoyer cette valeur à Arduino si nécessaire
    QByteArray command = QByteArray::number(buzzerVolume);  // Exemple de conversion en QByteArray
    A.write_to_arduino(command);
}


void MainWindow::on_pushButton_clicked()  // Bouton ON
{
    A.write_to_arduino("1");  // Activer le buzzer
    qDebug() << "Buzzer activé.";
}

void MainWindow::on_pushButton_2_clicked()  // Bouton OFF
{
    A.write_to_arduino("0");  // Désactiver le buzzer
    buzzerTimer->stop();      // Arrêter le timer
    buzzerVolume = 10;        // Réinitialiser le volume
    qDebug() << "Buzzer désactivé et timer arrêté.";
}

void MainWindow::onBuzzerTimeout() {
    // Logique de la méthode lorsque le timer expire
    qDebug() << "Timer expiré, augmenter le volume du buzzer.";
    // Implémenter le comportement souhaité, par exemple augmenter le volume
}
