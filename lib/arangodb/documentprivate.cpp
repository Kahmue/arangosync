#include "documentprivate.h"
#include "collection.h"
#include "request.h"
namespace Arango {

DocumentPrivate::DocumentPrivate(Document *parent) : QObject(parent), q_ptr(parent)
{
    _synced = false;
    _user = nullptr;
    c_ptr = nullptr;
//    db_ptr = nullptr;
    _handle = "Not yet implemented";
}
/*
void DocumentPrivate::copyMetaData()
{
    if (_data.contains("_key"))
        _key = _data.take("_key").toString();
    if (_data.contains("_rev"))
        _revision = _data.take("_rev").toString();
    if (_data.contains("_id"))
        _id = _data.take("_id").toString();
}

void DocumentPrivate::loadData(const QString &id)
{
    if (c_ptr == nullptr)
        return;
    Request req(_handle, Net::GET, _user);
    req.withMatchHeader(true, _revision);
    if (req.send()) {
        if (req.status() == 200) {
            _data = req.dataToJsonObject();
            copyMetaData();
            _synced = true;
        }
    }
}
*/
QJsonObject DocumentPrivate::delta() const
{
    QJsonObject o;
    for(QString s : _changed.keys()) {
        o[s] = _data.value(s);
    }
    return o;
}

} // namespace Arango
