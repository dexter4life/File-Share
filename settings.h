#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include "NetworkSharing.h"
#include "WlanInterface.h"
#include "NetwokSelectionBox.h"

#pragma comment(lib, "Wlanapi.lib")

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    bool isToTray();
    bool canSendAvatar() const;
    void setConnectedDevice(QListWidgetItem *item);
    void setIniSettings();
    QString getSourcePath() const;
    bool isWifiOnlyCheck();
public slots:
    void stopWifiHotspot();
    void startWifiHotspot();
    void enableNetworkSharing(int item);
    void disableNetworkSharing(int item);
private slots:
    void showPassword(bool state);
    void hidePassword();
private:
    Ui::Settings *ui;
    void enumNetworkConnections();
    HANDLE hClientHandle;
    NetworkSharing networks;
    bool wifiState;
    QString currentPath;
signals:
    void showSettings();
    void pathChanged(QString path);
};

#endif // SETTINGS_H
