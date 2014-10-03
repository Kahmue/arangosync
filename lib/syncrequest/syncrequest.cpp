#include "syncrequest.h"
#include "syncrequestprivate.h"
#include <QNetworkAccessManager>

#include <QtDebug>

namespace Net {

static int s_ssl_error_flag = Net::Default;
static QNetworkAccessManager * s_nam_ptr = nullptr;

/* ==================== DEBUG ==================== */
void printResponse(const RequestData& data)
{
    qDebug() << "==================== RequestData ====================";
    qDebug() << "StatusCode:" << data.status;
    qDebug() << "Method:" << data.method;
    qDebug() << "Url:" << data.url;
    qDebug() << "Header:";
    for(QString key : data.header.keys())
        qDebug() << "\t" << key << ":" << data.header.value(key);
    if (!data.data.isEmpty()) qDebug() << "Data:" << QString(data.data);
    qDebug() << "==================== End RequestData ====================";
}
/* ==================== DEBUG ==================== */



HeaderMap parseReplyHeaders(QNetworkReply *reply)
{
    HeaderMap result;
    for(QNetworkReply::RawHeaderPair rp : reply->rawHeaderPairs()) {
        result[QString(rp.first)] = QString(rp.second);
    }
    return result;
}

int parseReplyStatusCode(QNetworkReply *reply)
{
    QVariant v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (v.isValid())
        return v.toInt();
    return -1;
}

SyncRequest::SyncRequest(QObject *parent) : QObject(parent), d_ptr(new SyncRequestPrivate(this))
{
    Q_D(SyncRequest);
    d->sslErrorFlag = s_ssl_error_flag;
    if (s_nam_ptr != nullptr)
        d->nm = s_nam_ptr;
    else
        d->nm = new QNetworkAccessManager(d);
    connect(d->nm, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), d, SLOT(sslErrorHandler(QNetworkReply*,QList<QSslError>)));
}

SyncRequest::SyncRequest(const QString &url, QObject *parent) : SyncRequest(parent)
{
    Q_D(SyncRequest);
    d->reqdata.url = url;
}

SyncRequest::SyncRequest(const QString &url, const QByteArray &data, QObject *parent) : SyncRequest(parent)
{
    Q_D(SyncRequest);
    d->reqdata.url = url;
    d->reqdata.data = data;
}

SyncRequest::SyncRequest(const RequestData &data, QObject *parent) : SyncRequest(parent)
{
    Q_D(SyncRequest);
    d->reqdata = data;
}

QString SyncRequest::error() const
{
    Q_D(const SyncRequest);
    return d->error;
}

bool SyncRequest::send(RequestData &data)
{
    Q_D(SyncRequest);
    d->reqdata = data;
    return send();
}

bool SyncRequest::send()
{
    Q_D(SyncRequest);

    QUrl url = QUrl::fromUserInput(d->reqdata.url);
    if (!url.isValid()) {
        d->resdata.status = -1;
        emit logEvent("SyncRequest", QString("URL '%1' is invalid").arg(d->reqdata.url), -1);
        d->error = QString("Invalid URL '%1'").arg(d->reqdata.url);
        return false;
    }

    QBuffer *b_ptr = nullptr;
    if (!d->reqdata.data.isNull()) {
        b_ptr = new QBuffer(&d->reqdata.data);
        b_ptr->open(QIODevice::ReadOnly);
    }

    QNetworkRequest req(url); // Make Networkrequest

    for(QString header : d->reqdata.header.keys()) // Set user supplied header in request
        req.setRawHeader(header.toUtf8(), d->reqdata.header.value(header).toUtf8());

    switch(d->reqdata.method) {
    case GET:
        d->reply = d->nm->get(req);
        break;
    case PUT:
        d->reply = d->nm->put(req, b_ptr);
        break;
    case POST:
        d->reply = d->nm->post(req, b_ptr);
        break;
    case PATCH:
        d->reply = d->nm->sendCustomRequest(req, QString("PATCH").toUtf8(), b_ptr);
        break;
    case HEAD:
        d->reply = d->nm->head(req);
        break;
    case DELETE:
        d->reply = d->nm->deleteResource(req);
        break;
    }

    QEventLoop loop(this); // Construct loop to make request blocking
    connect(d->reply, SIGNAL(finished()), &loop, SLOT(quit())); // Finish loop when reply is finished
    loop.exec(); // Let loop run until reply is finished

    d->resdata.url = d->reqdata.url;
    d->resdata.method = d->reqdata.method;
    d->resdata.header = parseReplyHeaders(d->reply);
    d->resdata.status = parseReplyStatusCode(d->reply);
    d->resdata.data = d->reply->readAll();

    if (!d->reqdata.data.isNull()) {
        b_ptr->close();
        delete b_ptr;
    }
    return true;
}

RequestData SyncRequest::response()
{
    Q_D(SyncRequest);
    return d->resdata;
}

QString SyncRequest::replyError() const
{
    Q_D(const SyncRequest);
    return d->reply->errorString();
}

SyncRequest::~SyncRequest()
{
    delete d_ptr;
}

void setSslErrorFlags(int flag)
{
    s_ssl_error_flag = flag;
}

void setDefaultNetworkAccessManager(QNetworkAccessManager *mgr)
{
    s_nam_ptr = mgr;
}

} // namespace Net
