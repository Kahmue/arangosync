#ifndef NET_SYNCREQUESTPRIVATE_H
#define NET_SYNCREQUESTPRIVATE_H

#include <QtCore>
#include <QtNetwork>
#include "syncrequest.h"
#include "net.h"
namespace Net {

class SyncRequestPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SyncRequestPrivate(SyncRequest *parent);
    ~SyncRequestPrivate();
    QString error;
    RequestData reqdata;
    RequestData resdata;
    SyncRequest *q_ptr;
    int sslErrorFlag;
    QNetworkReply *reply;
    QNetworkAccessManager *nm;
    Q_DECLARE_PUBLIC(SyncRequest)
signals:

public slots:
    void sslErrorHandler(QNetworkReply *reply, const QList<QSslError>& errors);
};

} // namespace Net

#endif // NET_SYNCREQUESTPRIVATE_H
