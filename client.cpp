/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/
#include "fileshare.h"
#include "client.h"
#include "filewidget.h"
#include <QtXml>
#include <QRegularExpressionMatch>
#include <QRegExp>
#include <QRegularExpression>
#include "platform.h"
#include "writeprocess.h"
#include <iphlpapi.h>
#include "miniserver.h"
#include "historyview.h"

#pragma comment(lib, "IPHLPAPI.lib")

Client::Client(qintptr number, QString destination, QObject *parent) : QThread(parent)
  , dest(destination), id(number), nextBlockSize(0), tmpSize(0), socketState(QAbstractSocket::UnconnectedState)
{
    file = new QFile(this);
    writeToSocket = new WriteProcess(this);
    connect(this, SIGNAL(clientDisconnected()), writeToSocket, SLOT(abort()));
    connect(writeToSocket, SIGNAL(resultReady(QByteArray)), this, SLOT(write(QByteArray)), Qt::QueuedConnection);
}

Client::Client(QString destination, QObject *parent)
    : dest(destination), QThread(parent), id(0), nextBlockSize(0), tmpSize(0), socketState(QAbstractSocket::UnconnectedState)
{
    clientSocket = new QTcpSocket();
    if( !clientSocket->open(QIODevice::ReadWrite))
        emit error(clientSocket->error());

//    id = clientSocket->socketDescriptor();

    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(clientSocket, SIGNAL(connected()), this, SLOT(sendAvatar()));
    connect(clientSocket, SIGNAL(disconnected()),SIGNAL(clientDisconnected()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this, SIGNAL(destroyed()), clientSocket, SLOT(deleteLater()));

    file = new QFile(this);
    writeToSocket = new WriteProcess(this);
    connect(writeToSocket, SIGNAL(resultReady(QByteArray)), this, SLOT(write(QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(clientDisconnected()), writeToSocket, SLOT(abort()));
}
QByteArray Client::getByte(QByteArray &rawData, QString startTag, QString endTag)
{
    int iStartPos = rawData.indexOf(startTag);
    int iEndPos = rawData.indexOf(endTag);
    return rawData.mid(iStartPos + startTag.length(), (iEndPos - iStartPos) - startTag.length());
}

void Client::disconnectFromHost()
{
    if( clientSocket->state() == QTcpSocket::ConnectedState)
        clientSocket->disconnectFromHost();
}
bool Client::findHost() const
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        return false;
    }

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            QMessageBox::critical(NULL, tr("Error"), tr("Error allocating memory needed to call GetAdaptersinfo\n"));
            return false;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {

            clientSocket->connectToHost(QHostAddress(QString(pAdapter->GatewayList.IpAddress.String)), D_PORT);
            if( clientSocket->waitForConnected(100) )
                return true;
            pAdapter = pAdapter->Next;

        }
        clientSocket->connectToHost(QHostAddress::LocalHost, D_PORT);
        if( clientSocket->waitForConnected(50))
            return true;
    }
    else {
        QMessageBox::critical(NULL, tr("Error"), tr("GetAdaptersInfo failed with error: %1\n").arg(dwRetVal));
        return false;
    }

    clientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);


    if (pAdapterInfo)
        free(pAdapterInfo);

    return false;
}

bool Client::connectToSpecificHost(QString name)
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        return false;
    }

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            QMessageBox::critical(NULL, tr("Error"), tr("Error allocating memory needed to call GetAdaptersinfo\n"));
            return false;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            if( name.isEmpty() && name == QLatin1String(pAdapter->AdapterName))
                clientSocket->connectToHost(QHostAddress(QString(pAdapter->GatewayList.IpAddress.String)), D_PORT);
            if( clientSocket->waitForConnected(50))
                return true;
            pAdapter = pAdapter->Next;
        }
    }
    else {
        QMessageBox::critical(NULL, tr("Error"), tr("GetAdaptersInfo failed with error: %1\n").arg(dwRetVal));
        return false;
    }

    if (pAdapterInfo)
        free(pAdapterInfo);

    return false;
}

void Client::connectToHost(QHostAddress address, qint64 port)
{
    clientSocket->connectToHost(address, port);

    if( !clientSocket->waitForConnected(50)){
        QMessageBox::critical(NULL, tr("Connection failed"), tr("can't connect to %1 at port %2").arg(address.toString())
                              .arg(QString::number(port)));
        return;
    }
}

