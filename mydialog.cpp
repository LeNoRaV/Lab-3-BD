#include "mydialog.h"
#include <QLayout>

void MyDialogEnter::getMessageBox(const QString textError,bool error){
    QMessageBox msgBox;
    if(error) msgBox.setIcon(QMessageBox::Critical);
    else msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(textError);
    msgBox.exec();
}

MyDialogEnter::MyDialogEnter(QSqlDatabase* db1, QString* result1,QWidget *parent) : QDialog(parent)
{
    result=result1;
    db=db1;
    setModal(true);
    setWindowTitle("Вход");
    setWindowIcon(QIcon(":/MyPictures/pictures/enter.png"));
    login=new QLineEdit(this);
    QRegularExpression regExp("[1-9]{1}[0-9]{10}");
    login->setValidator(new QRegularExpressionValidator(regExp,this));
    login->setPlaceholderText("Номер телефона");
    password=new QLineEdit(this);
    password->setPlaceholderText("Пароль");
    password->setEchoMode(QLineEdit::Password);

    QPushButton* enter=new QPushButton("&Войти",this);
    QMenuBar* menuBar=new QMenuBar(this);
    QMenu* menu=new QMenu("&Регистрация",this);
    QAction* client=new QAction(tr("&клиента"));
    client->setIcon(QIcon(":/MyPictures/pictures/client.png"));
    menu->addAction(client);
    QAction* tutor=new QAction(tr("&репетитора"));
    tutor->setIcon(QIcon(":/MyPictures/pictures/tutor.png"));
    menu->addAction(tutor);
    menuBar->addMenu(menu);

    QLabel* label=new QLabel("Войти как",this);
    box=new QComboBox(this);
    box->addItems({"клиент","репетитор","администратор"});


    QHBoxLayout* layout1=new QHBoxLayout();
    layout1->addWidget(label);
    layout1->addWidget(box);
    QVBoxLayout* layout2=new QVBoxLayout();
    layout2->addWidget(login);
    layout2->addWidget(password);
    QHBoxLayout* layout3=new QHBoxLayout();
    layout3->setMenuBar(menuBar);
    layout3->addLayout(layout2);
    layout3->addWidget(enter);
    QVBoxLayout* layoutResult=new QVBoxLayout();
    layoutResult->setMenuBar(menuBar);
    layoutResult->addLayout(layout1);
    layoutResult->addLayout(layout3);
    setLayout(layoutResult);

    connect(enter,SIGNAL(pressed()),this,SLOT(slotEnter()));
    connect(menuBar,SIGNAL(triggered(QAction*)),this,SLOT(slotTriggeredMenuBar(QAction*)));
    show();
}

void MyDialogEnter::slotEnter(){
    if(box->currentText()=="администратор"){
        QFile file("D:/bd/lr3/files/admin.txt");
        if(!file.open(QIODevice::ReadOnly)){
            getMessageBox("Вы не можете войти как администратор с этого устройства",true);
            return;
        }
        QString str=file.readAll();
        file.close();
        qDebug()<<str;
        if(login->text()==str.section(",",0,0) || password->text()==str.section(",",2,2)){
            (*result)="админ";
            close();
        }
        else getMessageBox("Ошибка входа: проверьте правильность телефона и пароля",true);
    }
    if(box->currentText()=="репетитор"){
        QSqlQuery query(*db);
        if(!query.exec("SELECT Телефон,Пароль FROM az_tutors;")){
           getMessageBox("Таблица с репетиторами не открывается",true);
           return;
        }
        query.first();
        do{
            if(query.value(0).toString()==login->text() && query.value(1).toString()==password->text()){
                (*result)="репетитор,"+login->text();
                close();
                return;
            }
        }while(query.next());
        getMessageBox("Ошибка входа: проверьте правильность телефона и пароля",true);
    }
    if(box->currentText()=="клиент"){
        QSqlQuery query(*db);
        if(!query.exec("SELECT Телефон,Пароль FROM az_clients;")){
            getMessageBox("Таблица с клиентами не открывается",true);
            return;
         }
        do{
            if(query.value(0).toString()==login->text() && query.value(1).toString()==password->text()){
                (*result)="клиент,"+login->text();
                close();
                return;
            }
        }while(query.next());
        getMessageBox("Ошибка входа: проверьте правильность телефона и пароля",true);
    }
}

void MyDialogEnter::slotTriggeredMenuBar(QAction* action){
    if(action->text()==tr("&клиента")){
        MyDialogRegistration* dialogRegistration=new MyDialogRegistration(db,true);
        dialogRegistration->exec();
    }
    if(action->text()==tr("&репетитора")){
        MyDialogRegistration* dialogRegistration=new MyDialogRegistration(db,false);
        dialogRegistration->exec();
    }
}

