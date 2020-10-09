#include "paint.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Paint w;
    w.show();
    return a.exec();
}
Листинг 2.2 – Заголовочный файл paint.h
/**
 * @file paint.h
 * @brief window for painting
 * @author Fomichev
 */
#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QFileDialog>

#include <scene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Paint; }
QT_END_NAMESPACE

class Paint : public QWidget
{
  Q_OBJECT
private:
  Ui::Paint *ui;
  Scene* scene;

public:
  Paint(QWidget *parent = nullptr);
  ~Paint();

private slots:
  /**
   * @brief saveImage
   */
  void saveImage();
  /**
   * @brief openImage
   */
  void openImage();
  /**
   * @brief closeImage
   */
  void closeImage();
};
#endif // PAINT_H
Листинг 2.3 – Заголовочный файл paint.cpp
/**
 * @file paint.cpp
 * @brief window for painting
 * @author Fomichev
 */
#include "paint.h"
#include "ui_paint.h"
#include <QMessageBox>

Paint::Paint(QWidget *parent)
    : QWidget(parent), ui(new Ui::Paint)
{
  ui->setupUi(this);
  scene = new Scene();
  ui->graphicsView->setScene(scene);
  QCursor cursor = QCursor(QPixmap(":/Brush.png"));
  ui->graphicsView->setCursor(cursor);

  connect(ui->color, &QComboBox::currentTextChanged,
          scene, &Scene::setColor);
  connect(ui->size, &QLineEdit::textChanged,
          scene, &Scene::setSize);
  connect(ui->save, &QPushButton::clicked,
          this, &Paint::saveImage);
  connect(ui->open, &QPushButton::clicked,
          this, &Paint::openImage);
  connect(ui->close, &QPushButton::clicked,
          this, &Paint::closeImage);
}

Paint::~Paint()
{
  delete ui;
}

void Paint::saveImage()
{
  QString fileName = QFileDialog::getSaveFileName
                   (this, tr("Сохранить файл"),"",
                    tr("Images(* .png * .jpg)"));
  QImage image(ui->graphicsView->scene()->width(),
               ui->graphicsView->scene()->height(),
               QImage::Format_RGB32);
  image.fill(QColor(Qt::white).rgb());
  QPainter painter(&image);
  ui->graphicsView->scene()->render(&painter);
  painter.end();
  if(fileName.indexOf(".png") != -1 || fileName.indexOf(".jpg") != -1)
    image.save(fileName);
  else
    image.save(fileName.append(".png"));
}

void Paint::openImage()
{
  QString fileName = QFileDialog::getOpenFileName
          (this, tr("Открыть файл"),"",
           tr("Images(* .png * .jpg)"));
  Scene* sceneImage = new Scene;
  sceneImage->addPixmap(QPixmap(fileName));
  ui->graphicsView->setScene(sceneImage);
  sceneImage->setSize(ui->size->displayText());
  sceneImage->setColor(ui->color->currentText());
  connect(ui->color, &QComboBox::currentTextChanged,
          sceneImage, &Scene::setColor);
  connect(ui->size, &QLineEdit::textChanged,
          sceneImage, &Scene::setSize);
  QMessageBox::information(NULL, QObject::tr("Информация"), tr("Чтобы сохранить нажмите save"));
}

void Paint::closeImage()
{
  ui->graphicsView->setScene(this->scene);
}
