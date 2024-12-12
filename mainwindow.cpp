#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "service.h"
#include <QMessageBox>
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QPainter>
#include <QChartView>
#include <QPieSlice>
#include <QPieSeries>
#include <QList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQuery>
#include "Qrcode.hpp"


#include <QInputDialog>
#include <QPdfWriter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    refreshTable();
     stat();
     initUI();
     int ret=A.connect_arduino(); // lancer la connexion à arduino
         switch(ret){
         case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
             break;
         case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
            break;
         case(-1):qDebug() << "arduino is not available";
         }
          QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
          //le slot update_label suite à la reception du signal readyRead (reception des données).

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pb_ajouter_clicked() {
    int ID = ui->lineEdit->text().toInt();
    QString TYPE = ui->lineEdit_2->currentText();
    int PRIX = ui->lineEdit_3->text().toInt();


    service s(ID, TYPE, PRIX);
    bool test =s.ajouter();
    if (test) {
        QMessageBox::information(this, "Ajouter un service", "Client ajouté avec succès.");
        refreshTable();
        stat();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du service.");
    }



}
void MainWindow::on_pb_supprimer_clicked() {
    int ID = ui->lineEdit->text().toInt();
    if (tmpService.supprimer(ID)) {
        QMessageBox::information(this, "Supprimer un service", "Service supprimé avec succès.");
        refreshTable();
         stat();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du service.");
    }
}
void MainWindow::on_pb_modifier_clicked() {
    int ID = ui->lineEdit->text().toInt();
    QString TYPE = ui->lineEdit_2->currentText();
    int PRIX = ui->lineEdit_3->text().toInt();


    service s(ID, TYPE, PRIX);
    bool test = s.modifier();

    if (test) {
        QMessageBox::information(this, "Modifier un service", "service modifié avec succès.");
        refreshTable();
        stat();

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du service.");
    }
}
void MainWindow::refreshTable() {
    QSqlQueryModel *model = tmpService.afficher();
    ui->tableView_repas->setModel(model);
}




void MainWindow::on_tableView_repas_clicked(const QModelIndex &index)
{
    QAbstractItemModel* model = ui->tableView_repas->model();

        int row = index.row();



        QString ID = model->data(model->index(row, 0)).toString();
        QString TYPE = model->data(model->index(row, 1)).toString();
        QString PRIX = model->data(model->index(row, 2)).toString();

ui->lineEdit->setText(ID);
ui->lineEdit_2->setCurrentText(TYPE);
ui->lineEdit_3->setText(PRIX);


// Using QrCodegen to create a QR code from Facture attributes
                 QString text = "ID: " + ID + "\n"
                                 "TYPE: " + TYPE + "\n"
                                 "PRIX: " + PRIX + "\n";

 using namespace qrcodegen;
QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::MEDIUM);

                 qint32 sz = qr.getSize();
                 QImage im(sz, sz, QImage::Format_RGB32);
                 QRgb black = qRgb(0, 0, 0);
                 QRgb white = qRgb(255, 255, 255);

                 for (int y = 0; y < sz; y++) {
                     for (int x = 0; x < sz; x++) {
                         im.setPixel(x, y, qr.getModule(x, y) ? black : white);
                     }
                 }
                 ui->qrcode->setPixmap(QPixmap::fromImage(im.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation), Qt::MonoOnly));


}

void MainWindow::on_PDF_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableView_repas->model()->rowCount();
    const int columnCount = ui->tableView_repas->model()->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           "<title>%1</title>\n"
           "<style>\n"
           "table {\n"
           "    width: 100%;\n"
           "    border-collapse: collapse;\n"
           "}\n"
           "th, td {\n"
           "    padding: 8px;\n"
           "    text-align: left;\n"
           "    border-bottom: 1px solid #ddd;\n"
           "}\n"
           "tr:nth-child(even) {\n"
           "    background-color: #f2f2f2;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center> <H1>Liste des services</H1></center><br/><br/>\n"
           "<img src=\"path/to/your/image.jpg\" alt=\"Description of image\" style=\"max-width: 100%; height: auto;\">\n"
           "<table>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    {
        if (!ui->tableView_repas->isColumnHidden(column))
        {
            out << QString("<th>%1</th>").arg(ui->tableView_repas->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tableView_repas->isColumnHidden(column))
            {
                QString data = ui->tableView_repas->model()->data(ui->tableView_repas->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }



    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);

}

