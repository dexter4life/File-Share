/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "fileshare.h"
#include "miniserver.h"
#include "client.h"
#include "global.h"
#include <QtCore>
#include "filewidget.h"

miniserver::miniserver(QString filePath, QObject *parent) : QTcpServer(parent)
{ setPath(filePath); }

int miniserver::clientCount = 0;

miniserver::~miniserver(){}

void miniserver::setWidget(Client *client, QString path, qint64 size)
{
    FileWidget *widget = new FileWidget(path, qobject_cast< QWidget * > (_recent));
    widget->setFileSize(size);
    connect(client, SIGNAL(progressChanged(double)), widget, SLOT(updateProgressBar(double)));
    static_cast< FileShare * >(this->parent())->setHistoryItem(widget);
}

void miniserver::incomingConnection(qintptr handle)
{
    Client *client = new Client(handle, dest, this->parent());

    connect(client, SIGNAL(sendAvatarInfo(QString,QImage,QString)),SIGNAL(setAvatarInfo(QString,QImage,QString)));
    connect(this, SIGNAL(stopClient()), client, SLOT(stop()), Qt::QueuedConnection);
    connect(client, SIGNAL(setWidget(Client*,QString, qint64)), this, SLOT(setWidget(Client*,QString, qint64)));
    connect(client, SIGNAL(clientDisconnected(quint64)),SLOT(removeClient(quint64)));
    client->setRow( clientCount );
    clientCount++;
    clients.push_back(client); //list client connected

    client->start();

}

void miniserver::startServer()
{
    //start server
    if( !listen(QHostAddress::Any, D_PORT))
        QMessageBox::critical(qobject_cast< QWidget * >(this),
                              tr("Server Error"), tr("").arg(this->errorString()));

}

void miniserver::stopServer()
{
    emit stopClient();
    close();
    deleteLater();
}
void miniserver::writeToClient(QStringList pathList, int row, qintptr qiClientHandle)
{
    foreach(QString path, pathList){
        emit sendFilePath(path, Client::FILETYPE);//changes
    }
}

void miniserver::resetConnections()
{
    foreach(Client *client, clients){
        disconnect(this, SIGNAL(sendFilePath(QString)), client, SLOT(WriteToClientSocket(QString)));
    }
}

void miniserver::removeClient(quint64 handle)
{
    foreach(Client *client, clients){
        if( client->getHandle() == handle){
            emit removeRow(client->row());
        }
    }
}

void miniserver::setPath(QString path)
{ dest = path;}

