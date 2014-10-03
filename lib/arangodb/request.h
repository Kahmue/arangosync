#ifndef ARANGO_REQUEST_H
#define ARANGO_REQUEST_H

#include "syncrequest.h"
#include "arangodata.h"
#include <QStringList>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include "arangodb_global.h"

namespace Arango {

class RequestPrivate;

class ARANGODBSHARED_EXPORT Request
{
public:
    Request(const QString& url, Net::RequestMethod method = Net::GET, UserDescriptor *user = nullptr);
    ~Request();
    Request& withData(const QJsonObject& data);
    Request& withData(const QJsonArray& data);
    Request& withData(const QByteArray& data);
    Request& withUser(UserDescriptor* user);
    Request& withParameters(const StringMap& parameters);
    Request& withMatchHeader(bool match, const QString& etag);
    int status() const;
    bool statusEqualsOneOf(QVector<int> testvalues);
    QString url() const;
    QJsonObject dataToJsonObject();
    QJsonArray dataToArray();
    QByteArray data();
    bool send();
    bool sendWithResponseCode(QVector<int> codes);
    bool sendWithResponseCode(int code);
    Net::RequestData raw() const;
    QString error();

private:
    Request();
    RequestPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Request)
};

} // namespace Arango

#endif // ARANGO_REQUEST_H