void MainWindow::on_pushButton_afficher_clicked()
{
    ui->tableView_repas->setModel(tmpService.afficher());

}

void MainWindow::on_pushButton_rechercher_clicked()
{
    QString text=ui->lineEdit_5->text();
    ui->tableView_repas->setModel(tmpService.chercher("ID",text));

}

void MainWindow::on_pushButton_trier_clicked()
{
    QString choix=ui->comboBox_choix->currentText();
    QString ordre=ui->comboBox_ordre->currentText();

        QString order = (ordre == "ASCENDANT") ? "ASC" : "DESC";

        // List of valid columns
        QStringList validColumns = {"ID", "TYPE", "PRIX"};

        // Check if the selected column is valid
        if (validColumns.contains(choix)) {
            // Set the model based on the selected column and order
            ui->tableView_repas->setModel(tmpService.tri(choix, order));
        } else {
            // If the column is not valid, display the default model
            ui->tableView_repas->setModel(tmpService.afficher());
        }

}

void MainWindow::stat()
{
    //mise a jour
    QList<QWidget*> childWidgets = ui->label_stat->findChildren<QWidget*>();
        for (QWidget* childWidget : childWidgets) {
            childWidget->deleteLater();

        }
        //the clear didnt work, but my goal is when i second click this button it deleted old chart and renders a new one
        ui->label_stat->clear();
        ui->label_stat->hide();

    int s0, s1,s2,s3;

    s0 = tmpService.countType("Nettoyage");//calculer 9dh mawjouda men mara!!
    s1 = tmpService.countType("Restaurant");
    s2 = tmpService.countType("Piscine");
    s3 = tmpService.countType("Ascenseurs");




    int total = s0 + s1 +s2+s3  ;
    // Calculate percentages
        float x = (total != 0) ? (s0 * 100.0f) / total : 0.0f; // calcule du pourcentage ! nettoyage 9dh pourcentage mta3hA
        float x1 = (total != 0) ? (s1 * 100.0f) / total : 0.0f;
        float x2 = (total != 0) ? (s2 * 100.0f) / total : 0.0f;
        float x3 = (total != 0) ? (s3 * 100.0f) / total : 0.0f;


        // Adjust the percentages to ensure they sum up to 100%
        float totalPercentage = x + x1 +x2+x3 ;
        if (totalPercentage != 100.0f && total != 0) {
            float correction = 100.0f - totalPercentage;
            x += correction;  // Apply correction to one of the slices (usually the largest one)
        }



//qDebug() <<  x <<endl;
// 3 jour  33.00 %
    QString ch1 = QString("Nettoyage %1%").arg(QString::number(x, 'f', 2));
    QString ch2 = QString("Restaurant %2%").arg(QString::number(x1, 'f', 2));
    QString ch3 = QString("Piscine %3%").arg(QString::number(x2, 'f', 2));
    QString ch4 = QString("Ascenseurs %4%").arg(QString::number(x3, 'f', 2));






    QPieSeries *series=new QPieSeries();
    series->setHoleSize(0.35);

    QPieSlice *slice= series->append(ch1,x);
    slice->setLabelVisible();
    slice->setLabelColor(QColor(Qt::white));
        slice->setBrush(QColor(Qt::white));//changer couleur

    QPieSlice *slice1= series->append(ch2,x1);
    slice1->setLabelVisible();
    slice1->setLabelColor(QColor(Qt::white));
    slice1->setBrush(QColor(Qt::black));//changer couleur

    QPieSlice *slice2= series->append(ch3,x2);
    slice2->setLabelVisible();
    slice2->setLabelColor(QColor(Qt::white));
        slice2->setBrush(QColor(Qt::white));//changer couleur

    QPieSlice *slice3= series->append(ch4,x3);
    slice3->setLabelVisible();
    slice3->setLabelColor(QColor(Qt::white));
    slice3->setBrush(QColor(Qt::black));//changer couleur

    QChart *chart=new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBrush backgroundBrush(QColor(187,93,87,0));
    chart->setBackgroundBrush(backgroundBrush);
    QChartView *chartview=new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setFixedSize(ui->label_stat->size());
    chartview->setParent(ui->label_stat);
    ui->label_stat->setStyleSheet("background:transparent; color:white; ");
    ui->label_stat->show();

}
void MainWindow::initUI() {




    // Création des QListView pour chaque état


    ui->listView->setModel(&todoListModel[0]);
    ui->listView_2->setModel(&todoListModel[1]);
    ui->listView_3->setModel(&todoListModel[2]);




    connect(ui->add, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->rechercher, &QPushButton::clicked, this, &MainWindow::checkService);
    connect(ui->move1, &QPushButton::clicked, this, &MainWindow::moveToInProgress);
    connect(ui->move2, &QPushButton::clicked, this, &MainWindow::moveTodone);
    connect(ui->edit, &QPushButton::clicked, this, &MainWindow::editTask);
    connect(ui->edit1, &QPushButton::clicked, this, &MainWindow::editTask1);
    connect(ui->supp, &QPushButton::clicked, this, &MainWindow::removeTask);
    connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::generateContract);



}
 void MainWindow::checkService() {
        QString serviceId = ui->lineEdit_10->text();

        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM service WHERE ID = :id");
        query.bindValue(":id", serviceId);
        query.exec();
        query.next();

        int count = query.value(0).toInt();
        if (count > 0) {
            if (todoLists.contains(serviceId)) {
                displayTodoLists(serviceId);
            } else {
                todoLists[serviceId] = {QStringList(), QStringList(), QStringList()}; // Créer des listes vides pour chaque état
                QMessageBox::information(this, "Info", "Nouvelle liste de tâches créée pour le service " + serviceId);
                displayTodoLists(serviceId); // Afficher les listes vides
            }
        } else {
            // Vider les modèles si l'ID n'existe pas
            todoListModel[0].clear(); // Vider "To Do"
            todoListModel[1].clear(); // Vider "In Progress"
            todoListModel[2].clear(); // Vider "Done"

            QMessageBox::warning(this, "Service Not Found", "L'ID de service saisi n'existe pas.");
        }
    }




 void MainWindow::addTask() {
     bool ok;

     QString serviceId = ui->lineEdit_10->text();
     if (serviceId.isEmpty()) {
         QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
         return;
     }
     QString newTask = QInputDialog::getText(this, "Add Task", "Enter Task", QLineEdit::Normal, "", &ok);

     if (todoLists.contains(serviceId) && !newTask.isEmpty()) {
         todoLists[serviceId].todo.append(newTask); // Ajouter à "To Do"
         displayTodoLists(serviceId);
         newTask.clear();
         saveTodoLists(); // Sauvegarder les listes après ajout
     } else {
         QMessageBox::warning(this, "Erreur", "Veuillez vérifier l'ID du service ou entrer une tâche.");
     }
 }

