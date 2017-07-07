#ifndef FILESHARE_H
#define FILESHARE_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QListView>
#include <QHostAddress>
#include "WlanInterface.h"
#include <QThread>
#include <QTcpServer>
#include "global.h"
#include <QStringListIterator>
#include "client.h"


QT_BEGIN_NAMESPACE
class QGridLayout;
class QNetworkSession;
class TitleBarWidget;
class miniserver;
class Home;
class Settings;
class HotspotButton;
class QMenu;
class HistoryModel;
class historyView;
class fileServer;
class QTcpSocket;
class FileWidget;
class Client;
QT_END_NAMESPACE

class FileShare : public QWidget
{
    Q_OBJECT
public:
    Settings *settings;
    Home *home;

    explicit FileShare(QWidget *parent = 0);
    ~FileShare();

    virtual void paintEvent(QPaintEvent *e);
    void setTrayMenu();
    void setConnections();
    void initializeHistory();
    void initializeWidgets();
    // open socket for connecting to server
    void setHistoryItem(QString path);
    void openSocket();

    QTcpSocket *socket;
    void setFirewallException();
public slots:
    void join(bool state);
    void moveWindow(QPoint pos);
    void displayHome();
    void displaySettings();
    void startHotspot(bool state);
    void startHosting(bool state);
    void minimizeWindow();
    void showWindow(QSystemTrayIcon::ActivationReason reason);
    void displayRecent();
    void setHistoryItem(FileWidget *widget);
    void deleteSocket();
    void showJoinMessage(bool value);
    void sendFolder();
    void sendFileOnly();
    void setAvatarInfo(QString name, QImage image, QString address);
    void sendNextFile(QStringList &list, Client::FileType type);
private:
     QSystemTrayIcon *sysTray;
     TitleBarWidget *titleBar;
     HotspotButton *hotSpot;
     QMenu *menu;
     QAction *exit, *sendFile;
     HistoryModel *model;
     historyView *recent;
     QString sizePrefix(qint64 size);
     miniserver *server; //remove
     QStringList data;
     QPixmap userAvatar;
     QString userName;
     QList < QTcpSocket * > clients;
     HANDLE hClientHandle;
     QStringList filesList;
     QStringListIterator *fileListIterator;
     int filesCount;
     Client *client;
     int clientCount;

     QByteArray block;
     quint64 nextBlockSize;
     QString dest;
signals:
     void sendFilePath(QString path);
     void setJoinButtonChecked(bool state);
};

#endif // FILESHARE_H
