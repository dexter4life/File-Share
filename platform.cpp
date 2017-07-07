/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "platform.h"
#include <QString>
#include <QHostInfo>
#include <QFile>
#include <QDir>
#include <malloc.h>
#include <sstream>
#include <stdio.h>
#include <QStandardPaths>
#include <fstream>
#include <QPixelFormat>
#include <atlbase.h> //CRegKey
#include <QMessageBox>


#define TOTALBYTES    _MAX_FNAME

using std::ostringstream;

#pragma comment(lib, "Advapi32.lib")

#include <string>
#include <map>
using std::string;

// Returns the system username
QString Platform::getSystemUsername()
{
    // Save in a static variable so that It's always ready
    static QString username = "";
    if (username != "") return username;

    // Looking for the username
    QString uname(getenv("USERNAME"));
    if (uname == "") uname = getenv("USER");
    if (uname == "") uname = "Unknown";
    uname = uname.left(1).toUpper() + uname.mid(1);
    username = uname;

    return uname;
}

// Returns the hostname
QString Platform::getHostname()
{
    // Save in a static variable so that It's always ready
    static QString hostname = "";
    if (hostname != "") return hostname;

    // Get the hostname
    // (replace ".local" for MacOSX)
    hostname = QHostInfo::localHostName().replace(".local", "");

    return hostname;
}
// Returns the platform avatar path
QString Platform::getAvatarPath()
{
    QString path;
    if( IsWindows8OrGreater())
    {
        wchar_t sourceId[] = L"SourceID";
        path = QString("%1%2\\%3%4").arg(getenv("APPDATA")).arg("\\Microsoft\\Windows\\AccountPictures")\
                .arg(getRegValue(sourceId).c_str()).arg(".accountpicture-ms");
    }
    else  if( !QFile::exists(path) )
    {
        QString username = getSystemUsername().replace("\\", "+");
        path = QString(getenv("LOCALAPPDATA")) + "\\Temp\\" + username + ".bmp";
        if( !QFile::exists(path))
            path = QString(getenv("PROGRAMDATA")) + "\\Microsoft\\User Account Pictures\\" + username + ".bmp";
        if (!QFile::exists(path))
            path = QString(getenv("PROGRAMDATA")) + "\\Microsoft\\User Account Pictures\\Guest.bmp";
        if (!QFile::exists(path))
            path = QString(getenv("ALLUSERSPROFILE")) + "\\" + QDir(getenv("APPDATA")).dirName() + "\\Microsoft\\User Account Pictures\\" + username + ".bmp";
        if (!QFile::exists(path))
            path = QString(getenv("ALLUSERSPROFILE")) + "\\" + QDir(getenv("APPDATA")).dirName() + "\\Microsoft\\User Account Pictures\\Guest.bmp";
    }
    return path;
}

// Returns the platform default output path
QString Platform::getDefaultPath()
{
    return QString(getenv("USERPROFILE")) + "\\Desktop";
}

std::string Platform::getRegValue(const wchar_t *regName){
    //Open Current User Key
    HKEY hKeyCurrentUser;
    RegOpenCurrentUser(KEY_READ, &hKeyCurrentUser);

    DWORD bufferSize = 200;
    char *buffer = (char*)malloc(bufferSize);
    ZeroMemory(buffer, bufferSize);

    RegGetValue(hKeyCurrentUser, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\AccountPicture"),
        regName, RRF_RT_ANY, 0, buffer, &bufferSize);

    std::string stringValue;

    for (DWORD i = 0; i < bufferSize; i++){
        if (isalnum(buffer[i]))
            stringValue.push_back(buffer[i]);
    }
    stringValue.append("\0");

    free(buffer);
    RegCloseKey(hKeyCurrentUser);
    return stringValue;
}

void Platform::InitAccountInfo()
{
    //tmpList["pictureName"] = getRegValue(TEXT("SourceId"));
    //tmpList["firstName"] = getRegValue(TEXT("FirstName"));
    //tmpList["lastName"] = getRegValue(TEXT("LastName"));
}
QPixmap Platform::readAccountPix( QString location)
{
    QFile _px(location);
    if( !_px.open(QIODevice::ReadOnly))
        return QPixmap(); // can't read a thing

    QByteArray _px_image;
    _px_image.append(_px.readAll());
    if( !_px_image.contains("JFIF"))
        return QPixmap();
    //locate the big picture
    int fmt_pos = _px_image.indexOf("JFIF", _px_image.indexOf("JFIF")+1);
    //move 6 byte backward to get image 0 location in byte
    fmt_pos -= 6;

    QImage image = QImage::fromData(_px_image.mid(fmt_pos, _px_image.length()));

    //close file
    _px.close();
    return QPixmap::fromImage(image);
}

QString Platform::adapterIpAddress()
{
    CRegKey regKey;

    if (ERROR_SUCCESS !=
            regKey.Open(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters")))
    {
        QMessageBox::critical(NULL, QLatin1String("Wlan Error: "), QLatin1String("Unable to get Wlan Ip Address for registry"));
        return "";
    }

    LPTSTR buffer = (LPTSTR)malloc(TOTALBYTES);
    ZeroMemory(buffer, TOTALBYTES);
    ULONG size = TOTALBYTES;
    regKey.QueryStringValue(TEXT("StandaloneDhcpAddress"), buffer, &size);

    QString temp = QString::fromWCharArray(buffer);
    free(buffer);
    return temp;
}

StringMap Platform::tmpList;
