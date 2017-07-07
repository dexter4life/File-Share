#ifndef CLIENTINFO
#define CLIENTINFO
#include <QtCore>
#include <string>

using std::string;

struct ClientInfo
{
    int type;
    QByteArray data;
};

Q_DECLARE_METATYPE(ClientInfo)

#endif // CLIENTINFO

