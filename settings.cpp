/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "settings.h"
#include "ui_settings.h"
#include "platform.h"

#include <QNetworkInterface>
#include "netwokselectionbox.h"

Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings), hClientHandle(NULL)
{
    ui->setupUi(this);

    enumNetworkConnections();

    ui->password->setEchoMode(QLineEdit::Password);
    ui->passphrase->setChecked(true);
    ui->Persistent->setChecked(true);

    QDirModel *dirModel = new QDirModel(this);
    dirModel->setFilter(QDir::Dirs | QDir::NoSymLinks);
    ui->dirPath->setCompleter(new QCompleter(dirModel, this));

    ui->dirPath->setText(Platform::getDefaultPath());

    QSettings settings(QSettings::UserScope, QApplication::applicationName());
    settings.setSystemIniPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    settings.beginGroup(QLatin1String("UserSettings"));
    QString pass = settings.value(QLatin1String("Password")).toString();
    ui->password->setText(pass);
    ui->dirPath->setText(settings.value(QLatin1String("DirPath")).toString());
    ui->maxClient->setValue(settings.value(QLatin1String("MaxClient")).toInt());
    ui->wifiOnly->setChecked(settings.value(QLatin1Literal("WifiOnly")).toBool());
    ui->passphrase->setChecked(settings.value(QLatin1Literal("PassPhrase")).toBool());
    ui->ssid->setText(settings.value(QLatin1String("SSID")).toString());
    ui->minimizeToTray->setChecked(settings.value(QLatin1String("MinimizeToTray")).toBool());
    ui->sendAvatar->setChecked(settings.value(QLatin1String("SendAvatar")).toBool());
    ui->netSharing->setChecked(settings.value(QLatin1String("NetSharing")).toBool());
    ui->comboBox->setEnabled(settings.value(QLatin1String("NetSharing")).toBool());
    ui->Persistent->setChecked(settings.value(QLatin1String("Persistent")).toBool());
    ui->comboBox->setCurrentIndex(settings.value(QLatin1String("CurrentNetwork")).toInt());
    ui->comboBox->setCurrentText(settings.value(QLatin1String("SelectedNetwork")).toString());
    settings.endGroup();

    currentPath = ui->dirPath->text();

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(showPassword(bool)));
    //connect current item on the sharing list and disable the formal item before change
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(enableNetworkSharing(int)));
    connect(ui->comboBox, &NetwokSelectionBox::beforeCurrent, this, &Settings::disableNetworkSharing);
    connect(ui->dirPath, SIGNAL(textChanged(QString)), SIGNAL(pathChanged(QString)));
}

void Settings::setIniSettings()
{
    QSettings settings(QSettings::UserScope, QApplication::applicationName());
    settings.setSystemIniPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    settings.beginGroup(QLatin1Literal("UserSettings"));
    settings.setValue(QLatin1Literal("Password"), ui->password->text());
    settings.setValue(QLatin1Literal("DirPath"), ui->dirPath->text());
    settings.setValue(QLatin1Literal("MaxClient"), ui->maxClient->value());
    settings.setValue(QLatin1Literal("WifiOnly"), ui->wifiOnly->isChecked());
    settings.setValue(QLatin1Literal("PassPhrase"), ui->passphrase->isChecked());
    settings.setValue(QLatin1String("SSID"), ui->ssid->text());
    settings.setValue(QLatin1String("MinimizeToTray"), ui->minimizeToTray->isChecked());
    settings.setValue(QLatin1String("SendAvatar"), ui->sendAvatar->isChecked());
    settings.setValue(QLatin1String("NetSharing"), ui->netSharing->isChecked());
    settings.setValue(QLatin1String("Persistent"), ui->Persistent->isChecked());
    settings.setValue(QLatin1String("CurrentNetwork"), ui->comboBox->currentIndex());
    settings.setValue(QLatin1String("SelectedNetwork"), ui->comboBox->currentText());
    settings.endGroup();
}

Settings::~Settings()
{
    PWLAN_HOSTED_NETWORK_REASON pFailReason = 0;
    WlanHostedNetworkForceStop(hClientHandle, pFailReason, NULL);

    //save previous user settings
    setIniSettings();

    //remove the ui element
    delete ui;
}



void Settings::enumNetworkConnections(){
    NETCON_PROPERTIES *ppProps = NULL;
    auto netList = networks.getConnectionList();

    for (size_t i = 0; i < netList->size(); i++ ){
        netList->at(i)->GetProperties(&ppProps);
        ui->comboBox->addItem(QString::fromWCharArray(ppProps->pszwName));
    }
}

void Settings::disableNetworkSharing(int item)
{
    networks.disableSharing(networks.connectionAt(item-1));
}

void Settings::enableNetworkSharing(int item)
{
    //Little bug using this code here
    if( ui->netSharing->isChecked())
    {
          // enable sharing for the current item
            networks.enableSharing(networks.connectionAt(item-1), ICSSHARINGTYPE_PUBLIC);
    }
}

void Settings::startWifiHotspot(){

    if (ui->ssid->text().isEmpty() && ui->password->text().isEmpty()){
        QMessageBox::information(this, tr("Settings: "), tr("The Settings fields are empty, please enter "
                                                            "configuration for the wifi hotspot"), QMessageBox::Ok);
        return;
    }

    hClientHandle = NetOpenWlanHandle();

    PWLAN_HOSTED_NETWORK_REASON pFailReason = 0;

    NetInitWlanSettings(hClientHandle, pFailReason);

    NetSetWlanHostedNetworkEnable(
                hClientHandle,
                pFailReason,
                true
                );

 //   NetSetWlanAllowExplicitCreds(hClientHandle, true);

    NetSetWlanHostedNetworkSettings(
                hClientHandle,
                ui->ssid->text().toStdString(),
                ui->maxClient->value(),
                pFailReason);

    NetWlanHostedNetworkSetSecondaryKey(
                hClientHandle,
                ui->password->text().toStdString(),
                ui->passphrase->isChecked(),
                ui->Persistent->isChecked(),
                pFailReason);


    WlanHostedNetworkForceStart(hClientHandle, pFailReason, NULL);
    ///Update the INI Settings
    setIniSettings();
}

void Settings::stopWifiHotspot(){
    PWLAN_HOSTED_NETWORK_REASON pFailReason = 0;
    WlanHostedNetworkForceStop(hClientHandle, pFailReason, NULL );
}
bool Settings::isWifiOnlyCheck()
{ return ui->wifiOnly->isChecked();}

void Settings::showPassword(bool state )
{
    if( state )
        ui->password->setEchoMode(QLineEdit::Normal);
    else
        hidePassword();
}

void Settings::hidePassword(){ ui->password->setEchoMode(QLineEdit::Password); }



bool Settings::canSendAvatar() const { return ui->sendAvatar->isChecked(); }

void Settings::setConnectedDevice(QListWidgetItem *item)
{ ui->deviceConnected->addItem(item); }

bool Settings::isToTray() { return ui->minimizeToTray->isChecked(); }
QString Settings::getSourcePath() const { return currentPath; }
