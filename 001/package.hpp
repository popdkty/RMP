/**
 * @file package.hpp
 * @author Alexander Fomichev
 * @brief Лабораторная работа 1
 */
#include <iostream>
using namespace std;

class package{
public:
    int sender, receiver;
    string destignation, protocol;
};

class packageTCP: public package
{
};

class packageUDP: public package
{
};

class packageRDP: public package
{
};

class packageCreater
{
    virtual package* createPackage(ifstream *file) = 0;
};

class packageTCPCreater: public packageCreater
{
    public:
    packageTCP* createPackage(ifstream *file) override
    {
        /**
         * @brief Создание объекта TCP-пакета
         * @param file Указатель на файловый поток
         * @return Указатель на объект TCP-пакета
         */
        packageTCP *newpackage = new packageTCP();
        newpackage->protocol = "TCP";
        *file >> newpackage->sender >> newpackage->receiver >> newpackage->destignation;
        return newpackage;
    }
};

class packageUDPCreater: public packageCreater
{
    public:
    packageUDP* createPackage(ifstream *file) override
    {
        /**
         * @brief Создание объекта UDP-пакета
         * @param file Указатель на файловый поток
         * @return Указатель на объект UDP-пакета
         */
        packageUDP *newpackage = new packageUDP();
        newpackage->protocol = "UDP";
        *file >> newpackage->sender >> newpackage->receiver >> newpackage->destignation;
        return newpackage;
    }
};

class packageRDPCreater: public packageCreater
{
    public:
    packageRDP* createPackage(ifstream *file) override
    {
        /**
         * @brief Создание объекта RDP-пакета
         * @param file Указатель на файловый поток
         * @return Указатель на объект RDP-пакета
         */
        packageRDP *newpackage = new packageRDP();
        newpackage->protocol = "RDP";
        *file >> newpackage->sender >> newpackage->receiver >> newpackage->destignation;
        return newpackage;
    }
};
