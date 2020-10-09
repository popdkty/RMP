/**
 * @file router.hpp
 * @author Fomichev Alexander
 * @brief Лабораторная работа 1
 */
#include <iostream>

class router
{
    enum status
    {
        denied = 0,
        allowed = 1,
        not_defined=3
    };

public:
    static router& getInstance() {
        /**
         * @brief Возвращает всегда один и тот же объект класса router
         * @return Объект класса router
         */
        static router instance;
        return instance;
    }

    vector<package> input_parser(string path, packageTCPCreater tcpCreater, packageUDPCreater udpCreater, packageRDPCreater rdpCreater)
    {
        /**
         * @brief Парсинг пакетов из файла
         * @param string Путь к файлу
         * @param tcpCreater Объект фабрики TCP-пакетов
         * @param udpCreater Объект фабрики UDP-пакетов
         * @param rdpCreater Объект фабрики RDP-пакетов
         * @exception runtime_error В случае неправильного формата входного файла
         * @return Вектор пакетов
         */
        vector<package> packages;
        ifstream file(path);
        string protocol;
        try{
            while(!file.eof()){
                file >> protocol;
                if(protocol == "TCP")
                    packages.push_back(*tcpCreater.createPackage(&file));
                else if (protocol == "UDP")
                    packages.push_back(*udpCreater.createPackage(&file));
                else if (protocol == "RDP")
                    packages.push_back(*rdpCreater.createPackage(&file));
            }
        }
        catch (exception e){ throw runtime_error("Packages Wrong input"); }
        return packages;
    }

    vector<rule> rule_parser(string path)
    {
        /**
         * @brief Парсинг правил из файла
         * @param string Путь к файлу
         * @exception runtime_error В случае неправильного формата входного файла
         * @return Вектор правил
         */
        vector<rule> rules;
        ifstream file(path);
        rule newrule;
        try{
            while(!file.eof()){
                file >> newrule.source >> newrule.receiver >> newrule.sender >> newrule.destignation >> newrule.protocol >> newrule.status;
                rules.push_back(newrule);
            }
        }
        catch (exception e) { throw runtime_error("Rules Wrong input"); }
        return rules;
    }

    void routePackageVector(vector<package> packages, string interface, vector<rule> rules)
    {
        /**
         * @brief Маршрутизировать вектор пакетов
         * @param packages Вектор пакетов
         * @param interface Название входного интерфейса
         * @param rules Вектор правил
         */
        for(int i = 0; i<packages.size(); i++)
            routePackage(packages[i], interface, rules);
    }

    void routePackage(package package, string interface, vector<rule> rules)
    {
        /**
         * @brief Маршрутизировать пакет
         * @param package Входной пакет
         * @param interface Название входного интерфейса
         * @param rules Вектор правил
         */
        if(packageStatus(package, interface, rules))
            sendPackage(package, package.destignation);
        else sendPackage(package, "rejected");
    }

private:
    router() {}
    router( const router&);
    router& operator=( router& );

    int packageStatus(package package, string interface, vector<rule> rules)
    {
        /**
         * @brief Получить статус пакета
         * @param package Входной пакет
         * @param interface Название входного интерфейса
         * @param rules Вектор правил
         * @return allowed если пакет принят, denied если пакет отклонён
         */
        for(int i = 0; i < rules.size(); i++){
            if(isAccording(package, interface, rules[i]))
                if(rules[i].status == "allow")
                    return allowed;
                else return denied;
        }
        int status=isAccording(package, interface, rules[rules.size()-1]);
        if(rules[rules.size()-1].status == "deny")
            status=!status;
        return status;
    }

    bool isAccording(package package, string interface, rule rule)
    {
        /**
         * @brief Проверить, попадает ли пакет под правило
         * @param package Входной пакет
         * @param interface Название входного интерфейса
         * @param rule Входное правило
         */
        if((interface != rule.source && rule.source != "*")||(package.sender != rule.sender && rule.sender != 0)||(package.receiver != rule.receiver && rule.receiver != 0)||(package.destignation != rule.destignation && rule.destignation != "*")||(package.protocol != rule.protocol && rule.protocol != "*"))
            return false;
        return true;
    }

    void sendPackage(package package, string destignation)
    {
        /**
         * @brief Записать пакет в интерфейс
         * @param package Входной пакет
         * @param interface Название интерфейса
         */
        ofstream outFile;
        outFile.open(destignation + ".txt", std::ios::app);
        outFile << package.sender << " " << package.receiver << " " << package.destignation << " " << package.protocol << endl;
        outFile.close();
    }
};
