#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include <QPixmap>

#include "clientinfo.h"

class ClientSocket : public QThread
{
    Q_OBJECT
public:
    ClientSocket(int descriptor, QObject *parent = 0);
    ~ClientSocket();

    void run();

public slots:
    void connected();
    void disconnected();
    void readyRead();
private:
    int handle;
    QTcpSocket *socket;
};


#endif // CLIENTSOCKET_H
