#ifndef VKAPI_H
#define VKAPI_H
#include <QtNetwork/QtNetwork>
#include<QNetworkAccessManager>
#include <QObject>
#include <QPixmap>
class VkApi : public QObject
{
    Q_OBJECT
public:
    explicit VkApi(QObject *parent = nullptr);
public slots:
    /**
     * @brief getId Получение айди
     * @param id айди
     */
    void getId(QString id);
    /**
     * @brief setId запрос к методу users API
     */
    void setId();
    /**
     * @brief setIdFriend запрос к методу friends API
     */
    void setIdFriend();
private:
    /**
     * @brief info получение значения ключа response
     * @param reply ответ на запрос к API
     */
    void info(QNetworkReply *reply);
    /**
     * @brief info2 получение значения ключа response
     * @param reply ответ на запрос к API
     */
    void info2(QNetworkReply *reply);
    /**
     * @brief response вычленение необходимых полей
     * @param jv массив-значение ключа response
     */
    void response(QJsonValue jv);
    /**
     * @brief getPicture получение ссылки на фото
     * @param subtree нулевой эл-т массива-значение ключа response
     */
    void getPicture(QJsonObject subtree);
    QString id;
    QNetworkAccessManager manager;
private slots:
    /**
     * @brief getName получение имени
     * @param subtree нулевой эл-т массива-значение ключа response
     */
    void getName(QJsonObject subtree);
    /**
     * @brief getLastName получение фамилии
     * @param subtree нулевой эл-т массива-значение ключа response
     */
    void getLastName(QJsonObject subtree);
    /**
     * @brief getBirth получение даты рождения
     * @param subtree нулевой эл-т массива-значение ключа response
     */
    void getBirth(QJsonObject subtree);
    /**
     * @brief getNick получение ника
     * @param subtree нулевой эл-т массива-значение ключа response
     */
    void getNick(QJsonObject subtree);
    /**
     * @brief load получение данных фото
     * @param imageUrl ссылка на фото
     */
    void load(QString imageUrl);
    /**
     * @brief on_load отображение фото
     * @param reply  данные фото
     */
    void on_load(QNetworkReply* reply);
    /**
     * @brief response2 получение списка id друзей
     * @param jv значение ключа response
     */
    void response2(QJsonValue jv);
    /**
     * @brief checkFriends проверка на наличие поля items
     * @param ja массив с id друзей
     */
    void checkFriends(QJsonArray ja);
    /**
     * @brief checkPrivate проверка на приватность профиля
     * @param subtree нулевой эл-т массива-значение ключа response
     */
    void checkPrivate(QJsonObject subtree);
signals:
    /**
     * @brief setName имя
     */
    void setName(QString);
    /**
     * @brief setLastName фамилия
     */
    void setLastName(QString);
    /**
     * @brief setBirth дата рождения
     */
    void setBirth(QString);
    /**
     * @brief setNick ник
     */
    void setNick(QString);
    /**
     * @brief loaded фото
     */
    void loaded(QPixmap);
    /**
     * @brief noPhoto надпись - Нет фото
     */
    void noPhoto(QString);
    /**
     * @brief setFriends id друзей
     */
    void setFriends(QString);
};
