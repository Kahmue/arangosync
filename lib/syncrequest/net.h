#ifndef NET_H
#define NET_H

#include <QString>
#include <QMap>

namespace Net {

enum RequestMethod { GET = 1, HEAD, PUT, POST, PATCH, DELETE };
enum SslErrorFlags { Default = 0, Ignore = 1, Log };

typedef QMap<QString, QString> HeaderMap;

typedef struct REQ_STR {
    int status;
    QString url;
    Net::RequestMethod method;
    HeaderMap header;
    QByteArray data;
} RequestData;

}

#endif // NET_H
