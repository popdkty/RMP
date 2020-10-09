#ifndef ACTION_H
#define ACTION_H
#include <QObject>
#include <cmath>

class action : public QObject
{
    Q_OBJECT
public:
    explicit action(QObject *parent = nullptr);

private:
    double firstNum=0, secondNum=0;
    char operation=NULL;

public slots:
    void init0() {
        firstNum=firstNum*10;
        emit updateScreen(QString::number(firstNum));
    }
    void init1() {
        firstNum=firstNum*10+1;
        emit updateScreen(QString::number(firstNum));
    }
    void init2() {
        firstNum=firstNum*10+2;
        emit updateScreen(QString::number(firstNum));
    }
    void init3() {
        firstNum=firstNum*10+3;
        emit updateScreen(QString::number(firstNum));
    }
    void init4() {
        firstNum=firstNum*10+4;
        emit updateScreen(QString::number(firstNum));
    }
    void init5() {
        firstNum=firstNum*10+5;
        emit updateScreen(QString::number(firstNum));
    }
    void init6() {
        firstNum=firstNum*10+6;
        emit updateScreen(QString::number(firstNum));
    }
    void init7() {
        firstNum=firstNum*10+7;
        emit updateScreen(QString::number(firstNum));
    }
    void init8() {
        firstNum=firstNum*10+8;
        emit updateScreen(QString::number(firstNum));
    }
    void init9() {
        firstNum=firstNum*10+9;
        emit updateScreen(QString::number(firstNum));
    }
    void initPlus(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='+';
        emit updateScreen("");
    }
    void initMinus(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='-';
        emit updateScreen("");
    }
    void initMultiplexion(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='*';
        emit updateScreen("");
    }
    void initDivision(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='/';
        emit updateScreen("");
    }
    void initEquals(){
        if(operation=='+')
            firstNum+=secondNum;
        else if(operation=='-')
            firstNum=secondNum-firstNum;
        else if(operation=='*')
            firstNum*=secondNum;
        else if(operation=='/')
            firstNum=secondNum/firstNum;
        else if (operation=='s')//sqrt
            firstNum=pow(secondNum, 1/firstNum);
        else if (operation=='l')//log
            firstNum=log(secondNum)/log(firstNum);
        else if (operation=='p')//pow
            firstNum=pow(firstNum, secondNum);
        secondNum=0;
        operation=NULL;
        emit updateScreen(QString::number(firstNum));
    }
    void initCE(){
        firstNum=0;
        emit updateScreen(QString::number(firstNum));
    }
    void initC(){
        operation=NULL;
        firstNum=0;
        secondNum=0;
        emit updateScreen("0");
    }
    void initSin(){
        firstNum=sin(firstNum);
        emit updateScreen(QString::number(firstNum));
    }
    void initCos(){
        firstNum=cos(firstNum);
        emit updateScreen(QString::number(firstNum));
    }
    void initTan(){
        firstNum=tan(firstNum);
        emit updateScreen(QString::number(firstNum));
    }
    void initSqrt(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='s';
        emit updateScreen("");
    }
    void initLog(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='l';
        emit updateScreen("");
    }
    void initPow(){
        if(operation==NULL){
            secondNum=firstNum;
            firstNum=0;
        }
        operation='p';
        emit updateScreen("");
    }

signals:
    void updateScreen(QString result);
};

#endif // ACTION_H
