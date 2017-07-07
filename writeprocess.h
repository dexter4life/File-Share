#ifndef WRITEPROCESS_H
#define WRITEPROCESS_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "client.h"

#include <QQueue>
#include <QPair>

class WriteProcess : public QThread
{
    Q_OBJECT
public:
    explicit WriteProcess(QObject *parent = 0);
    ~WriteProcess();
    void run();

    void setTask(QString path, Client::FileType type);
signals:

    void resultReady(QByteArray block);
public slots:
    void abort();
    void sendNextBlock(QFile &file, qint64 size, Client::FileType type, QString fileName);
private:
    bool wait;
    bool done;
    bool end;
    QQueue<QPair<QString, Client::FileType> > task;
};

#endif // WRITEPROCESS_H
