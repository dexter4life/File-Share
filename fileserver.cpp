#include "fileserver.h"
#include "clientsocket.h"
#include "platform.h"
#include "client.h"

#include <QSignalMapper>

fileServer::fileServer(QObject *parent)
    : QTcpServer(parent){ }

void fileServer::startServer()
{
      if(!listen(QHostAddress::Any, D_PORT))
        emit error(errorString());
}

void fileServer::incomingConnection(qintptr handle)
{

    Client *client = new Client(handle);
    connect(client, SIGNAL(clientDisconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(sendAvatarInfo(QString,QPixmap,QString)),
            this, SIGNAL(sendAvatarInfo(QString,QPixmap,QString)));

    existingClient.push_front(client);

    emit networkSessionStatus("new client");
}

void fileServer::disconnected()
{
    qDebug() << "Disconnected";
}

