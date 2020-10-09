/**
  * @file mainwindow.cpp
  * @brief main window whith charts
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread = new QThread();
    array = new Sort(500000);
    array->moveToThread(thread);
    chart = new QChart();
    view = new QChartView(ui->widget);
    timer = new QTimer();

    this->createChart();
    this->createChartView();

    connect(ui->start, &QPushButton::clicked, this, &MainWindow::startClicked);
    connect(ui->stop, &QPushButton::clicked, this, &MainWindow::stopClicked);
    connect(thread, &QThread::started, array, &Sort::run);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::startClicked()
{
    array->setRunning(true);
    thread->start();
    timer->start(3000);
    connect(timer, &QTimer::timeout, this, &MainWindow::changeSeries);
    connect(array, &Sort::finished, thread, &QThread::terminate);
    connect(array, &Sort::finished, timer, &QTimer::stop);
    connect(array, &Sort::finished, this, &MainWindow::sortFinished);
}

void MainWindow::stopClicked()
{
    if(thread->isRunning()){
        array->setRunning(false);
        thread->exit();
        timer->stop();
    }
    this->close();
}

void MainWindow::createChart()
{
    series = new QBarSeries;
    this->createSeries();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
}

void MainWindow::createChartView()
{
    view->setRenderHint(QPainter::Antialiasing);
    chart->legend()->setVisible(false);
    view->setChart(chart);
    view->resize(ui->widget->size());
}

QBarSeries* MainWindow::createSeries()
{
    series->append(createSet());
    return series;
}

QBarSet* MainWindow::createSet()
{
    QBarSet *set = new QBarSet("data");
    int* arr;
    arr = array->getArray();
    for (int i=0; i<array->getN(); i++) {
        int buf = 0;
        while(i%1000 != 0 && i!=0){
            buf += arr[i];
            i++;
        }
        buf = buf/1000;
        *set << buf;
    }
    return set;
}

QBarCategoryAxis* MainWindow::createXAxis()
{
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    return axisX;
}

QValueAxis* MainWindow::createYAxis()
{
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    return axisY;
}

void MainWindow::changeSeries()
{
    series->clear();
    series->append(createSet());
    timer->start(3000);
}

void MainWindow::sortFinished()
{
    this->changeSeries();
    timer->stop();
    QMessageBox::information(NULL,QObject::tr("Information"), tr("Sort is complete"));
}
