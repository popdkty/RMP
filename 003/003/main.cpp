#endif // VKAPI_H
#include "vkapi.h"
#include <QtNetwork/QtNetwork>
#include<QNetworkAccessManager>
#include <QDebug>
VkApi::VkApi(QObject *parent) : QObject(parent)
{

}

void VkApi::getId(QString id){
    this->id=id;
}

void VkApi::setId(){
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();
    connect(networkManager, &QNetworkAccessManager::finished, this, &VkApi::info);
    networkManager->get(QNetworkRequest(QUrl("https://api.vk.com/method/users.get?user_ids="+id+"&fields=bdate,nickname,photo_400_orig&access_token=6c38b39f6c38b39f6c38b39f0e6c57510566c386c38b39f326a2cdea89151ca34047b2c&v=5.103")));
}

void VkApi::setIdFriend(){
    QNetworkAccessManager *networkManager2 = new QNetworkAccessManager();
    connect(networkManager2, &QNetworkAccessManager::finished, this, &VkApi::info2);
    networkManager2->get(QNetworkRequest(QUrl("https://api.vk.com/method/friends.get?user_id="+id+"&access_token=6c38b39f6c38b39f6c38b39f0e6c57510566c386c38b39f326a2cdea89151ca34047b2c&v=5.103")));
}

void VkApi::info(QNetworkReply *reply){
    if(!reply->error()){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();
        QJsonValue jv = root.value("response");
        response(jv);
    }
    reply->deleteLater();
}

void VkApi::response(QJsonValue jv){
    if(jv.isArray()){
        QJsonObject subtree = jv.toArray().at(0).toObject();
            getName(subtree);
            getLastName(subtree);
            getBirth(subtree);
            getNick(subtree);
            getPicture(subtree);
            checkPrivate(subtree);
    }
}

void VkApi::getName(QJsonObject subtree){
    QJsonValue val=subtree.value("first_name");
    emit setName(val.toString());
}
void VkApi::getLastName(QJsonObject subtree){
    QJsonValue val=subtree.value("last_name");
    emit setLastName(val.toString());
}
void VkApi::getBirth(QJsonObject subtree){
    QJsonValue val=subtree.value("bdate");
    emit setBirth(val.toString());
}
void VkApi::getNick(QJsonObject subtree){
    QJsonValue val=subtree.value("nickname");
    emit setNick(val.toString());
}
void VkApi::getPicture(QJsonObject subtree){
   QJsonValue val=subtree.value("photo_400_orig");
    load(val.toString());
}

void VkApi::load(QString imageUrl) {
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(imageUrl)));
    QEventLoop event;
    connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    disconnect(response,SIGNAL(finished()),&event,SLOT(quit()));
    on_load(response);
}
void VkApi::on_load(QNetworkReply *reply) {
    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    if(pixmap.isNull())
        emit noPhoto("Нет фото");
    else emit loaded(pixmap);
    reply->deleteLater();
}
void VkApi::info2(QNetworkReply *reply){
        if(!reply->error()){
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject root = document.object();
            QJsonValue jv = root.value("response");
            response2(jv);
        }
        reply->deleteLater();
}
void VkApi::response2(QJsonValue jv){
       QJsonValue val=jv.toObject().value("items");
       QJsonArray ja = val.toArray();
       checkFriends(ja);
       QString str;
       for(int i=0;i<ja.count();i++){
           str+="id: "+QString::number(ja.at(i).toInt())+"\n";
           emit setFriends(str);
       }
}
void VkApi::checkFriends(QJsonArray ja){
    if(ja.at(0).toInt()==0)
        emit setFriends("");
}

void VkApi::checkPrivate(QJsonObject subtree){
    QJsonValue val=subtree.value("is_closed");
    if(val.toBool()==true)
        emit setFriends("Аккаунт закрыт");
}
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QNetworkAccessManager>
#include"vkapi.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VkApi * Vk;
};

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
