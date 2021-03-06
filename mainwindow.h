#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>
#include <QLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "mydialog.h"

//Cтруктура участницы
struct Participant{
    QLineEdit* record_book;
    QLineEdit* name;
    QLineEdit* weight;
    QLineEdit* height;
    QLineEdit* age;
    QLineEdit* talant;
};

//Структура номинации
struct Nomination{
    QLineEdit* id;
    QLineEdit* name;
};

struct Person{

    QLineEdit* telephone;
    QLineEdit* name;
    QComboBox* region;
    QLineEdit* price;
    QLineEdit* password;
};

struct Person2{
    QComboBox* region;
    QComboBox* price;
    QComboBox* discipline;
    QComboBox* date;
    QComboBox* time;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QSqlDatabase db;
    QTableView* tableView;
    QSqlQueryModel* model;
    QMenuBar* menuBar;

    QLineEdit* lineEdit;
    QLineEdit* lineEdit2;

    Participant participant;


    Person person;
    Person2 person2;
    QPalette* palette;

    QLabel* label;

    QLabel* infoLabel;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeMenuBar(const QList<QStringList>);  //done

    void adminProfile(); // Админ
    void tutorProfile(QString);
    // void juryProfile(QString); // Жюри
    void clientProfile(QString);
    // void participantProfile(QString); // Участница

    void getMessageBox(const QString,bool);   // done
private slots:
    void slotTriggeredMenuBar(QAction*);

    void slotPlusAndDeleteTutor(QPoint);
    void slotAddTutor();
    void slotDeleteTutor();

    void slotAddAllTutors();
    void slotDeleteAllTutors();

    void slotDeleteDiscipline(QPoint);
    void slotAddDiscipline();
    void slotDeleteDiscipline();

    void slotDeleteRegion(QPoint);
    void slotAddRegion();
    void slotDeleteRegion();

    void slotDeleteTutor2(QPoint);
    void slotDeleteTutor2();

    void slotDeleteClient(QPoint);
    void slotDeleteClient();

    void changeInfoAdmin();

    void slotStatistics(int);

    void changeInfoTutor();

    void slotDeleteLesson(QPoint);
    void slotDeleteLesson();
    void slotAddLesson();

    void changeInfoClient();

    void slotChangeLesson(QPoint);
    void slotChangeLesson();

    void currentTextChanged1(const QString);
    void currentTextChanged2(const QString);
    void currentTextChanged3(const QString);
    void currentTextChanged4(const QString);
    void currentTextChanged5(const QString);

    void slotChooseDiscipline(QPoint);
    void slotChooseDiscipline();
};
#endif // MAINWINDOW_H
