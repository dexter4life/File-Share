#include "filereadwritehandler.h"
#include "client.h"

FileReadWriteHandler::FileReadWriteHandler()
{
}



void FileReadWriteHandler::run()
{

    QFile *file = new QFile(filesPath);
    sz = file->size();

    //char buffer[MAXBTYESIZE] = {0};

    qDebug() << "data size: " << sz << "File path: " << filesPath;

    if( file->open(QIODevice::ReadOnly) )
    {
        while( !file->atEnd() )
        {
             QByteArray tmp;

             tmp.append("data$");
            tmp.append(filesPath);
            tmp.append( "$");
            tmp.append(file->read(MAXBTYESIZE));

            emit sendByte( tmp, sz);
            //emit updateProgress(len, sz);
        }
        emit processSuccess();
    }
}

void FileReadWriteHandler::setTask(QString path)
{
    filesPath = path;
}
