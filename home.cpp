/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "home.h"
#include "ui_home.h"
#include "platform.h"
#include "profilepicture.h"
#include <VersionHelpers.h>
#include "WlanInterface.h"
#include "clientviewmodel.h"
#include <QtWidgets>
#include <QStorageInfo>
#include <QSysInfo>
#include "sendfilebutton.h"
#include <QSignalMapper>
#include "global.h"

void Home::setHomeInformation()
{

    ui->userName->setText("Welcome, " + QString::fromStdString(Platform::getRegValue(L"FirstName")) + " "
                          + QString::fromStdString(Platform::getRegValue(L"LastName")));
    HANDLE hClientHandle = 0;
    hClientHandle = NetOpenWlanHandle();
    PWLAN_INTERFACE_INFO_LIST interfaceList;
    NetEnumWlanInterfaces(hClientHandle, &interfaceList);

    ui->systemInfo->setText(QString::fromWCharArray(interfaceList->InterfaceInfo[0].strInterfaceDescription));
    ui->hostName->setText(Platform::getHostname());

    ui->label_2->setText(QSysInfo::currentCpuArchitecture());
    ui->label_4->setText("Status:  " + QString::fromStdString(interfaceStateToString(interfaceList->InterfaceInfo[0].isState)));
    ui->label_5->setText(QSysInfo::productType() + " " + QSysInfo::productVersion());


    /// set the state for other widgets in home view
    connect(ui->hostConnection, &QAbstractButton::clicked, this, &Home::hostConnection_Clicked);
    connect(ui->joinConnection, &QAbstractButton::clicked, this, &Home::joinConnection_Clicked);
    connect(ui->sendFile, &SendFileButton::ctrlOnClick, this, &Home::ctrlOnClick);
    connect(ui->sendFile, &QAbstractButton::clicked, this, &Home::sendFileButtonClick);
}

void Home::setStorageInfo()
{
    qint64 byteUsed = (double( storageInfo.bytesTotal() - storageInfo.bytesFree()))/1073741824;
    double totalByte = ( (double)storageInfo.bytesTotal() / 1073741824 );
    double temp = (byteUsed / totalByte) * 100;
    double byteFree = (double)storageInfo.bytesFree() / 1073741824;
    ui->totalByteLabel->setText(QString("Internal Storage (%1GB) ").arg(
                                    QString::number(totalByte, 'f', 2)));
    ui->progressBar->setValue(temp);
    ui->progressBar->setAcceptDrops(false);

    ui->freeSpace->setText(QString("%1GB Available").arg(QString::number(byteFree, 'f', 2)));
}

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home),
    storageInfo(QDir::rootPath())
{
    ui->setupUi(this);

    setStorageInfo();

    profilePix = new ProfilePicture(this);

    if( IsWindows8OrGreater()){
        profilePix->setPixmap(Platform::readAccountPix(Platform::getAvatarPath()));
    }
    else
        profilePix->setPixmap(QPixmap(Platform::getAvatarPath()));


    profilePix->setFixedSize(80,80);
    profilePix->setStyleSheet("QWidget {background: red}");
    profilePix->move(10,20);

    setHomeInformation();

    ui->groupBox_2->setVisible(false);
    ui->line->setVisible(false);

    oldWidth = ui->infoGroup->width();
    ui->infoGroup->setFixedWidth(550);
    newWidth = ui->infoGroup->width();

    ui->hostConnection->setToolTip("Press to start hosting");
    ui->joinConnection->setToolTip("Press to connect");

}

Home::~Home()
{
    delete ui;
}

void Home::setUserName(QString name)
{
    ui->userName->setText(name);
}
void Home::setUserInformation(QString userName, QPixmap avatarData, QString address){
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, userName);
    item->setData(Qt::ToolTipRole, address);
    item->setData(Qt::DecorationRole, QIcon(avatarData));
    ui->listWidget->addItem(item);
}
QList<QListWidgetItem*> Home::selectedClient() const
{
   return ui->listWidget->selectedItems();
}
void Home::setWlanStatus(QString status){
    ui->label_4->setText("Status: " + status);
}

void Home::setJoinButtonEnable()
{
    ui->joinConnection->setEnabled(true);
    if( ui->joinConnection->isChecked() )
    {
        ui->joinConnection->setChecked(false);
        ui->joinConnection->setToolTip("Press to connect");
    }
    ui->hostConnection->setEnabled(true);
}

bool Home::isHosting(){

    return ui->hostConnection->isChecked();
}


void Home::joinConnection_Clicked(bool state)
{

    if( state )
    {
        ui->joinConnection->setToolTip("Press to disconnect");
        ui->hostConnection->setDisabled(true);
        ui->sendFile->setEnabled(true);
    }
    else
    {
        ui->sendFile->setEnabled(false);
        ui->joinConnection->setToolTip("Press to connect");
        ui->hostConnection->setEnabled(true);
    }
    emit join(state);
}

void Home::hostConnection_Clicked(bool state)
{
    if( state )
    {
        ui->hostConnection->setToolTip("Press to stop hosting");
        ui->line->setVisible(true);
        ui->groupBox_2->setVisible(true);
        ui->joinConnection->setDisabled(true);
        ui->sendFile->setEnabled(true);
        ui->infoGroup->setFixedWidth(oldWidth);
    }
    else
    {
        ui->sendFile->setEnabled(false);
        ui->hostConnection->setToolTip("Press to start hosting");
        ui->joinConnection->setEnabled(true);
        ui->infoGroup->setFixedWidth(newWidth);
        ui->line->setVisible(false);
        ui->groupBox_2->setVisible(false);

    }

    emit host(state);
}

void Home::removeClient(int row)
{
     ui->listWidget->removeItemWidget( ui->listWidget->item(row));
}
