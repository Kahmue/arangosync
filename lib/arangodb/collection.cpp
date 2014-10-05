#include "collection.h"
#include "collectionprivate.h"
#include "arangodb.h"
#include "aqlquery.h"

#include <QtDebug>

namespace Arango {

Collection::Collection(const QString &name, ArangoDB *parent) : QObject(parent), d_ptr(new CollectionPrivate(this))
{
    Q_D(Collection);
    d->db_ptr = parent;
    d->_apiurl = parent->apiUrl() + "/collection/" + name;
    d->_user = parent->user();
    d->_name = name;

    d->loadDescription();
    d->_doc_headers = d->loadDocInfo();
}

UserDescriptor *Collection::user()
{
    Q_D(Collection);
    return d->_user;
}

QString Collection::name() const
{
    Q_D(const Collection);
    return d->_name;
}

CollectionDescriptor *Collection::descriptor()
{
    Q_D(Collection);
    return d->_desc;
}

int Collection::count() const
{
    Q_D(const Collection);
    return d->_doc_headers.count();
}

QString Collection::revision() const
{
    Q_D(const Collection);
    return d->getRevision();
}

QString Collection::storedRevision() const
{
    Q_D(const Collection);
    return d->_stored_revision;
}

void Collection::storeRevision(const QString &rev)
{
    Q_D(Collection);
    d->_stored_revision = rev;
}

bool Collection::revisionChanged() const
{
    return revision() != storedRevision();
}

quint64 Collection::checksum(ChecksumMode mode)
{
    Q_D(Collection);
    return d->getChecksum(mode);
}

Document *Collection::document(const QString &key)
{
    Q_D(Collection);
    if (!d->_doc_headers.contains(key))
        return nullptr;
    if (d->_documents.contains(key))
        return d->_documents.value(key);
    else {
        Document *doc = d->loadDocument(key);
        if (doc != nullptr)
            d->_documents[key] = doc;
        return doc;
    }
    return nullptr;
}

Document *Collection::insert(Document *doc)
{
    Q_D(Collection);
    Document *t = d->storeDocument(doc);
    if (t != nullptr) {
        // invalidate checksum & revision
        return t;
    }
    return nullptr;
}

bool Collection::remove(Document *doc, MatchPolicy match)
{
    Q_D(Collection);
    if (d->_documents.value(doc->key()) != doc)
        return false;
    if (d->removeDocument(doc, match)) {
        d->_documents.remove(doc->key());
        delete doc;
        return true;
    }
    return false;
}

bool Collection::remove(const QString &key, MatchPolicy match)
{
    Q_D(Collection);
    if (!d->_documents.contains(key))
        return false;
    Document *doc = d->_documents.value(key);
    return remove(doc, match);
}

bool Collection::update(const QString key, MatchPolicy match)
{
    Q_D(Collection);
    if (!d->_documents.contains(key))
        return false;
    Document *doc = d->_documents.value(key);
    return update(doc, match);
}

bool Collection::update(Document *doc, MatchPolicy match)
{
    Q_D(Collection);
    if (doc->revision().isEmpty())
        return false;
    if (d->_documents.value(doc->key()) != doc)
        return false;
    if (d->updateDocument(doc, match)) {
        return true;
    }
    return false;
}

bool Collection::replace(const QString &key, MatchPolicy match)
{
    Q_D(Collection);
    if (!d->_documents.contains(key))
        return false;
    Document *doc = d->_documents.value(key);
    return replace(doc, match);
}

bool Collection::replace(Document *doc, MatchPolicy match)
{
    Q_D(Collection);
    if (doc->revision().isEmpty())
        return false;
    if (d->_documents.value(doc->key()) != doc)
        return false;
    if (d->updateDocument(doc, match)) {
        return true;
    }
    return false;
}

bool Collection::verifyRevision(const QString &key)
{
    Q_D(Collection);

    Document *doc = d->_documents.value(key);

    if (doc != nullptr)
        return d->verifyRevision(doc);
    return false;
}

bool Collection::verifyRevision(Document *doc)
{
    Q_D(Collection);
    return d->verifyRevision(doc);
}

QStringList Collection::filter(const QString &fclause)
{
    Q_D(Collection);
    QStringList result;
    QString fstring = "FOR item IN " + name() + " FILTER " + fclause + " RETURN { \"_key\" : item._key }";
    AQLQuery *q = d->db_ptr->query(fstring);
    if (!q->isQueryValid()) {
        qDebug() << "FILTER is invalid" << q->error();
        return QStringList();
    }
    if (!q->exec()) {
        qDebug() << "FILTER Query failed" << q->error();
        return QStringList();
    }
    qDebug() << "FILTER COUNT" << q->count();
    for (QJsonValue v : q->objects()) {
        result << v.toObject()["_key"].toString();
    }
    return result;
}

Document *Collection::operator[](const QString &key)
{
    return document(key);
}

QStringList Collection::keys() const
{
    Q_D(const Collection);
    return d->_documents.keys();
}

} // namespace Arango
