#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtWidgets>
#include <QtNetwork>
#include <QtXml>
#include <QQueue>
#include <QPair>
#include "global.h"

class historyView;
class FileWidget;
class WriteProcess;

enum __FileType
{
   UNKNOWN = 0, AVATAR, FOLDERTYPE, FILETYPE
};

class Client : public QThread
{
    Q_OBJECT
    Q_ENUMS(FileType)

public:
    enum FileType
    {
       UNKNOWN = 0, AVATAR, FOLDERTYPE, FILETYPE
    };

    historyView *recent;
    explicit Client(qintptr number = 0, QString destination = QString(), QObject *parent = 0);
    explicit Client(QString destination, QObject *parent = 0);
     explicit Client(QTcpSocket *socket, QString destination, QObject *parent = NULL);
    virtual ~Client() {}
    void run();
    int number() const;
    void setSocket(QTcpSocket *socket);
    void setHistoryWidget(QString source);
    QByteArray getByte(QByteArray &rawData, QString startTag, QString endTag);

    QTcpSocket *socket() { return clientSocket;}
    void disconnectFromHost();
    void connectToHost(QHostAddress address, qint64 port);

    qintptr id;
    void writeFile(QByteArray byte, QString name);
    bool removeFile(QString name, bool newFile);
    bool findHost() const;
    bool connectToSpecificHost(QString name);

    quint64 getHandle() const {return handle;}
    void setHandle(quint64 h){ handle = h; }
    void setRow(int row){ r = row;}
    int row(){ return r;}
public slots:
    void readyRead();
    void WriteToClientSocket(QString source, FileType type);
    void stop();
    void disconnected();
    void sendAvatar();
    void write(QByteArray byte);
    void stateChanged(QAbstractSocket::SocketState state);
private:
    QTcpSocket *clientSocket;
    QString dest;
    QPixmap userAvatar;
    QString userName;
    QByteArray block;
    quint64 nextBlockSize;
    unsigned int tmpSize;
    QFile *file;
    WriteProcess *writeToSocket;
    QQueue<QPair<QString, FileType> > queue;
    bool startWrite;
    QAbstractSocket::SocketState socketState;
    quint64 handle;
    int r;
signals:
    void setWidget(Client *client, QString path, qint64 size);
    ///emit this signal to main thread
    void sendAvatarInfo(QString name,QImage image,QString address);
    void clientDisconnected();
    void clientDisconnected(quint64);
   // void updateProgressBar(double, qint64 size, QString name);
    void progressChanged(double);
    void sendHistoryWidget(FileWidget *widget);
    void requestNextFile();
    void error(QTcpSocket::SocketError errorcode);
    void connected();
};

#endif // CLIENT_H
