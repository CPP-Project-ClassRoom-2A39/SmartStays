#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<reservation.h>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QThread>
#include "generateurpdf.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QSslSocket>
MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent),
  ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->LA_Num->setRange(0, 999999);
    QRegularExpression rx("^[a-zA-Z\\s]+$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->CA_type->setValidator(validator);

    Reservation reservation;
    ui->tab_reservation->setModel(reservation.afficher());

      ui->CA_type->addItem("pension complette");
      ui->CA_type->addItem("demi-pension");

      ui->CM_type->addItem("pension complette");
      ui->CM_type->addItem("demi-pension");




}

MainWindow::~MainWindow()
{
    delete ui;


}
//ajouter
void MainWindow::on_btA_clicked()
{
    // Récupérer les valeurs des champs
    int num = ui->LA_Num->value();
    QString type = ui->CA_type->currentText();
    QDate datdebut = ui->DA_D->date();
    QDate datfin = ui->DA_F->date();

    // Vérifier si tous les champs sont remplis
    if (num == 0 || type.isEmpty() || !datdebut.isValid() || !datfin.isValid()) {
        // Si un champ est vide ou invalide, afficher un message d'erreur
        QMessageBox::warning(this, "Erreur", "Veuillez saisir tous les champs.");
        return;
    }

    // Demander une confirmation à l'utilisateur
    QMessageBox::StandardButton Confirmation;
    Confirmation = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment ajouter cette réservation?",
                                          QMessageBox::Yes | QMessageBox::No);

    // Si l'utilisateur confirme, procéder à l'ajout
    if (Confirmation == QMessageBox::Yes) {
        Reservation r(num, datdebut, datfin, type);
        bool test = r.ajouter();

        // Vérifier si l'ajout a réussi
        if (test) {
            addToHist("Ajout du Reservation", num);  // Log only if the query was successful
            Reservation reservation;
            ui->tab_reservation->setModel(reservation.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué \nClick Cancel to exit."), QMessageBox::Ok);
        } else {
            QMessageBox::information(nullptr, QObject::tr("Not OK"),
                                     QObject::tr("Ajout non effectué \nClick Cancel to exit."), QMessageBox::Cancel);
        }
    }
}
//modifier ok
void MainWindow::on_BTM_clicked()
{
    // Récupérer les valeurs des champs
    int id = ui->LM_ID->text().toInt();
    int num = ui->LM_Num->value();
    QString type = ui->CM_type->currentText();
    QDate datdebut = ui->DM_D->date();
    QDate datfin = ui->DM_F->date();

    // Vérification que tous les champs sont remplis
    if (id == 0 || num == 0 || type.isEmpty() || !datdebut.isValid() || !datfin.isValid()) {
        // Si un des champs est vide ou invalide, afficher un message d'erreur
        QMessageBox::warning(this, "Erreur", "Veuillez saisir tous les champs.");
        return; // Sortir de la fonction pour ne pas procéder à la modification
    }

    // Si tous les champs sont remplis, demander une confirmation
    QMessageBox::StandardButton Confirmation;
    Confirmation = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment modifier cette réservation ?",
                                         QMessageBox::Yes | QMessageBox::No);

    if (Confirmation == QMessageBox::Yes) {
        // Créer un objet de réservation avec les données saisies
        Reservation r(num, datdebut, datfin, type);

        // Appeler la méthode modifier() pour effectuer la modification
        bool test = r.modifier(id);

        // Vérifier si la modification a réussi
        if (test) {
            addToHist("Modification du Reservation ID = ", id);  // Log only if the query was successful

            Reservation reservation;
            ui->tab_reservation->setModel(reservation.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification effectuée \n"
                                                "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
        } else {
            QMessageBox::information(nullptr, QObject::tr("Not OK"),
                                     QObject::tr("Modification non effectuée \n"
                                                "Cliquez sur Cancel pour quitter."), QMessageBox::Cancel);
        }
    }
}
//rechercher
void MainWindow::on_BTRCH_clicked()
{
    // Vérifiez si tous les champs sont remplis
    int id = ui->LM_ID->text().toInt();
    if (id == 0 || ui->LM_Num->value() == 0 || ui->CM_type->currentText().isEmpty() || ui->DM_D->date().isNull() || ui->DM_F->date().isNull()) {
        // Si un champ est vide ou invalide, afficher un message d'erreur
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Veuillez saisir tous les champs : ID, Numéro, Type, Date Début et Date Fin."),
                             QMessageBox::Ok);
    } else {
        // Si tous les champs sont remplis, procédez à la recherche
        Reservation r;
        r.recherche(id);

        if (r.getId() == 0) {
            // Si l'ID n'est pas valide ou trouvé, afficher un message d'information
            QMessageBox::information(nullptr, QObject::tr("Non OK"),
                                     QObject::tr("Veuillez saisir une ID valide.\nCliquez sur Annuler pour quitter."),
                                     QMessageBox::Cancel);
        } else {
            // Si l'ID est valide, remplissez les autres champs avec les valeurs récupérées
            ui->LM_Num->setValue(r.getNum());
            ui->CM_type->setCurrentText(r.getType());
            ui->DM_D->setDate(r.getDateDebut());
            ui->DM_F->setDate(r.getDateFin());
        }
    }
}
//supprimer
void MainWindow::on_BTSUP_clicked()
{
    // Création de l'objet Reservation
    Reservation reservation;

    // Récupération de l'ID
    int id = ui->LS_Id->text().toInt();

    // Vérification si l'ID est valide (si le champ est vide ou l'ID est égal à 0)
    if (ui->LS_Id->text().isEmpty() || id == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir tous les champs.");
        return; // On quitte la fonction si les champs ne sont pas remplis correctement
    }

    // Confirmation de l'action de suppression
    QMessageBox::StandardButton Confirmation;
    Confirmation = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment modifier cette réservation ?",
                                         QMessageBox::Yes | QMessageBox::No);

    // Si l'utilisateur confirme la suppression
    if (Confirmation == QMessageBox::Yes) {
        bool test = reservation.supprimer(id); // Appel à la méthode supprimer() avec l'ID

        // Si la suppression réussie
        if (test) {
            addToHist("suppression du Reservation ID = ", id);  // Log only if the query was successful

            ui->tab_reservation->setModel(reservation.afficher()); // Mise à jour du tableau
            QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Suppression effectuée"), QMessageBox::Cancel);
        } else {
            // Si la suppression échoue
            QMessageBox::information(nullptr, QObject::tr("Not OK"), QObject::tr("Suppression non effectuée"), QMessageBox::Cancel);
        }
    }
}


