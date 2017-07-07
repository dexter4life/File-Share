#include "clientsocket.h"

#include <QDebug>

ClientSocket::ClientSocket(int descriptor, QObject *parent)
    : QThread(parent)
{
    handle = descriptor;
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::run()
{
    socket = new QTcpSocket();

    connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ClientSocket::readyRead, Qt::DirectConnection);

    socket->setSocketDescriptor(handle);
     socket->open(QIODevice::ReadWrite);

    exec();
}

void ClientSocket::connected()
{
    qDebug() << "connected";
}

void ClientSocket::disconnected()
{
    socket->deleteLater();
    exit(0);
}

void ClientSocket::readyRead()
{
    QByteArray dataSent;
    QDataStream ds(socket);
    ds.setVersion(QDataStream::Qt_5_0);

    //QString dataSent;
    ds.readRawData( dataSent.data(), socket->bytesAvailable());

    qDebug() << dataSent;
}
