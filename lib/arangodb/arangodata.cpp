#include "arangodata.h"
#include <QJsonDocument>
#include <QJsonArray>

namespace Arango {

void USERINFO_STR::fromJson(const QJsonObject &o)
{
    username = o["username"].toString();
    password = o["passwd"].toString();
    activeUser = o["active"].toBool();
    if (o.contains("extra"))
        extra = o["extra"].toObject();
}

QJsonObject USERINFO_STR::toJson()
{
    QJsonObject _response;
    _response["username"] = username;
    _response["passwd"] = password;
    _response["active"] = activeUser;
    _response["extra"] = extra;
    return _response;
}

QJsonObject KEYOPT_STR::toJson() {
    QJsonObject o;
    o["type"] = (type == KeyType::Traditional ? "traditional" : "autoincrement");
    o["allowUserKeys"] = allowUserKeys;
    if (type == KeyType::Autoincrement) {
        o["increment"] = increment;
        o["offset"] = offset;
    }
    return o;
}

void KEYOPT_STR::fromJson(const QJsonObject &o) {
    type = (o["type"].toString() == "traditional" ? KeyType::Traditional : KeyType::Autoincrement);
    allowUserKeys = o["allowUserKeys"].toBool();
    if (type == KeyType::Autoincrement) {
        increment = o["increment"].toInt();
        offset = o["offset"].toInt();
    }
}

QJsonObject COLINFO_STR::toJson() {
    QJsonObject o;
    o["name"] = name;
    o["waitForSync"] = waitForSync;
    o["doCompact"] = doCompact;
    o["isVolatile"] = isVolatile;
    if (keyoptions.type != KeyType::Undefined) {
        o["keyOptions"] = keyoptions.toJson();
    }
    o["type"] = (int)type;
    if (journalSize != -1)
        o["journalSize"] = journalSize;
    return o;
}

void COLINFO_STR::fromJson(const QJsonObject &o) {
    name = o["name"].toString();
    id = o["id"].toString();
    waitForSync = o["waitForSync"].toBool();
    doCompact = o["doCompact"].toBool();
    isVolatile = o["isVolatile"].toBool();
    type = (CollectionType)o["type"].toInt();
    status = (CollectionStatus)o["status"].toInt();
    journalSize = o["journalSize"].toInt();
    if (o.contains("keyOptions")) {
        keyoptions.fromJson(o["keyOptions"].toObject());
    }
}

}