void MainWindow::displayTodoLists(const QString &serviceId) {
    // Afficher les tâches dans les trois listes
    todoListModel[0].clear();
    todoListModel[1].clear();
    todoListModel[2].clear();

    // Get the TaskLists for the given serviceId
    const TaskLists &taskLists = todoLists[serviceId];

    // Populate the "To Do" list
    for (const QString &task : taskLists.todo) {
        todoListModel[0].appendRow(new QStandardItem(task)); // To Do
    }
    // Populate the "In Progress" list
    for (const QString &task : taskLists.inProgress) {
        todoListModel[1].appendRow(new QStandardItem(task)); // In Progress
    }
    // Populate the "Done" list
    for (const QString &task : taskLists.done) {
        todoListModel[2].appendRow(new QStandardItem(task)); // Done
    }
}

void MainWindow::loadTodoLists() {
    QFile file("/Users/Kaaboura69/Desktop/Atelier_Connexion - Copie/Atelier_Connexion - Copie/todoLists.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return; // Ne rien faire si le fichier n'existe pas
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(":");
        if (parts.size() == 4) {
            QString serviceId = parts[0];
            QStringList todo = parts[1].split(",");
            QStringList inProgress = parts[2].split(",");
            QStringList done = parts[3].split(",");
            todoLists[serviceId] = {todo, inProgress, done};
        }
    }
    file.close();
}

