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
