#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QTableWidget>
#include<QPushButton>
#include<QLabel>
#include<QStatusBar>
#include<QAction>
#include<QMenu>
#include<QMessageBox>
#include<QInputDialog>
#include<QDialog>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QFormLayout>
#include<QLineEdit>
#include<QSpinBox>
#include<QComboBox>
#include<QDialogButtonBox>

const int MAX_STUDENTS=100;
const int MAX_SUBJECTS =5;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget*parent=nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionSave_triggered();
    void on_actionAddStudent_triggered();
    void on_actionShowAll_triggered();
    void addStudent();
    void displayReport();

private:
    Ui::MainWindow*ui;
    QTableWidget*tableWidget;
    QPushButton*addButton;
    QPushButton*displayButton;
    QLabel*statusLabel;

    QString studentNames[MAX_STUDENTS];
    int studentIDs[MAX_STUDENTS];
    float studentAverages[MAX_STUDENTS];
    char studentGrades[MAX_STUDENTS];
    int studentCount;

    float calculateAverage(int grades[],int size);
    char determineGrade(float average);
    void setupMenus();
    void setupUI();
    void updateTable();

};
#endif
