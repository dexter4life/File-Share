/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "writeprocess.h"

WriteProcess::WriteProcess(QObject *parent) : QThread(parent)
{
    end = false;
}

WriteProcess::~WriteProcess()
{

}

void WriteProcess::run()
{
    typedef QPair< QString, Client::FileType> Item;

    forever
    {
        if(end) return; //end forever

        Item item = task.dequeue();
        done = false;

        QFile file(item.first);
        QString fileName = QFileInfo(file).fileName();

        if(!file.open(QIODevice::ReadOnly))
        {
            int result = QMessageBox::question(NULL, tr("Write Error"), tr("Unable to write to socket. Continue?"),
                                               QMessageBox::Yes | QMessageBox::No);
            if( result == QMessageBox::No)
                return;
        }

        while(!done)
        {
            sendNextBlock(file, MAXBUFFERLENGTH, item.second, fileName);
            thread()->msleep(500); //wait for readyRead to finish
        }
        if( task.isEmpty() )
            break;
    }
}

void WriteProcess::setTask(QString path, Client::FileType type)
{
    QPair< QString, Client::FileType > tmp;
    tmp.first = path;
    tmp.second = type;

    task.enqueue(tmp);

    if( !this->isRunning() ){
        end = false;
        start();
    }
}

void WriteProcess::abort()
{
    end = true;
    if(isRunning())
      quit();
}

void WriteProcess::sendNextBlock(QFile &file, qint64 size, Client::FileType type, QString fileName)
{
    QByteArray &byte = file.read(size);
    QByteArray block;

    QDataStream os(&block, QIODevice::WriteOnly);
    os.setVersion(QDataStream::Qt_4_3);

    os << quint64(0) << quint16(type)
                     << quint16(fileName.size())
                     << fileName
                     << byte
                     << quint64(file.size());

    if( file.atEnd() && file.size() < size ){
        os << bool(true);
        done = true;
    }
    else if( file.atEnd() ){
         os << bool(true);
         done = true;
    }
    else
         os << bool(false);

    os.device()->seek(0);
    os << quint64(byte.size());

    emit resultReady(block);
}
