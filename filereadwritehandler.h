#ifndef FILEREADWRITEHANDLER_H
#define FILEREADWRITEHANDLER_H

#include <QObject>
#include <QThread>
#include <QtCore>
#include <QTcpSocket>

#define MAXBTYESIZE 256 //copy 256byte for each

class FileReadWriteHandler : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit FileReadWriteHandler();
    ~FileReadWriteHandler(){}

    void run();
    void setTask(QString path);
signals:
    void bytesRead(qint64 bytes);
    void updateProgress(double bytesSent, qint64 totalByte);
    void sendByte(QByteArray bytes, double size);
    void processSuccess();
private:
    QString filesPath;
    int len;
    int sz;
};

#endif // FILEREADWRITEHANDLER_H
