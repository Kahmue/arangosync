#include "request.h"
#include <QJsonDocument>

namespace Arango {

class RequestPrivate
{
public:
    RequestPrivate();

    Net::RequestData out;
    Net::RequestData in;
    QString error;
};

RequestPrivate::RequestPrivate()
{
    in.status = -1;
}

Net::RequestData _makeDefaultRequest()
{
    Net::RequestData r;
    r.header["Content-Type"] = "application/json;  charset=utf-8";
    r.status = -1;
    return r;
}

Request::Request() : d_ptr(new RequestPrivate) {}

Request::Request(const QString &url, Net::RequestMethod method, UserDescriptor *user) : Request()
{

    Q_D(Request);
    d->out = _makeDefaultRequest();
    d->out.method = method;
    d->out.url = url;
    if (user != nullptr)
        withUser(user);
}

Request::~Request()
{
    delete d_ptr;
}

Request &Request::withData(const QJsonObject &data)
{
    return withData(QJsonDocument(data).toJson());
}

Request &Request::withData(const QJsonArray &data)
{
    return withData(QJsonDocument(data).toJson());
}

Request &Request::withData(const QByteArray &data)
{
    Q_D(Request);
    d->out.data = data;
    return *this;
}

Request &Request::withUser(UserDescriptor *user)
{
    Q_D(Request);
    if (user != nullptr) {
        QString token = QString("%1:%2").arg(user->username).arg(user->password);
        d->out.header["Authorization"] = QString("Basic %1").arg(QString(token.toUtf8().toBase64()));
    }
    return *this;
}

Request &Request::withParameters(const StringMap &parameters)
{
    Q_D(Request);
    if (!parameters.isEmpty()) {
        if (parameters.contains("If-None-Match")) {
            d->out.header["If-None-Match"] = parameters.value("If-None-Match");
        }
        if (parameters.contains("If-Match")) {
            d->out.header["If-Match"] = parameters.value("If-Match");
        }
        QStringList _par;
        for(QString key : parameters.keys()) {
            if (key == "If-Match" || key == "If-None-Match")
                continue;
            _par << QString("%1=%2").arg(key).arg(parameters.value(key));
        }
        if (!_par.isEmpty())
            d->out.url += "?" + _par.join("&");
    }
    return *this;
}

Request &Request::withMatchHeader(bool match, const QString &etag)
{
    Q_D(Request);
    if (!etag.isEmpty()) {
        QString hdr = match ? "If-Match" : "If-None-Match";
        d->out.header[hdr] = etag;
    }
    return *this;
}

int Request::status() const
{
    Q_D(const Request);
    return d->in.status;
}

bool Request::statusEqualsOneOf(QVector<int> testvalues)
{
    int v = status();
    for(int i : testvalues) {
        if (v == i)
            return true;
    }
    return false;
}

QString Request::url() const
{
    Q_D(const Request);
    return d->out.url;
}

QJsonObject Request::dataToJsonObject()
{
    Q_D(Request);
    return QJsonDocument::fromJson(d->in.data).object();
}

QJsonArray Request::dataToArray()
{
    Q_D(Request);
    return QJsonDocument::fromJson(d->in.data).array();
}

QByteArray Request::data()
{
    Q_D(Request);
    return d->in.data;
}

bool Request::send()
{
    Q_D(Request);
    Net::SyncRequest r(d->out);
    if (r.send()) {
        d->in = r.response();
        return true;
    }
    d->error = r.error();
    return false;
}

bool Request::sendWithResponseCode(QVector<int> codes)
{
    if (send()) {
        return statusEqualsOneOf(codes);
    }
    return false;
}

bool Request::sendWithResponseCode(int code)
{
    if (send())
        return status() == code;
    return false;
}

Net::RequestData Request::raw() const
{
    Q_D(const Request);
    return d->in;
}

QString Request::error()
{
    Q_D(Request);
    return d->error;
}

} // namespace Arango
