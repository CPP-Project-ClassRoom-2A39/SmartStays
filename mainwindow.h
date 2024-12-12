#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "service.h"
#include <QListView>
#include <QStandardItemModel>
#include "arduino.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pb_ajouter_clicked();
    void on_pb_supprimer_clicked();
    void on_pb_modifier_clicked();
    void refreshTable();

    void on_tableView_repas_clicked(const QModelIndex &index);

    void on_PDF_clicked();

    void on_pushButton_afficher_clicked();

    void on_pushButton_rechercher_clicked();

    void on_pushButton_trier_clicked();
void stat();
void initUI();
void checkService();
void addTask() ;
void displayTodoLists(const QString &serviceId) ;
void saveTodoLists();
void loadTodoLists() ;
void moveToInProgress();
void moveTodone();
void on_lineEdit_10_textChanged();
void editTask() ;
void editTask1();
void removeTask();
void createPDF(const QStringList &doneTasks, const QString &serviceId, const QString &filePath) ;
void generateContract() ;
void update_label();

void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    service tmpService;
    QStandardItemModel todoListModel[3];
    struct TaskLists {
        QStringList todo;        // Tasks that are "To Do"
        QStringList inProgress;  // Tasks that are "In Progress"
        QStringList done;        // Tasks that are "Done"
    };
    QMap<QString, TaskLists> todoLists;
    Arduino A;
    QByteArray  data;

};

#endif // MAINWINDOW_H











