#ifndef NET_SYNCREQUEST_H
#define NET_SYNCREQUEST_H

#include <QObject>
#include "syncrequest_global.h"
#include "net.h"

class QNetworkAccessManager;

namespace Net {

void setSslErrorFlags(int flag);
void setDefaultNetworkAccessManager(QNetworkAccessManager *mgr);

class SyncRequestPrivate;

class SYNCREQUESTSHARED_EXPORT SyncRequest : public QObject
{
Q_OBJECT
public:
    SyncRequest(QObject *parent = 0);
    SyncRequest(const QString& url, QObject *parent = 0);
    SyncRequest(const QString& url, const QByteArray& data, QObject *parent = 0);
    SyncRequest(const RequestData& data, QObject *parent = 0);
    QString error() const;
    bool send(RequestData& data);
    bool send();
    RequestData response();
    QString replyError() const;
    ~SyncRequest();
signals:
    void logEvent(const QString& source, const QString& message, int loglevel);
private:
    SyncRequestPrivate *d_ptr;
    Q_DECLARE_PRIVATE(SyncRequest)
};

} // namespace Net
#endif // NET_SYNCREQUEST_H