void MainWindow::on_RBdtdebut_toggled(bool checked)
{
    if(checked){this->etattrie="dateDebut";
    ui->TBtrie->setText("Affichage trier selon date Debut par ordre");}
}

void MainWindow::on_RBdf_toggled(bool checked)
{
    if(checked){this->etattrie="dateFin";
    ui->TBtrie->setText("Affichage trier selon date Fin par ordre");}
}

void MainWindow::on_RBnom_toggled(bool checked)
{
    if(checked){this->etattrie="nom";
    ui->TBtrie->setText("Affichage trier selon nom par ordre");
}
}
void MainWindow::on_RBC_clicked()
{
    if(this->etattrie==""){
       QMessageBox::information (nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Veuillez choisir une option pour trier \n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);
   }
    else {
        Reservation Reservation;
        ui->tab_reservation->setModel(Reservation.trier(this->etattrie,true));
    }
}

void MainWindow::on_RBdc_clicked()
{
    if(this->etattrie==""){
       QMessageBox::information (nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Veuillez choisir une option pour trier \n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);
   }
    else {
        Reservation Reservation;
        ui->tab_reservation->setModel(Reservation.trier(this->etattrie,false));
    }
}

void MainWindow::on_BTRCH_7_clicked()
{
    Reservation Reservation;
    Reservation.statistiquesParType();

}

void MainWindow::on_BTRCH_8_clicked()
{
    QString nomFichierPDF = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");

               if (!nomFichierPDF.isEmpty()) {
                   QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tab_reservation->model());

                   if (model) {
                       GenerateurPDF::genererPDF(model, nomFichierPDF);
                   } else {
                       QMessageBox::warning(this, "Erreur", "Le modèle n'est pas de type QSqlQueryModel.");
                   }
               }
}

