#include "collectionprivate.h"
#include "arangodb.h"
#include "collection.h"
#include "request.h"
#include "aqlquery.h"
#include <QtDebug>

namespace Arango {

CollectionPrivate::CollectionPrivate(Collection *parent) : QObject(parent), q_ptr(parent)
{
}

CollectionPrivate::~CollectionPrivate()
{
    delete _desc;
}

quint64 CollectionPrivate::getChecksum(ChecksumMode mode)
{
    QString apiurl = QString("%1/checksum?withRevisions=%2&withData=%3")
            .arg(_apiurl)
            .arg(bool2string(mode == ChecksumMode::withRevisions))
            .arg(bool2string(mode == ChecksumMode::withData));
    qDebug() << "REV" << apiurl;
    Request req(apiurl, Net::GET, _user);
    if (req.sendWithResponseCode({200})) {
        return (quint64)req.dataToJsonObject()["checksum"].toDouble();
    }
    return 0;
}

Document *CollectionPrivate::loadDocument(const QString &key, const QString &rev, MatchPolicy match)
{
    QString apiurl = db_ptr->apiUrl() + "/document/" + QString("%1/%2").arg(_name).arg(key);
    qDebug() << "DOCURL" << apiurl;
    Request req(apiurl, Net::GET, _user);
    if (match != MatchPolicy::None)
        req.withMatchHeader(match == MatchPolicy::IfMatch, rev);
    if (req.sendWithResponseCode({200})) {
        Document *doc = new Document(q_ptr, req.dataToJsonObject());
        _documents[key] = doc;
        if (_doc_headers[key].rev != doc->revision()) {
            _doc_headers[key].rev = doc->revision();
            _doc_headers[key].valid = true;
        }
        return doc;
    }
    return nullptr;
}

Document *CollectionPrivate::storeDocument(Document *doc)
{
    Q_Q(Collection);
    QString apiurl = db_ptr->apiUrl() + "/document?collection=" + _name;
    QJsonObject o = doc->jsonObject();
    if (!_desc->keyoptions.allowUserKeys)
        o.remove("_key");
    o.remove("_id");
    o.remove("_rev");
    Request req(apiurl, Net::POST, _user);
    req.withData(o);
    if (req.sendWithResponseCode({201, 202})) {
        o["_key"] = req.dataToJsonObject()["_key"].toString();
        o["_id"] = req.dataToJsonObject()["_id"].toString();
        o["_rev"] = req.dataToJsonObject()["_rev"].toString();
        Document *doc = new Document(q, o);
        DocumentDescriptor dd;
        dd.id = doc->id();
        dd.key = doc->key();
        dd.rev = doc->revision();
        dd.valid = true;
        _doc_headers[dd.key] = dd;
        _documents[doc->key()] = doc;
        return doc;
    }
    return nullptr;
}

bool CollectionPrivate::verifyRevision(Document *doc)
{
    if (doc->revision().isEmpty())
        return false;
    QString apiurl = db_ptr->apiUrl() + "/document/" + doc->id();
    Request req(apiurl, Net::HEAD, _user);
    if (req.sendWithResponseCode({200})) {
        Net::RequestData d = req.raw();
        qDebug() << "HEADER" << d.header;
        if (d.header.contains("Etag")) {
            QString etag = d.header.value("Etag");
            etag.remove("\"");
            qDebug() << etag << doc->revision();
            return etag == doc->revision();
        }
    }
    return false;
}

bool CollectionPrivate::removeDocument(Document *doc, MatchPolicy match)
{
    QString apiurl = db_ptr->apiUrl() + "/document/" + doc->id();
    Request req(apiurl, Net::DELETE, _user);
    if (match != MatchPolicy::None)
        req.withMatchHeader(match == MatchPolicy::IfMatch, doc->revision());
    return req.sendWithResponseCode(200);
}

bool CollectionPrivate::updateDocument(Document *doc, MatchPolicy match)
{
    if (doc->needsReplace())
        return replaceDocument(doc, match);
    QString apiurl = db_ptr->apiUrl() + "/document/" + doc->id();
    QJsonObject diffobj;
    for(QString s : doc->changedAttributes())
        diffobj[s] = doc->value(s);
    qDebug() << "updating" << apiurl << "with" << diffobj;
    Request req(apiurl, Net::PATCH, _user);
    if (match != MatchPolicy::None)
        req.withMatchHeader(match == MatchPolicy::IfMatch, doc->revision());
    req.withData(diffobj);
    return req.sendWithResponseCode({201, 202});
}

bool CollectionPrivate::replaceDocument(Document *doc, MatchPolicy match)
{
    QString apiurl = db_ptr->apiUrl() + "/document/" + doc->id();
    Request req(apiurl, Net::PUT, _user);
    req.withData(doc->jsonObject());
    qDebug() << "replacing" << apiurl << "with" << doc->jsonObject();
    if (match != MatchPolicy::None)
        req.withMatchHeader(match == MatchPolicy::IfMatch, doc->revision());
    return req.sendWithResponseCode({201, 202});
}

bool CollectionPrivate::loadDescription()
{
    QString apiurl = _apiurl + "/properties";
    Request req(apiurl, Net::GET, _user);
    if (req.sendWithResponseCode({200})) {
        _desc = new CollectionDescriptor;
        QJsonObject o = req.dataToJsonObject();
        _desc->fromJson(o);
        return true;
    }
    return false;
}

DocumentDescriptorMap CollectionPrivate::loadDocInfo()
{
    DocumentDescriptorMap map;
    QString query = QString("FOR p IN %1 RETURN { \"key\" : p._key, \"rev\" : p._rev, \"id\" : p._id }").arg(_name);
    AQLQuery *q = db_ptr->query(query);
    if (q->exec()) {
        while (q->next()) {
            DocumentDescriptor dd;
            dd.id = q->current()["id"].toString();
            dd.key = q->current()["key"].toString();
            dd.rev = q->current()["rev"].toString();
            dd.valid = true;
            map[dd.key] = dd;
        }
    }
    return map;
}

QString CollectionPrivate::getRevision() const
{
    Request req(_apiurl + "/revision", Net::GET, _user);
    if (req.sendWithResponseCode({200})) {
        return req.dataToJsonObject()["revision"].toString();
    }
    return QString();
}

} // namespace Arango
