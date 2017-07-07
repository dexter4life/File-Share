#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QProgressBar>
#include <QStorageInfo>
#include <QItemSelectionModel>
namespace Ui {
class Home;
}


class ClientViewModel;
class ProfilePicture;
class QListWidgetItem;

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = 0);
    ~Home();

    void setUserName(QString name);
    void setUserInformation(QString userName, QPixmap avatarData, QString address);
    void setHomeInformation();
    void setStorageSizeIndicator();
    void setStorageInfo();
    bool isHosting();
    QList<QListWidgetItem *> selectedClient() const;
private:
    ProfilePicture *profilePix;
    Ui::Home *ui;
    ClientViewModel *clientView;
    QProgressBar *storageIndicator;
    QStorageInfo storageInfo;
    int oldWidth, newWidth;
signals:
    void startHosting();
    void startSending();
    void ctrlOnClick();
    void sendFileButtonClick();
    void host(bool state);
    void join(bool state);
public slots:
    void removeClient(int row);
    void setJoinButtonEnable();
    void setWlanStatus(QString status);
    void joinConnection_Clicked(bool state);
    void hostConnection_Clicked(bool state);
};

#endif // HOME_H
