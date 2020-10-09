/**
  * @file mainwindow.h
  * @author Fomichev
  * @brief Work with database and ui
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  QSqlDatabase db;
  Ui::MainWindow *ui;
  QString field;
  QString tName1 = "0", tName2 = "0";

public:
  /**
   * @brief Constructor
   * @param parent
   */
  MainWindow(QWidget *parent = nullptr);
  /**
    *@brief Destructor
    */
  ~MainWindow();
  /**
   * @brief Create Table
   * @param name of Table
   */
  void createTable(QString name);
  /**
   * @brief set name fields for table
   */
  void setNameFields();
  /**
   * @brief clear fields
   */
  void clearFields();
  /**
   * @brief Settings of spinBox
   */
  void spinBox();

public slots:
  /**
   * @brief connect Database
   */
  void connectDatabase();
  /**
   * @brief show Table
   */
  void showTable();
  /**
   * @brief query to database
   */
  void initQuery();
  /**
   * @brief add stroke to table
   */
  void addStroke();
  /**
   * @brief delete stroke from table
   */
  void deleteStroke();
  /**
   * @brief update stroke in table
   */
  void updateStroke();
  /**
   * @brief line-by-line editing
   * @param n - number of stroke
   */
  void change(QString n);
  /**
   * @brief change table
   */
  void changeTable(QString name);};
#endif // MAINWINDOW_H
