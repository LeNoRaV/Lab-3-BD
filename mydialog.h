#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "mainwindow.h"

//Диалог входа
class MyDialogEnter : public QDialog
{
    Q_OBJECT

private:
    QSqlDatabase* db;
    QLineEdit* login;
    QLineEdit* password;
    QComboBox* box;

    QString* result;
public:
    explicit MyDialogEnter(QSqlDatabase* db1, QString* result1, QWidget *parent = nullptr);

    void getMessageBox(const QString textError,bool error);
private slots:
    void slotEnter();
    void slotTriggeredMenuBar(QAction*);
};


//Диалог регистрации (и клиентов, и репетиторов)
class MyDialogRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialogRegistration(QSqlDatabase* db1, bool isClient, QWidget *parent=nullptr);

    void getMessageBox(const QString textError,bool error);
private:
    QSqlDatabase* db;

    QLineEdit* telephone;
    QLineEdit* name;
    QComboBox* discipline;
    QComboBox* region;
    QLineEdit* price;
    QLineEdit* password;

    bool isClient;
private slots:
    void slotEnter();
};

#endif // MYDIALOG_H
