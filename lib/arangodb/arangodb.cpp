#include "arangodb.h"
#include "arangodbprivate.h"
#include "request.h"
#include "aqlquery.h"
#include <QtDebug>

namespace Arango {


bool ArangoDB::createDB(const QString &hosturl, const QString &name, UserVector user, UserDescriptor *curuser)
{
    QString apiurl = QString("%1/_api/database").arg(hosturl);
    QJsonObject o;
    o["name"] = name;
    QJsonArray ua;
    if (!user.isEmpty()) {
        for(UserDescriptor ud : user) {
            QJsonObject u;
            u["username"] = ud.username;
            u["passwd"] = ud.password;
            u["active"] = ud.activeUser;
            ua.append(u);
        }
    }
    o["users"] = ua;
    Request req(apiurl, Net::POST, curuser);
    req.withData(o);
    return req.sendWithResponseCode({201});
}

bool ArangoDB::dropDB(const QString &hosturl, const QString &name, UserDescriptor *curuser)
{
    QString apiurl = QString("%1/_api/database/%2").arg(hosturl).arg(name);
    Request req(apiurl, Net::DELETE, curuser);
    return req.sendWithResponseCode({200});
}

QStringList ArangoDB::listDatabases(const QString& hosturl, UserDescriptor *user)
{
    QStringList result;
    QString apiurl = hosturl + "/_api/database";
    if (user != nullptr)
        apiurl += "/user";
    Request req(apiurl, Net::GET);
    if (user != nullptr)
        req.withUser(user);
    if (req.sendWithResponseCode(200)) {
        QJsonArray a = req.dataToJsonObject()["result"].toArray();
        for(QJsonValue v : a) {
            result << v.toString();
        }
    }
    return result;
}

ArangoDB::ArangoDB(const QString& hosturl, const QString &name, UserDescriptor *user, QObject *parent) : ArangoDB(hosturl, name, parent)
{
    Q_D(ArangoDB);
    d->_user = user;
    d->loadCollectionHeaders();
}

ArangoDB::ArangoDB(const QString &hosturl, const QString &name, QObject *parent) : ArangoDB(parent)
{
    Q_D(ArangoDB);
    d->_baseurl = hosturl;
    d->_apiurl = hosturl + "/_db/" + name + "/_api";
    d->loadCollectionHeaders();
}

void ArangoDB::setUserContext(UserDescriptor *user)
{
    Q_D(ArangoDB);
    d->_user = user;
}

QString ArangoDB::apiUrl() const
{
    Q_D(const ArangoDB);
    return d->_apiurl;
}

QString ArangoDB::baseUrl() const
{
    Q_D(const ArangoDB);
    return d->_baseurl;
}

DBDescriptor *ArangoDB::descriptor()
{
    Q_D(ArangoDB);
    if (d->_dbinfo == nullptr) {
        if (d->loadDBInfo())
            return d->_dbinfo;
    }
    return nullptr;
}

CollectionInfoPtrVector ArangoDB::collectionInfo()
{
    Q_D(ArangoDB);
    CollectionInfoPtrVector res;
    for(CollectionInfo *c : d->_coll_headers)
        res.append(c);
    return res;
}

UserDescriptor *ArangoDB::user() const
{
    Q_D(const ArangoDB);
    return d->_user;
}

QString ArangoDB::error() const
{
    Q_D(const ArangoDB);
    return d->_error_str;
}

AQLQuery *ArangoDB::query(const QString &query, int batchSize)
{
    AQLQuery *q = new AQLQuery(query, batchSize, this);
    return q;
}

bool ArangoDB::createCollection(CollectionDescriptor &desc)
{
    Q_D(ArangoDB);
    QString apiurl = d->_apiurl + "/collection";
    Request req(apiurl, Net::POST, d->_user);
    req.withData(desc.toJson());
    if (req.sendWithResponseCode({200})) {
        d->_collHeadersLoaded = false;
        d->loadCollectionHeaders();
        return true; // TODO
    }
    return false;
}

bool ArangoDB::dropCollection(const QString& name)
{
    Q_D(ArangoDB);
    if (!d->_collections.contains(name))
        return false;
    QString apiurl = d->_apiurl + "/collection/" + name;
    Request req(apiurl, Net::DELETE, d->_user);
    if (req.sendWithResponseCode({200})) {
        delete d->_coll_headers.take(name);
        delete d->_collections.take(name);
        return true;
    }
    if (req.status() == 404)
        d->_error_str = QString("Collection '%1' is unknown").arg(name);
    return false;
}

QStringList ArangoDB::collections() const
{
    Q_D(const ArangoDB);
    return d->_coll_headers.keys();
}

Collection *ArangoDB::operator[](const QString &id)
{
    Q_D(ArangoDB);
    if (d->_coll_headers.contains(id)) {
        if (!d->_coll_headers[id]->isLoaded) {
            if (d->loadCollection(id))
                return d->_collections[id];
            return nullptr;
        }
        return d->_collections[id];
    }
    return nullptr;
}

ArangoDB::ArangoDB(QObject *parent) : QObject(parent), d_ptr(new ArangoDBPrivate(this))
{

}

} // namespace Arango
