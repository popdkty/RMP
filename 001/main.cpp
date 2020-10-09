/**
 * @file main.cpp
 * @author Fomichev Alexander
 * @brief Лабораторная работа 1
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "package.hpp"
#include "rule.hpp"
#include "router.hpp"

using namespace std;

int main()
{
    router& router = router::getInstance();
    packageTCPCreater tcpCreater;
    packageUDPCreater udpCreater;
    packageRDPCreater rdpCreater;
    try{
        vector<package> inInt1 = router.input_parser("InInt1.txt", tcpCreater, udpCreater, rdpCreater), inInt2 = router.input_parser("InInt2.txt", tcpCreater, udpCreater, rdpCreater);
        vector<rule> rules = router.rule_parser("rules.txt");
        router.routePackageVector(inInt1, "int1", rules);
        router.routePackageVector(inInt2, "int2", rules);
    }
    catch(runtime_error e){
        cout << e.what();
    }
    return 0;
}
