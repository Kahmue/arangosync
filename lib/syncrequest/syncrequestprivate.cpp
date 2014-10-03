#include "syncrequestprivate.h"

#include <QtDebug>

namespace Net {

SyncRequestPrivate::SyncRequestPrivate(SyncRequest *parent) : QObject(parent), q_ptr(parent)
{
}

SyncRequestPrivate::~SyncRequestPrivate()
{
    delete reply;
}

void SyncRequestPrivate::sslErrorHandler(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_Q(SyncRequest);
    if (reply != this->reply) // Nicht zuständig
        return;
    if (sslErrorFlag & Net::Log) {
        for (QSslError e : errors) {
            emit q->logEvent("SyncRequest", e.errorString(), -1);
        }
    }
    if (sslErrorFlag & Net::Ignore)
        reply->ignoreSslErrors();
}

} // namespace Net
