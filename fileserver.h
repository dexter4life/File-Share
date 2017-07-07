#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>
#include <QList>
#include <QTcpSocket>
#include <QList>
#include <QPixmap>
#include <QObject>



enum DATA_SISE{
   BUFFER_SIZE = 100
};

enum DEFAULT_PORT
{
    D_PORT = 8184
};

class Client;
class QSignalMapper;

typedef QList< Client * > ClientList;

class fileServer : public QTcpServer
{
    Q_OBJECT

public:
    fileServer(QObject *parent = 0);
    ~fileServer()
    {
    }

    void startServer();
    virtual void incomingConnection(qintptr handle);
    ClientList &clients(){ return existingClient; }
signals:
    void error(QString error);
    void networkSessionStatus(QString status);
    void readyRead(QObject *socket);
    void sendAvatarInfo(QString name, QPixmap image, QString address);
public slots:
    void disconnected();
private:
    ClientList existingClient;
};

#endif // FILESERVER_H
