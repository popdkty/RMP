#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include "action.h"

QT_BEGIN_NAMESPACE
namespace Ui { class calculator; }
QT_END_NAMESPACE

class calculator : public QWidget
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();

private:
    Ui::calculator *ui;
    action* actionObject;
};
#endif // CALCULATOR_H
