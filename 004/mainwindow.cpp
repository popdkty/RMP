/**
  * @file mainwindow.cpp
  * @author Fomichev
  * @brief Work with database and ui
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  ui->WTable->hide();
  ui->Redactor->hide();

  db = QSqlDatabase::addDatabase("QSQLITE");

  connect(ui->openDB, &QPushButton::clicked, this, &MainWindow::connectDatabase);
  connect(ui->showTable, &QPushButton::clicked, this, &MainWindow::showTable);
  connect(ui->queryTable,  &QPushButton::clicked, this, &MainWindow::initQuery);
  connect(ui->add, &QPushButton::clicked, this, &MainWindow::addStroke);
  connect(ui->delete_2, &QPushButton::clicked, this, &MainWindow::deleteStroke);
  connect(ui->save, &QPushButton::clicked, this, &MainWindow::updateStroke);
  connect(ui->spinBox, &QSpinBox::textChanged, this, &MainWindow::change);
  connect(ui->Table, &QComboBox::currentTextChanged, this, &MainWindow::changeTable);
}

MainWindow::~MainWindow() {
  db.close();
  delete ui;
}

void MainWindow::connectDatabase(){
  db.setDatabaseName(ui->lineEdit->displayText());
  if (db.open()){
    ui->WTable->show();
    ui->Database->hide();
  }
  else {
    qDebug() << "Невозможно открыть БД";
    return;
  }
}

void MainWindow::showTable(){
  QSqlQuery q;
  QSqlTableModel *table = new QSqlTableModel(parent(),db);
  if (!q.exec("SELECT * FROM " + ui->Table->currentText())) {
    createTable(ui->Table->currentText());
  } else {
    table->setTable(ui->Table->currentText());
    table->select();
    ui->tableView->setModel(table);
  }
}

void MainWindow::initQuery(){
  QSqlQuery q;
  QString select = "SELECT " + ui->entFields->displayText() + " FROM " + ui->comboBox->currentText();
  if (ui->where->displayText() != "")
    select+=" WHERE " + ui->where->displayText();
  if (q.exec(select)) {
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery(select);
    ui->tableView->setModel(query);
  } else {
    qDebug() << "Некорректный запрос";
  }
}

void MainWindow::createTable(QString name){
  ui->Redactor->show();
  QSqlQuery q;
  if(tName1 == "0"){
    tName1 = name;
    q.exec("CREATE TABLE " + name + " (sID TEXT PRIMARY KEY NOT NULL, sName TEXT NOT NULL, GPA NUMERIC NOT NULL, sizeH NUMERIC NOT NULL)");
    setNameFields();
  } else if(tName2 == "0"){
    tName2 = name;
    q.exec("CREATE TABLE " + name + " (cName TEXT PRIMARY KEY NOT NULL, state TEXT NOT NULL, enrollment INTEGER NOT NULL)");
    setNameFields();
    clearFields();
  } else
    q.exec("CREATE TABLE " + name + " (id INT PRIMARY KEY AUTOINCREMENT NOT NULL)");
}

void MainWindow::addStroke(){
  QString name = ui->Table->currentText();
  QSqlQuery query;
  if(name == tName1){
    query.prepare("INSERT INTO " + name + "(sID, sName, GPA, sizeH) VALUES (:sID, :sName, :GPA, :sizeH);");
    query.bindValue(":sID", ui->field1->displayText());
    query.bindValue(":sName", ui->field2->displayText());
    query.bindValue(":GPA", ui->field3->displayText());
    query.bindValue(":sizeH", ui->field4->displayText());
  } else if (name == tName2) {
    query.prepare("INSERT INTO " + name + "(cName, state, enrollment) VALUES (:cName, :state, :enrollment);");
    query.bindValue(":cName", ui->field1->displayText());
    query.bindValue(":state", ui->field2->displayText());
    query.bindValue(":enrollment", ui->field3->displayText());
  }
  if(query.exec()){
    qDebug() << "Строка добавлена";
    spinBox();
  }
}

void MainWindow::deleteStroke(){
  QString name = ui->Table->currentText();
  QSqlQuery query;
  if(name == tName1){
    query.prepare("DELETE FROM " + name + " WHERE sID = :sID AND sName = :sName AND GPA = :GPA");
    query.bindValue(":sID", ui->field1->displayText());
    query.bindValue(":sName", ui->field2->displayText());
    query.bindValue(":GPA", ui->field3->displayText());
  } else if (name == tName2) {
    query.prepare("DELETE FROM " + name + " WHERE cName = :cName AND state = :state AND enrollment = :enrollment");
    query.bindValue(":cName", ui->field1->displayText());
    query.bindValue(":state", ui->field2->displayText());
    query.bindValue(":enrollment", ui->field3->displayText());
  }
  if(query.exec()){
    qDebug() << "Строка удалена";
    spinBox();
  }
}

void MainWindow::updateStroke(){
  QString name = ui->Table->currentText();
  QSqlQuery query;
  if(name == tName1){
    query.prepare("UPDATE " + name + " SET sID = :sID , sName = :sName , "
                  " GPA = :GPA, sizeH = :sizeH WHERE sID = :ressID");
    query.bindValue(":sID", ui->field1->displayText());
    query.bindValue(":sName", ui->field2->displayText());
    query.bindValue(":GPA", ui->field3->displayText());
    query.bindValue(":sizeH", ui->field4->displayText());
    query.bindValue(":ressID", this->field);
  } else if (name == tName2) {
    query.prepare("UPDATE " + name + " SET cName = :cName , state = :state , enrollment = :enrollment WHERE cName = :rescName");
    query.bindValue(":cName", ui->field1->displayText());
    query.bindValue(":state", ui->field2->displayText());
    query.bindValue(":enrollment", ui->field3->displayText());
    query.bindValue(":rescName", this->field);
  }
  if(query.exec())
    qDebug() << "Строка отредактирована";
}

void MainWindow::change(QString n){
  int i = n.toInt();
  QString name = ui->Table->currentText();
  QSqlQuery query;
  query.exec("SELECT COUNT(*) FROM " + name);
  query.next();
  if(query.value(0).toInt() > 0){
    query.exec("SELECT * FROM " + name);
    int k=0;
    while(k!=i){
      query.next();
      k++;
    }
    ui->field1->setText(query.value(0).toString());
    ui->field2->setText(query.value(1).toString());
    ui->field3->setText(query.value(2).toString());
    if(ui->Table->currentText() == tName1)
      ui->field4->setText(query.value(3).toString());
    this->field = ui->field1->displayText();
  } else {
    ui->spinBox->setMinimum(0);
    qDebug() << "Нет записей";
    return;
  }
}

void MainWindow::setNameFields(){
  if(ui->Table->currentText() == tName1){
    ui->nameField1->setText("sID");
    ui->nameField2->setText("sName");
    ui->nameField3->setText("GPA");
    ui->nameField4->show();
    ui->field4->show();
  } else if(ui->Table->currentText() == tName2){
    ui->nameField1->setText("cName");
    ui->nameField2->setText("state");
    ui->nameField3->setText("enrollment");
    ui->nameField4->hide();
    ui->field4->hide();
  }
}

void MainWindow::clearFields(){
  ui->field1->clear();
  ui->field2->clear();
  ui->field3->clear();
  ui->field4->clear();
}

void MainWindow::spinBox(){
  QSqlQuery q;
  q.exec("SELECT COUNT(*) FROM " + ui->Table->currentText());
  q.next();
  if(q.value(0).toInt() == 0)
    return;
  ui->spinBox->setMinimum(1);
  ui->spinBox->setMaximum(q.value(0).toInt());
}

void MainWindow::changeTable(QString name){
  if(name == tName1 || name == tName2){
    spinBox();
    setNameFields();
    change("1");
  }
}

