#include "arangodbprivate.h"
#include "arangodb.h"
#include "request.h"
#include "collection.h"
#include <QtDebug>

namespace Arango {

ArangoDBPrivate::ArangoDBPrivate(ArangoDB *parent) : QObject(parent), _db(parent)
{
}

ArangoDBPrivate::~ArangoDBPrivate()
{
    delete _dbinfo;
    delete _user;
    qDeleteAll(_coll_headers);
}

bool ArangoDBPrivate::loadDBInfo()
{
    if (_dbinfo != nullptr)
        return true;
    QString apiurl = _apiurl + "/database/current";
    Request req(apiurl, Net::GET, _user);
    if (req.sendWithResponseCode({200})) {
        _dbinfo = new DBDescriptor;
        QJsonObject o = req.dataToJsonObject()["result"].toObject();
        _dbinfo->id = o["id"].toString();
        _dbinfo->name = o["name"].toString();
        _dbinfo->path = o["path"].toString();
        _dbinfo->isSystem = o["isSystem"].toBool();
        return true;
    }
    return false;
}

int ArangoDBPrivate::loadCollectionHeaders()
{
    if (_collHeadersLoaded)
        return _coll_headers.count();
    int count = 0;
    QString apiurl = _apiurl + "/collection?excludeSystem=true";
    Request req(apiurl, Net::GET, _user);
    if (req.sendWithResponseCode({200})) {
        QJsonArray arr = req.dataToJsonObject()["collections"].toArray();
        for (QJsonValue v : arr) {
            QJsonObject o = v.toObject();
            CollectionInfo *info = new CollectionInfo;
            info->id = o["id"].toString();
            info->isSystem = o["isSystem"].toBool();
            info->name = o["name"].toString();
            info->status = (CollectionStatus)o["status"].toInt();
            info->type = (CollectionType)o["type"].toInt();
            info->isLoaded = _collections.value(info->name, nullptr) != nullptr;
            _coll_headers[info->name] = info;
        }
        _collHeadersLoaded = true;
    }
    return count;
}

bool ArangoDBPrivate::loadCollection(const QString &name)
{
    Collection *c = new Collection(name, _db);
    _collections[name] = c;
    _coll_headers[name]->isLoaded = true;
    return c;
}

} // namespace Arango