void MyDialogRegistration::getMessageBox(const QString textError,bool error){
    QMessageBox msgBox;
    if(error) msgBox.setIcon(QMessageBox::Critical);
    else msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(textError);
    msgBox.exec();
}

MyDialogRegistration::MyDialogRegistration(QSqlDatabase* db1,bool isClient1, QWidget *parent) : QDialog(parent)
{
    isClient=isClient1;
    db=db1;
    setModal(true);

    name=new QLineEdit(this);
    name->setPlaceholderText("ФИО");
    telephone=new QLineEdit(this);
    QRegularExpression regExp("[1-9]{1}[0-9]{10}");
    telephone->setValidator(new QRegularExpressionValidator(regExp,this));
    telephone->setPlaceholderText("Номер телефона");
    password=new QLineEdit(this);
    password->setPlaceholderText("Пароль");
    password->setEchoMode(QLineEdit::Password);

    QVBoxLayout* layoutLineEdit=new QVBoxLayout();
    layoutLineEdit->addWidget(name);
    layoutLineEdit->addWidget(telephone);

    QPushButton* enter;
    if(isClient){
        setWindowTitle("Регистрация");
        setWindowIcon(QIcon(":/MyPictures/pictures/client.png"));
        enter=new QPushButton("&Регистрироваться",this);
    }
    else{
        setWindowIcon(QIcon(":/MyPictures/pictures/tutor.png"));
        setWindowTitle("Оставить заявку");
        enter=new QPushButton("&Оставить заявку",this);
        discipline=new QComboBox(this);
        QSqlQuery query(*db);
        if(!query.exec("SELECT * FROM az_disciplines;")){
            getMessageBox("Таблица с дисциплинами не открывается",true);
            return;
         }
        do{
            QString str=query.value(0).toString();
            discipline->addItem(str);
        }while(query.next());
        discipline->setItemText(0,"<Предмет>");
        region=new QComboBox(this);
        if(!query.exec("SELECT * FROM az_regions;")){
            getMessageBox("Таблица с регионами не открывается",true);
            return;
         }
        do{
            QString str=query.value(0).toString();
            region->addItem(str);
        }while(query.next());
        region->setItemText(0,"<Регион>");
        price=new QLineEdit(this);
        QRegularExpression regExp("[1-9]{1}[0-9]{3}");
        price->setValidator(new QRegularExpressionValidator(regExp,this));
        price->setPlaceholderText("Цена");
        layoutLineEdit->addWidget(discipline);
        layoutLineEdit->addWidget(region);
        layoutLineEdit->addWidget(price);
    }
    layoutLineEdit->addWidget(password);
    QHBoxLayout* layoutResult=new QHBoxLayout();
    layoutResult->addLayout(layoutLineEdit);
    layoutResult->addWidget(enter);
    setLayout(layoutResult);

    connect(enter,SIGNAL(pressed()),this,SLOT(slotEnter()));
    show();
}

void MyDialogRegistration::slotEnter(){
    if(telephone->text().count()!=11) {
        getMessageBox("Ошибка в номере телефона: неверное количество цифр",true);
        return;
    }
    QSqlQuery query(*db);
    if(!query.exec("SELECT * FROM az_clients;")) {
        getMessageBox("Таблица с клиентами не открывается",true);
        return;
     }

    if(isClient){
        if(name->text()==nullptr || password->text()==nullptr) {
            getMessageBox("Ошибка ввода данных: не все поля заполнены",true);
            return;
        }
        if(!query.exec("INSERT INTO az_clients VALUES ('"+telephone->text()+"','"+name->text()+"','"+password->text()+"');")) {
            getMessageBox("Ошибка в номере телефона: этот телефон уже зарегистрирован",true);
            return;
        }
        getMessageBox("Поздравляем! Вы удачно зарегистрировались.",false);
    }
    else{
        if(discipline->currentText()=="<Предмет>"){
                getMessageBox("Ошибка в выборе предмета: он не выбран",true);
                return;
        }
        if(region->currentText()=="<Регион>") {
            getMessageBox("Ошибка в выборе региона: он не выбран",true);
            return;
        }
        if(name->text()==nullptr || password->text()==nullptr || price->text()==nullptr) {
            getMessageBox("Ошибка ввода данных: не все поля заполнены",true);
            return;
        }
        if(!query.exec("INSERT INTO az_reg_tutors VALUES ('"+telephone->text()+"','"+name->text()+"','"+discipline->currentText()+"','"+region->currentText()+"','"+price->text()+"','"+password->text()+"');")) {
            getMessageBox("Ошибка в номере телефона: этот телефон уже зарегистрирован",true);
            return;
        }
        getMessageBox("Поздравляем! Вы оставили заявку на рассмотрение. Пожалуйста, дождитесь ответа от администратора.",false);
    }
    close();
}
