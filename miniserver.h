#ifndef MINISERVER_H
#define MINISERVER_H

#include <QObject>
#include <QTcpServer>
#include <QImage>
#include "client.h"
#include "historyview.h"

class FileWidget;

class miniserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit miniserver(QString filePath, QObject *parent = 0);
    ~miniserver();

    void incomingConnection(qintptr handle);

    void writeToClient(QStringList pathList, int row, qintptr qiClientHandle);
    void resetConnections();
    historyView *_recent;
signals:
    void removeRow(int);
    void setAvatarInfo(QString,QImage,QString);
    void sendFilePath( QString, Client::FileType);
    void stopClient();
    void sendHistoryWidget(FileWidget* widget);
public slots:
    void removeClient(quint64 handle);
    void startServer();
    void stopServer();
    void setPath(QString path);
    //    void displayProgress(double progress, qint64 size, QString path);
    void setWidget(Client *client, QString path, qint64 size);
private:
    static int clientCount;
    QList< Client *> clients;
    QString dest;
};
#endif // MINISERVER_H
