#include "document.h"
#include "documentprivate.h"
#include "collection.h"
#include <QUuid>

#include <QtDebug>

namespace Arango {

Document::Document() : QObject(nullptr), d_ptr(new DocumentPrivate(this))
{
    Q_D(Document);
    d->_synced = false;
    d->_dataValid = true;
}

Document::Document(Collection *parent, const QJsonObject &o) : Document()
{
    Q_D(Document);
    d->_data = o;
    d->_synced = true;
    d->_dataValid = true;
    setParent(parent);
}

Document::Document(const QJsonObject &o) : Document()
{
    Q_D(Document);
    d->_data = o;
    d->_synced = false;
    //d->copyMetaData();
    d->_dataValid = true;
}

void Document::setParent(Collection *coll)
{
    Q_D(Document);
    QObject::setParent(coll);
    d->c_ptr = coll;
}
/*
Document::Document(Collection *parent) : Document()
{
    Q_D(Document);
    setParent(parent);
    registerWithCollection(parent);
    d->_dataValid = true;
    d->_synced = false;
}


Document::Document(Collection *parent, const QJsonObject &data) : Document(parent)
{
    Q_D(Document);
    d->_data = data;
    d->copyMetaData();
    d->_synced = false;
    d->_dataValid = true;
}

Document::Document(Collection *parent, const QString &key, const QString &rev) : Document(parent)
{
    Q_D(Document);
    d->_key = key;
    d->_revision = rev;
    d->_id = QString("%1/%2").arg(parent->name()).arg(key);
    d->_handle = d->db_ptr->apiUrl() + "/document/" + d->_id;
    d->_dataValid = false;
    d->_synced = true;
    //d->loadData(id);
}
*/
Document::~Document()
{
    //Q_D(Document);
}

QString Document::generateUid()
{
    QString id = QUuid::createUuid().toString().remove("{").remove("}");
    setValue("_key", id);
    return id;
}

/*
 * Document::sync synchronizes the current object with the DB
 * possible states of Document befor sync are:
 *  1. Document is in memory but not on disc -> store new Document
 *  2. Document is in memory and on disc, same revision -> do nothing
 *  3. Document is in memory and on disc, revision is invalid -> update disc
 *  4. Document is on disc but not in memory -> load Document
*/
/*
bool Document::sync()
{
    Q_D(Document);
    if (d->c_ptr == nullptr) // no connection to Database, no sync
        return false;
    if (!d->_synced) { // case 1
        QJsonObject o = d->_data;
        if (!d->_key.isEmpty() && d->c_ptr->descriptor().keyoptions.allowUserKeys)
            o["_key"] = d->_key;
        QString _apiurl = d->db_ptr->apiUrl() + "/document?collection=" + d->c_ptr->name();
        Request req(_apiurl, Net::POST, d->_user);
        req.withData(o);
        if (req.sendWithResponseCode({201, 202})) {
            d->_id = req.dataToJsonObject()["_id"].toString();
            d->_revision = req.dataToJsonObject()["_rev"].toString();
            d->_key = req.dataToJsonObject()["_key"];
            d->_synced = true;
            d->_dataValid = true;
            return true;
        }
        // store Document and return
    }
    if (!d->_dataValid) { // case 4
        Request req(d->_handle, Net::GET, d->_user);
        if (req.sendWithResponseCode({200})) {
            d->_data = req.dataToJsonObject();
            d->copyMetaData();
            return true;
        }
    } else {
        if (!isDirty()) // case 2
            return true;
        // case 3
        Request req(d->_handle, Net::PATCH, d->_user);
        req.withData(d->delta());
        if (req.sendWithResponseCode({201, 202})) {
            d->_revision = req.dataToJsonObject()["_rev"].toString();
            d->_changed.clear();
            d->_watch.clear();
            return true;
        }
    }
    return false;
}

void Document::registerWithCollection(Collection *c)
{
    Q_D(Document);
    d->c_ptr = c;
    d->db_ptr = c->db();
    d->_user = c->user();
}
*/
bool Document::isDirty()
{
    Q_D(Document);
    // Clean up attribute watchlist
    for (QString k : d->_watch.keys()) {
        if (!d->_data.contains(k)) { // if attribute is removed
            d->_changed[k] = AttributeState::Removed; // mark as removed
            d->_docNeedsReplace = true;
        } else if (d->_watch.value(k) != value(k)) { // if watched attribute has changed
            d->_changed[k] = AttributeState::Changed; // insert into changed set
        }
        d->_watch.remove(k); // remove it from watchlist
    }
    // If nothing has changed we're clean
    return !d->_changed.isEmpty();
}

QJsonValue Document::value(const QString &key) const
{
    Q_D(const Document);

    return d->_data.value(key);
}

QStringList Document::keys() const
{
    Q_D(const Document);
    return d->_data.keys();
}

void Document::setValue(const QString &key, const QVariant &value)
{
    Q_D(Document);
    QJsonValue v = QJsonValue::fromVariant(value);
    if (v != d->_data.value(key)) { // value is same as stored
        d->_data[key] = v; // replace value
        d->_changed[key] = AttributeState::Changed; // register attribute as changed
        emit documentChanged(key); // notify about change
    }
}

void Document::remove(const QString &key)
{
    Q_D(Document);
    if (!d->_data.contains(key))
        return;
    d->_data.remove(key);
    d->_docNeedsReplace = true;
    d->_changed[key] = AttributeState::Removed;
}

QJsonValueRef Document::operator[](const QString &key)
{
    Q_D(Document);
    if (!d->_watch.contains(key)) { // if key is not in watchlist
        d->_watch[key] = value(key); // store key and attribute in watchlist
    }
    return d->_data[key];
}

QJsonValue Document::operator [](const QString &key) const
{
    Q_D(const Document);
    return d->_data[key];
}

QJsonObject Document::jsonObject()
{
    Q_D(const Document);
    QJsonObject o = d->_data;
    return o;
}

bool Document::operator==(const Document &other) const
{
    Q_D(const Document);
    return d->_data == other.d_func()->_data;
}

int Document::count() const
{
    Q_D(const Document);
    return d->_data.size();
}

int Document::size() const
{
    return count();
}

QString Document::id() const
{
    Q_D(const Document);
    return d->_data.value("_id").toString();
}

bool Document::contains(const QString& key) const
{
    Q_D(const Document);
    return d->_data.contains(key);
}

QString Document::revision() const
{
    Q_D(const Document);
    return d->_data.value("_rev").toString();
}

bool Document::hasValidRevision()
{
    return !isDirty();
}
/*
QString Document::handle() const
{
    Q_D(const Document);
    return d->_handle;
}
*/
QString Document::key() const
{
    Q_D(const Document);
    return d->_data.value("_key").toString();
}

QStringList Document::changedAttributes()
{
    Q_D(Document);
    if (isDirty())
        return d->_changed.keys();
    return QStringList();
}

QStringList Document::watchedAttributes() const
{
    Q_D(const Document);
    return d->_watch.keys();
}

void Document::setClean()
{
    Q_D(Document);
    d->_watch.clear();
    d->_changed.clear();
    d->_docNeedsReplace = false;
}

bool Document::isEmpty() const
{
    Q_D(const Document);
    return d->_data.isEmpty();
}

void Document::removeNulls()
{
    Q_D(Document);
    for(QString v : d->_data.keys()) {
        if (d->_data.value(v).isNull())
            remove(v);
    }
}

bool Document::needsReplace() const
{
    Q_D(const Document);
    return d->_docNeedsReplace;
}
/*
bool Document::revisionIsValid()
{
    Q_D(const Document);
    if (!revision().isEmpty())
        return !isDirty();
    return true;
}
*/
} // namespace Arango