void MainWindow::saveTodoLists() {
    QFile file("/Users/Kaaboura69/Desktop/Atelier_Connexion - Copie/Atelier_Connexion - Copie/todoLists.txt");

    // Ouvrir le fichier en mode écriture
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return; // Si l'ouverture échoue, on sort de la fonction
    }

    QTextStream out(&file);

    // Parcourir les éléments de todoLists
    for (auto it = todoLists.constBegin(); it != todoLists.constEnd(); ++it) {
        QString serviceId = it.key();

        // Assurez-vous que chaque valeur est un TaskLists
        const TaskLists &taskLists = it.value(); // Get the TaskLists object

        QString todo = taskLists.todo.join(",");          // Tâches "To Do"
        QString inProgress = taskLists.inProgress.join(","); // Tâches "In Progress"
        QString done = taskLists.done.join(",");          // Tâches "Done"

        // Écrire dans le fichier
        out << serviceId << ":" << todo << ":" << inProgress << ":" << done << "\n";
    }

    file.close(); // Fermer le fichier
}


void MainWindow::moveToInProgress() {
    QString serviceId = ui->lineEdit_10->text();
    if (serviceId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
        return;
    }

    // Obtenir l'élément sélectionné dans "To Do"
    QModelIndex selectedIndex = ui->listView->currentIndex();
    if (selectedIndex.isValid()) {
        // Get the task to move
        QString task = todoLists[serviceId].todo[selectedIndex.row()]; // Tâche à déplacer

        // Déplacer la tâche de "To Do" vers "In Progress"
        todoLists[serviceId].todo.removeAt(selectedIndex.row()); // Supprimer de "To Do"
        todoLists[serviceId].inProgress.append(task); // Ajouter à "In Progress"

        displayTodoLists(serviceId); // Mettre à jour l'affichage
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une tâche à déplacer.");
    }
}
void MainWindow::moveTodone() {
    QString serviceId = ui->lineEdit_10->text();

    if (serviceId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
        return;
    }

    // Obtenir l'élément sélectionné dans "In Progress"
    QModelIndex selectedIndex = ui->listView_2->currentIndex();
    if (selectedIndex.isValid()) {
        // Get the task to move
        QString task = todoLists[serviceId].inProgress[selectedIndex.row()]; // Tâche à déplacer

        // Déplacer la tâche de "In Progress" vers "Done"
        todoLists[serviceId].inProgress.removeAt(selectedIndex.row()); // Supprimer de "In Progress"
        todoLists[serviceId].done.append(task); // Ajouter à "Done"

        displayTodoLists(serviceId); // Mettre à jour l'affichage
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une tâche à déplacer.");
    }
}


void MainWindow::on_lineEdit_10_textChanged()
{
    QString serviceId = ui->lineEdit_10->text();
    if (serviceId.isEmpty()) {
        todoListModel[0].clear();
        todoListModel[1].clear();
        todoListModel[2].clear();// Vider les résultats si aucun service n'est sélectionné
        return;
    }
}

