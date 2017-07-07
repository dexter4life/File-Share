#ifndef PLATFORM_H
#define PLATFORM_H

#include <map>
#include <Windows.h>
#include <VersionHelpers.h>
#include <QString>
#include <QImage>
#include <QPixmap>

typedef std::map<char *, std::string> StringMap;

class Platform
{
public:
    static QString getSystemUsername();
    static QString getHostname();
    static QString getAvatarPath();
    static QString getDefaultPath();
    static StringMap &getAccountPictureInfo();

    static QString getValue(char *key){
        if ( tmpList.empty() ){
            InitAccountInfo();
        }
        return QString::fromStdString(tmpList[key]);
    }

    static std::string getRegValue(const wchar_t *regName);
    static void InitAccountInfo();
    static QPixmap readAccountPix(QString location);
    static QString adapterIpAddress();
private:
    static StringMap tmpList;
    Platform() = delete;
};

#endif // PLATFORM_H
