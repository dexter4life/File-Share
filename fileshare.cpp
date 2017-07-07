/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#include "windowsfirewall.h"
#include "filewidget.h"
#include "fileshare.h"
#include "titlebarwidget.h"
#include "home.h"
#include "platform.h"
#include "settings.h"
#include "hotspotbutton.h"
#include "historymodel.h"
#include "historyview.h"
#include "clientinfo.h"
#include "miniserver.h"
#include <QtNetwork>

#pragma comment(lib, "IPHLPAPI.lib")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pNotifyFunc(PWLAN_NOTIFICATION_DATA pData, PVOID pContext)
{
    reinterpret_cast<FileShare*>(pContext)->home->setWlanStatus(
                QString::number(pData->NotificationCode));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::setFirewallException()
{
    HRESULT hr = S_OK;
    HRESULT comInit = E_FAIL;
    INetFwProfile* fwProfile = NULL;

    // Initialize COM.
    comInit = CoInitializeEx(
                0,
                COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
                );

    // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
    // initialized with a different mode. Since we don't care what the mode is,
    // we'll just use the existing mode.
    if (comInit != RPC_E_CHANGED_MODE)
    {
        hr = comInit;
        if (FAILED(hr))
        {
            QMessageBox::critical(this, tr("Filewall exception"), tr("CoInitializeEx failed: %1 ").arg(hr));
        }
    }
    // Retrieve the firewall profile currently in effect.
    hr = WindowsFirewall::WindowsFirewallInitialize(&fwProfile);
    if (FAILED(hr))
    {
        QMessageBox::critical(this, tr("Filewall exception"), tr("WindowsFirewallInitialize failed: %1").arg(hr));
    }

    BOOL fwOn = 0;
    WindowsFirewall::WindowsFirewallIsOn(fwProfile, &fwOn);

    if( fwOn == FALSE ){
        int reply = QMessageBox::information(this, QApplication::applicationName(), tr("It Seems that firewall is disabled, do you want it turn on? "),
                                 QMessageBox::Yes | QMessageBox::No);
       if( reply == QMessageBox::Yes){
           WindowsFirewall::WindowsFirewallTurnOn(fwProfile);
       }
    }
    BOOL fwPortEnabled = 0;
    WindowsFirewall::WindowsFirewallPortIsEnabled(fwProfile, D_PORT, NET_FW_IP_PROTOCOL_TCP, &fwPortEnabled);

    hr = WindowsFirewall::WindowsFirewallAddApp(
                fwProfile,
                QApplication::applicationFilePath().toStdWString().c_str(),
                QApplication::applicationName().toStdWString().c_str()
                );
    if (FAILED(hr))
    {
        printf("WindowsFirewallAddApp failed: 0x%08lx\n", hr);
    }
    if( fwPortEnabled != TRUE )
        hr = WindowsFirewall::WindowsFirewallPortAdd(fwProfile, D_PORT, NET_FW_IP_PROTOCOL_TCP,
                                                 (QApplication::applicationName() + "PORT").toStdWString().c_str());
    if (FAILED(hr))
    {
        printf("WindowsFirewallPortAdd failed: 0x%08lx\n", hr);
    }
    // Release the firewall profile.
    WindowsFirewall::WindowsFirewallCleanup(fwProfile);

    // Uninitialize COM.
    if (SUCCEEDED(comInit))
    {
        CoUninitialize();
    }
}

FileShare::FileShare(QWidget *parent) : QWidget(parent)
  , clientCount(0), server(0)//for data transfer
{
    /// Initialize the history widget
    initializeHistory();
    /// Initialize most widgets
    initializeWidgets();

    //    hClientHandle = NetOpenWlanHandle();
    //    WlanRegisterNotification(hClientHandle, WLAN_NOTIFICATION_SOURCE_ACM, TRUE,
    //                             (WLAN_NOTIFICATION_CALLBACK)pNotifyFunc, this, NULL, NULL);

    filesCount =  0;
    fileListIterator = 0;


    setFirewallException();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileShare::~FileShare()
{
    delete fileListIterator;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::initializeHistory()
{
    recent = new historyView(this);
    recent->setGeometry(3, 60, 595, height()-60);
    recent->setVisible(false);
    recent->setColumnCount(1);
    recent->verticalHeader()->setVisible(false);
    recent->horizontalHeader()->setVisible(false);
    recent->setSelectionBehavior(QAbstractItemView::SelectRows);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::initializeWidgets()
{

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint );
    resize(600, 500);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    ///initialize settings widget
    settings = new Settings(this);
    settings->resize(size());
    settings->move(2,60);
    settings->setVisible(false);

    ///Initialize hotspot button
    hotSpot = new HotspotButton(settings);
    hotSpot->setFixedSize(40,40);
    hotSpot->setText(tr("Start Hotspot"));
    hotSpot->setToolTip(tr("Start Hotspot"));
    hotSpot->setCheckable(true);
    hotSpot->move(10, 360);

    /// set system tray icon
    if( QSystemTrayIcon::isSystemTrayAvailable() )
    {
        sysTray = new QSystemTrayIcon(this);
        sysTray->setIcon(this->windowIcon());
        setTrayMenu();
    }

    titleBar = new TitleBarWidget(this);
    titleBar->setFixedSize(width(), 60);
    titleBar->move(0,0);

    home = new Home(this);
    home->resize(600, 450);
    home->move(20,50);

    setConnections();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QString sizePrefix(qint64 size)
{
    if(size < 1024)
        return QString("%1byte").arg(QString::number(size));
    if(size >= 1024 && size < 1048576 )
        return QString("%1KB").arg(QString::number(double(size)/1024, 'f', 2));
    if(size >= 1048576 && size < 1073741824)
        return QString("%1MB").arg(QString::number(double(size)/1048576, 'f', 2));
    return "";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::displaySettings()
{
    settings->setVisible(true);
    home->setVisible(false);
    recent->setVisible(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::startHotspot(bool state)
{
    if( state )
    {
        settings->startWifiHotspot();
        return;
    }
    settings->stopWifiHotspot();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    QColor color(255, 255, 250);
    painter.fillRect(rect(),color);

    painter.restore();
    QWidget::paintEvent(e);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::moveWindow(QPoint pos) { move(pos);}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FileShare::displayHome()
{
    home->setVisible(true);
    settings->setVisible(false);
    recent->setVisible(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::openSocket()
{
    client = new Client(settings->getSourcePath(),this);
    connect(client, SIGNAL(clientDisconnected()), this, SLOT(deleteSocket()));
    client->start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::join(bool state)

{
    if( state )
    {
        openSocket();
        home->setWlanStatus("Searching for host");
        if( client->findHost()){
            home->setWlanStatus("Connected");
        }
        else{
            home->setJoinButtonEnable();
            home->setWlanStatus("Can't find host");
        }

        return;
    }
    else
    {
        client->disconnectFromHost();
        client->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// connect most signals and slots
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::setConnections()
{
    connect(sysTray, &QSystemTrayIcon::activated, this, &FileShare::showWindow);
    connect(titleBar, &TitleBarWidget::moveWindow, this, &FileShare::moveWindow);
    connect(titleBar, &TitleBarWidget::minimizeWindow, this, &FileShare::minimizeWindow);
    connect(titleBar, &TitleBarWidget::closeWindow, this, &QWidget::close);
    connect(titleBar, &TitleBarWidget::displaySetting, this, &FileShare::displaySettings);
    connect(titleBar, &TitleBarWidget::displayHome, this, &FileShare::displayHome);
    connect(titleBar, &TitleBarWidget::displayRecent, this, &FileShare::displayRecent);
    connect(home, &Home::host, this, &FileShare::startHosting);
    connect(home, &Home::join, this, &FileShare::join);
    connect(settings, &Settings::showSettings, this, &FileShare::displaySettings);
    connect(hotSpot, &QAbstractButton::clicked, this, &FileShare::startHotspot);
    connect(home, &Home::ctrlOnClick, this, &FileShare::sendFolder);
    connect(home, &Home::sendFileButtonClick, this, &FileShare::sendFileOnly);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::startHosting( bool state )
{
    if( state ) // if button is checked
    {
        server = new miniserver(settings->getSourcePath(),this);
        server->_recent = recent;
        server->startServer();

        connect(settings, SIGNAL(pathChanged(QString)), server, SLOT(setPath(QString)));
        connect(server, SIGNAL(sendHistoryWidget(FileWidget*)), this, SLOT(setHistoryItem(FileWidget*)));
        connect(server, SIGNAL(setAvatarInfo(QString,QImage,QString)), this, SLOT(setAvatarInfo(QString,QImage,QString)));
        connect(server, SIGNAL(removeRow(int)), home, SLOT(removeClient(int)));
        home->setWlanStatus(tr("Listening on port: %1").arg(QString::number(D_PORT)));
    }
    else
    {
        home->setWlanStatus("Hosting stopped");
        server->pauseAccepting();
        server->stopServer();
        delete server;
        server = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::minimizeWindow()
{
    if( settings->isToTray())
    {
        sysTray->show();
        sysTray->showMessage(tr("FileShare"),tr("Minimize to tray, click to restore"));
        this->hide();
    }else
        this->showMinimized();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::displayRecent()
{
    recent->setVisible(true);
    home->setVisible(false);
    settings->setVisible(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::setTrayMenu(){
    menu = new QMenu(this);
    QList< QAction *> actions;

    exit = new QAction("Exit", this);
    sendFile = new QAction("Send file",this);

    connect(exit, SIGNAL(triggered()), this, SLOT(close()));
    //connect(sendFile, SIGNAL(triggered()), this, SLOT(startSending()));

    QAction *separator = new QAction(this);
    separator->setSeparator(true);

    actions << sendFile << separator << exit;
    menu->addActions(actions);
    sysTray->setContextMenu(menu);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::showWindow(QSystemTrayIcon::ActivationReason reason)
{
    switch( reason ){
    case QSystemTrayIcon::DoubleClick:
        this->show();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::setHistoryItem(FileWidget *widget)
{

    recent->newItem = true;
    //Current row must be 1 less than the new (row)

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    //int row = widget->getRow();
    if( recent->newItem )
    {
        widget->setVisible(true);
        int row = recent->rowCount();
        recent->setRowCount(row + 1);
        recent->verticalHeader()->resizeSection(row, 50);
        recent->horizontalHeader()->resizeSection(row, 630);
        recent->setCellWidget(row,0,widget);
        widget->setRow(row);
        connect(widget, SIGNAL(buttonClicked(int)), recent, SLOT(removeRow(int)));
        recent->newItem = false;
    }
    this->displayRecent();
}

void FileShare::setHistoryItem(QString path)
{
    FileWidget *widget = new FileWidget(path, recent);
    this->setHistoryItem(widget);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::setAvatarInfo(QString name, QImage image, QString address)
{
    /// change is possible in the next version
    home->setUserInformation(name, QPixmap::fromImage(image), address);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FileShare::showJoinMessage(bool value)
{
    if( !value ){
        QMessageBox::critical(this, tr("Socket Error"), tr("Can't find host to connect to"));
        return;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::sendFolder()
{
    filesList.push_back(QFileDialog::getExistingDirectory(this, QApplication::applicationName(), dest));
    sendNextFile(filesList, Client::FOLDERTYPE); //start sending from index zero or beginning
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::sendFileOnly()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, QApplication::applicationName(), dest);
    sendNextFile(fileList, Client::FILETYPE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::sendNextFile(QStringList &list, Client::FileType type )
{
    if(home->isHosting())
        server->writeToClient(list, 0,0);
    else
    {
        foreach(QString path, list){
            setHistoryItem(path);
            client->WriteToClientSocket(path, type);
        }
    }

    //server->resetConnections();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileShare::deleteSocket() { home->setJoinButtonEnable(); }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