void MainWindow::editTask() {
    QString serviceId = ui->lineEdit_10->text();
    if (serviceId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
        return;
    }

    QModelIndex selectedIndex = ui->listView->currentIndex();
    if (selectedIndex.isValid()) {
        bool ok;
        QString currentTask = todoLists[serviceId].todo[selectedIndex.row()]; // Access "To Do" tasks

        QString newTask = QInputDialog::getText(this, "Modifier la tâche", "Entrez la nouvelle tâche", QLineEdit::Normal, currentTask, &ok);

        if (ok && !newTask.isEmpty()) {
            todoLists[serviceId].todo[selectedIndex.row()] = newTask; // Modify the task
            displayTodoLists(serviceId);
            saveTodoLists(); // Save after modification
            QMessageBox::information(this, "Succès", "Tâche modifiée avec succès !");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une tâche à modifier.");
    }
}
void MainWindow::editTask1() {
    QString serviceId = ui->lineEdit_10->text();
    if (serviceId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
        return;
    }

    QModelIndex selectedIndex = ui->listView_2->currentIndex();
    if (selectedIndex.isValid()) {
        bool ok;
        QString currentTask = todoLists[serviceId].inProgress[selectedIndex.row()]; // Access "In Progress" tasks

        QString newTask = QInputDialog::getText(this, "Modifier la tâche", "Entrez la nouvelle tâche", QLineEdit::Normal, currentTask, &ok);

        if (ok && !newTask.isEmpty()) {
            todoLists[serviceId].inProgress[selectedIndex.row()] = newTask; // Modify the task
            displayTodoLists(serviceId);
            saveTodoLists(); // Save after modification
            QMessageBox::information(this, "Succès", "Tâche modifiée avec succès !");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une tâche à modifier.");
    }
}
void MainWindow::removeTask() {
    QString serviceId = ui->lineEdit_10->text();
    if (serviceId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
        return;
    }

    QModelIndex selectedIndex = ui->listView->currentIndex();
    if (selectedIndex.isValid()) {
        todoLists[serviceId].todo.removeAt(selectedIndex.row()); // Supprimer de "To Do"
        displayTodoLists(serviceId);
        saveTodoLists(); // Sauvegarder après suppression
        QMessageBox::information(this, "Succès", "Tâche supprimée avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une tâche à supprimer.");
    }
}
void MainWindow::createPDF(const QStringList &doneTasks, const QString &serviceId, const QString &filePath) {
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setFont(QFont("Arial", 12));

    // Écrire le titre
    painter.drawText(100, 100, "Contrat de Tâches pour le Service: " + serviceId);
    painter.drawText(100, 200, "Liste des Tâches Terminées:");

    int yOffset = 300; // Décalage vertical pour les tâches
    for (const QString &task : doneTasks) {
        painter.drawText(100, yOffset, task);
        yOffset += 100; // Espacement entre les tâches
    }

    // Terminer le dessin et sauvegarder le PDF
    painter.end();

    QMessageBox::information(this, "Succès", "Le contrat a été généré et enregistré avec succès : " + filePath);
}
void MainWindow::generateContract() {
    QString serviceId = ui->lineEdit_10->text();

    if (serviceId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de service.");
        return;
    }

    // Vérifier si les listes "To Do" et "In Progress" sont vides
    if (!todoLists[serviceId].todo.isEmpty() || !todoLists[serviceId].inProgress.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Les listes 'To Do' et 'In Progress' doivent être vides pour générer le contrat.");
        return;
    }

    // Rassembler toutes les tâches de "Done"
    QStringList doneTasks = todoLists[serviceId].done; // Access "Done" tasks

    // Vérifier s'il y a des tâches à inclure dans le PDF
    if (doneTasks.isEmpty()) {
        QMessageBox::information(this, "Info", "Aucune tâche terminée à inclure dans le contrat.");
        return;
    }

    // Ouvrir un dialogue de sauvegarde pour choisir le chemin du fichier
    QString pdfFileName = QFileDialog::getSaveFileName(this, "Enregistrer le contrat", "", "Fichiers PDF (*.pdf)");

    if (pdfFileName.isEmpty()) {
        return; // L'utilisateur a annulé le dialogue
    }

    // Générer le PDF
    createPDF(doneTasks, serviceId, pdfFileName);
}
void MainWindow::update_label(){

    data=A.read_from_arduino();
    qDebug()<< data;

    QSqlQuery q;
    q.prepare("Select *  from Service where id = "+data);
    q.bindValue("id",data.toInt());
    q.exec();
    q.first();
    if (q.value(0).toString() != ""  )
    {
        ui->lineEdit->setText(q.value(0).toString());
        ui->lineEdit_3->setText(q.value(2).toString());
    }








}
void MainWindow::on_pushButton_2_clicked()
{
    QString id = ui->lineEdit_5->text();
    qDebug()<< id;
    QSqlQuery q;
    q.prepare("Select *  from Service where id ="+id);
    q.bindValue("id",id.toInt());
    q.exec();
    q.first();
    if (q.value(0).toString() != ""  )
    {
        A.write_to_arduino("1")   ;
         QMessageBox::information(this, "etat", "Service trouvé");
    }
    else
    {
        QMessageBox::critical(this, "etat", "Service non trouvé");
    }

}