Client::Client(QTcpSocket *socket, QString destination, QObject *parent)
{
    clientSocket = socket;
}
bool Client::removeFile(QString name, bool newFile)
{
    QFileInfo fileInfo(name);
    if( fileInfo.exists() && newFile )
    {
        QFile file(name);
        return file.remove();
    }
    return false;
}

void Client::writeFile(QByteArray byte, QString name)
{

}

void Client::readyRead()
{

    quint16 type = 0;

    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_4_3);

    if (nextBlockSize == 0)
    {
        if (clientSocket->bytesAvailable() < sizeof(quint64))
            return;
        in >> nextBlockSize;
    }

    if( clientSocket->bytesAvailable() < nextBlockSize)
        return;

    quint16 nameSize = 0; //type(AVATAR, FILETYPE, etc)
    quint64 size;
    QString name(nameSize, '\0'); //name of data
    QByteArray byte(nextBlockSize, '\0'); //data
    bool endOfByte; //end flag
    double len;

    in  >> type >> nameSize >> name >> byte >> size >> endOfByte;

    QMetaEnum qMetaEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("FileType"));

    switch(qMetaEnum.keysToValue(qMetaEnum.valueToKey(type)))
    {
    case FileType::FILETYPE:
    {

        if(tmpSize == 0 ){
            QString tmp = QString("%1\\%2").arg(dest).arg(name);
            file->setFileName(tmp);
            if( file->size() == size)
                file->remove();

            if( !file->open(QIODevice::Append | QIODevice::WriteOnly))
                QMessageBox::critical(NULL, tr("File Error"), tr("Can't write to disk"));
            emit setWidget(this, tmp, size);
         }

        tmpSize += byte.size();
        len = file->write(byte);

        double fract = (double)tmpSize / size;
        double value = size < quint64(MAXBUFFERLENGTH)  ? double(100) : (fract * 100);

        emit progressChanged(value);

        if(quint64(tmpSize) == size){
            file->close();
            tmpSize = 0;
        }

    }break;
    case FileType::AVATAR:
    {
        QImage &avatar = QImage::fromData(byte, "JPG");
        emit sendAvatarInfo((!name.isEmpty() ? name : "No Name"), avatar, QString::number(this->socket()->socketDescriptor()));
    }break;
    case FileType::FOLDERTYPE:
    {

    }break;
    case FileType::UNKNOWN: //from an unknown source
        qDebug() << "Unknown";
    }

    clientSocket->readAll();
    nextBlockSize = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client::WriteToClientSocket( QString source, FileType type )
{
    this->writeToSocket->setTask(source, type);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Client::run()
{
    if( id != 0 )
    {
        clientSocket = new QTcpSocket();

        if( !clientSocket->setSocketDescriptor(id) )
            emit error(clientSocket->error());

        clientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(clientSocket, SIGNAL(connected()), this, SLOT(sendAvatar()));
        connect(this, SIGNAL(destroyed()), clientSocket, SLOT(deleteLater()));
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
        connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

    }

    exec();
}
void Client::sendAvatar()
{

    userName = QString::fromStdString(Platform::getRegValue(L"FirstName") + " " +
                                      Platform::getRegValue(L"LastName"));
    userAvatar = Platform::readAccountPix(Platform::getAvatarPath());

    if( clientSocket->isOpen())
    {
        QByteArray image;
        QByteArray block;
        QBuffer tmpBuffer(&image);
        tmpBuffer.open(QIODevice::ReadWrite);
        tmpBuffer.seek(0);
        userAvatar.save(&tmpBuffer, "JPG");

        QDataStream os(&block, QIODevice::WriteOnly);
        os.setVersion(QDataStream::Qt_4_3);

        os << quint64(0) << quint16(AVATAR)
           << quint16(userName.size())
           << userName
           << image
           << bool(true);

        os.device()->seek(0);
        os << quint64(image.size());

        clientSocket->write(block);
        clientSocket->flush();
    }
}

void Client::write(QByteArray byte)
{
    clientSocket->write(byte);
    clientSocket->flush();

}

void Client::stateChanged(QAbstractSocket::SocketState state)
{
    if( state == QAbstractSocket::UnconnectedState && this->writeToSocket->isRunning()){
        writeToSocket->abort();
    }
}

void Client::stop()
{
    clientSocket->deleteLater();
    exit(0);
}

void Client::disconnected()
{
    emit clientDisconnected(id);
}
int Client::number() const
{ return id; } //return the id for the client

void Client::setSocket(QTcpSocket *socket)
{  clientSocket = socket; }


