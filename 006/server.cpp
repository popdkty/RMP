/**
 * @file server.cpp
 * @brief server
 * @author Fomichev
 */
#include "server.h"
#include <QtWebSockets>
#include <QDebug>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QFile>

Server::Server(QObject *parent) : QObject(parent),
    webServer (new QWebSocketServer(QStringLiteral("Chat Server"),
                                    QWebSocketServ-er::NonSecureMode,
                                    this))
{
  if (webServer->listen(QHostAddress::Any, readSet-tings())){
    qDebug() << webServer->serverUrl();
    QMessage-Box::information(NULL,QObject::tr("Информация"), tr("Server started"));
    connect (webServer, &QWebSocketServer::newConnection,
             this, &Server::connectUser);
  } else {
    QMessage-Box::information(NULL,QObject::tr("Информация"),
                             tr("Unable to start serv-er"));
  }
  clients.clear();
  db = QSqlDatabase::addDatabase("QSQLITE");
  connectDatabase();
}

Server::~Server()
{
  clients.clear();
  db.close();
  emit info("Server close");
  webServer->close();
}

void Server::sender(QString message){
  foreach (client, clients) {
    QWebSocket* clientSock = (QWebSocket*) client;
    clientSock->sendTextMessage(message);
  }
}

void Server::connectDatabase()
{
  db.setDatabaseName("db.sqlite");
  if (db.open()){
      qInfo() << "Database open";
      QFile f;
      f.setFileName(":/dbStructure.txt");
      f.open(QIODevice::ReadOnly);
      QSqlQuery query;
      if(query.exec(f.readLine()))
        qInfo() << "Таблица создана";
      f.close();
  }
  else
    qInfo() << "Невозможно открыть БД";
}

void Server::recordMessage(QString nick, QString mes-sage)
{
  QSqlQuery query;
  query.prepare("INSERT INTO messages (id, nickname, message) VALUES (NULL, :nick, :mes)");
  query.bindValue(":nick", nick);
  query.bindValue(":mes", message);
  if(query.exec())
    emit info("     Message recorded");
}

void Server::setSettings(QString host, quint16 port)
{
  settings = new QSettings(":/setting.ini", QSet-tings::IniFormat);
  settings->beginGroup("Settings");
  settings->setValue( "host", host);
  settings->setValue("port", port);
  settings->endGroup();
  QMessage-Box::information(NULL,QObject::tr("Информация"),tr("Настройки сервера изменены. Перезапустите сервер"));
}

quint16 Server::readSettings()
{
  settings = new QSettings(":/setting.ini", QSet-tings::IniFormat);
  settings->beginGroup("Settings");
  QString host = settings->value("host").toString();
  quint16 port = settings->value("port").toUInt();
  settings->endGroup();
  return port;
}

void Server::connectUser()
{
  emit info("We have new user!");
  client = webServer->nextPendingConnection();
  clients.push_back(client);
  sender("1 new user");
  send10Message();

  connect(client, &QWebSocket::textMessageReceived,
          this, &Server::runClientAction);
  connect(client, &QWebSocket::disconnected,
          this, &Server::disconnectUser);
}

void Server::runClientAction(QString message)
{
  QString nick, mes;
  QStringList mesList;
  mesList = message.split(" ");
  nick = mesList[0];
  mesList.removeFirst();
  mes = mesList.join(" ");
  emit info("Get message from " + nick + ": " + mes);
  sender(nick + ": " + mes);
  recordMessage(nick, mes);
}

void Server::disconnectUser()
{
  emit info("We lost user!");
  sender("1 user left");
  clients.removeOne(client);
  client->disconnect();
}

void Server::send10Message()
{
  QSqlQuery query;
  int k, i=0;
  query.exec("SELECT COUNT(*) FROM messages");
  query.next();
  k = query.value(0).toInt();
  query.exec("SELECT * FROM messages");
  query.next();
  if(k > 10){
    while (i < (k-10)){
      query.next();
      i++;
    }
  }
  while(i<k){
    client->sendTextMessage(query.value(1).toString() + ": " + query.value(2).toString());
    query.next();
    i++;
  }
}
Листинг 2.6 – Файл res.qrc
<RCC>
    <qresource prefix="/images">
        <file>icon.png</file>
    </qresource>
    <qresource prefix="/">
        <file>setting.ini</file>
        <file>dbStructure.txt</file>
    </qresource>
</RCC>