void MainWindow::on_mail_clicked()
{
    QString serveur = "smtp.gmail.com";
    int port = 587; // Port pour SMTP avec STARTTLS

    // Informations de l'expéditeur et du destinataire
    QString expediteur = "kmarsrarfi5@gmail.com";
    QString destinataire = ui->reciepient->text();
    QString subject = "Cher notre fidele client";

    // Détails du message
    QString corps = ui->contenu->toPlainText();

    // Vérification des champs
    if (destinataire.isEmpty() || corps.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir votre mail et l'object .");
        return; // Arrêter l'exécution si les champs sont vides
    }

    // Connexion au serveur SMTP
    QSslSocket socket;
    socket.connectToHost(serveur, port);
    if (!socket.waitForConnected()) {
        qDebug() << "Échec de connexion au serveur SMTP:" << socket.errorString();
        return;
    }

    // Attente de la réponse du serveur
    if (!socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP:" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse initiale du serveur

    // Envoi de la commande EHLO
    socket.write("EHLO localhost\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (EHLO):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après EHLO

    // Envoi de la commande STARTTLS
    socket.write("STARTTLS\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (STARTTLS):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après STARTTLS

    // Démarrer le chiffrement
    socket.startClientEncryption();
    if (!socket.waitForEncrypted()) {
        qDebug() << "Échec du chiffrement:" << socket.errorString();
        return;
    }

    // Envoi de la commande AUTH LOGIN
    socket.write("AUTH LOGIN\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (AUTH LOGIN):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après AUTH LOGIN

    // Envoi du nom d'utilisateur encodé en Base64
    socket.write(QByteArray().append(expediteur.toUtf8()).toBase64() + "\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (Nom d'utilisateur):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après l'envoi du nom d'utilisateur

    // Envoi du mot de passe encodé en Base64
    socket.write(QByteArray().append("lzeg felv vatx ztca").toBase64() + "\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Échec de l'authentification SMTP (Mot de passe):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après l'envoi du mot de passe

    // Envoi de la commande MAIL FROM
    socket.write("MAIL FROM:<" + expediteur.toUtf8() + ">\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (MAIL FROM):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après MAIL FROM

    // Envoi de la commande RCPT TO
    socket.write("RCPT TO:<" + destinataire.toUtf8() + ">\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (RCPT TO):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après RCPT TO

    // Envoi de la commande DATA
    socket.write("DATA\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Délai d'attente de réponse du serveur SMTP (DATA):" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après DATA

    // Envoi des en-têtes et du corps du message
    socket.write("From: " + expediteur.toUtf8() + "\r\n");
    socket.write("To: " + destinataire.toUtf8() + "\r\n");
    socket.write("Subject: " + subject.toUtf8() + "\r\n");
    socket.write("\r\n");
    socket.write(corps.toUtf8() + "\r\n");
    socket.write(".\r\n");
    if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
        qDebug() << "Échec d'envoi des données du mail:" << socket.errorString();
        return;
    }
    qDebug() << socket.readAll(); // Afficher la réponse après l'envoi des données du mail

    // Envoi de la commande QUIT
    socket.write("QUIT\r\n");
    if (!socket.waitForBytesWritten()) {
        qDebug() << "Échec d'envoi de la commande QUIT:" << socket.errorString();
        return;
    }

    // Fermeture de la connexion
    socket.close();
    QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès à " + destinataire);
}





void MainWindow::on_Histo_clicked()
{
    QString cheminFichier = "C:/Users/PC/Desktop/Atelier_Connexion1/histo.txt";
     QFile file(cheminFichier);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier d'historique des réservations.");
         return;
     }

     // Lire le contenu du fichier
     QTextStream in(&file);
     QString historique = in.readAll();

     // Fermer le fichier
     file.close();

     // Afficher le contenu de l'historique dans une boîte de dialogue
     QMessageBox msgBox;
     msgBox.setWindowTitle("Historique des Réservations");
     msgBox.setText(historique);
     msgBox.exec();






}
void MainWindow::afficherHistor()
{
    QString cheminFichier = "C:/Users/PC/Desktop/Atelier_Connexion1/histo.txt";

       QFile file(cheminFichier);
       if (!file.open(QIODevice::Append | QIODevice::Text))
       {
           qDebug() << "Erreur lors de l'ouverture du fichier d'historique des réservations.";
           return;
}
}
void MainWindow::writeToHist(const QString &fileN) {
    QFile file(fileN);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique:" << fileN;
        return;
    }

    QTextStream out(&file);
    for (const QString &entry : historyList) {
        out << entry << "\n";
    }

    file.close();
    qDebug() << "Historique enregistré dans le fichier:" << fileN;
}
void MainWindow::addToHist(const QString &action, int id) {
    QString cheminFichier = "C:/Users/PC/Desktop/Atelier_Connexion1/histo.txt";
    QFile file(cheminFichier);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique :" << file.errorString();
        return;
    }

    QTextStream out(&file);
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QString entry = currentDateTime.toString("yyyy-MM-dd hh:mm:ss") + " - " + action + " (ID: " + QString::number(id) + ")";
    qDebug() << "Entrée à écrire dans l'historique :" << entry;
    out << entry << "\n";

    file.close();
    qDebug() << "Entrée ajoutée avec succès à l'historique.";
}
