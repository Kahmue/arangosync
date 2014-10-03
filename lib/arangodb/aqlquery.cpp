#include "aqlquery.h"
#include "aqlqueryprivate.h"
#include "arangodb.h"
#include "request.h"
#include <QJsonObject>

#include <QtDebug>

namespace Arango {

AQLQuery::AQLQuery(const QString &query, int batchsize, ArangoDB *parent) : QObject(parent), d_ptr(new AQLQueryPrivate(this))
{
    Q_D(AQLQuery);
    d->_query = query;
    qDebug() << "QUERY" << query;
    d->_batchsize = batchsize;
    d->db_ptr = parent;
    d->_apiurl = parent->apiUrl() + "/cursor";
    qDebug() << "QUERY API" << d->_apiurl;
    d->_reqObject["query"] = query;
    d->_reqObject["count"] = d->_count;
    d->_reqObject["batchSize"] = d->_batchsize;
}

bool AQLQuery::isQueryValid()
{
    Q_D(AQLQuery);
    QString apiurl = d->db_ptr->apiUrl() + "/query";
    QJsonObject o;
    o["query"] = d->_query;
    Request req(apiurl, Net::POST, d->db_ptr->user());
    req.withData(o);
    if (req.sendWithResponseCode({200})) {
        d->clearErrors();
        return true;
    }
    d->setError(req.dataToJsonObject()["errorNum"].toInt(), req.dataToJsonObject()["errorMessage"].toString());
    return false;
}

QString AQLQuery::error() const
{
    Q_D(const AQLQuery);
    return d->_error;
}

bool AQLQuery::exec()
{
    Q_D(AQLQuery);
    if (!isQueryValid())
        return false;
    Request req(d->_apiurl, Net::POST, d->db_ptr->user());
    req.withData(d->_reqObject);
    if (req.sendWithResponseCode({201})) {
        QJsonObject o = req.dataToJsonObject();
        d->_cursorid = o["id"].toString();
        d->_cursorurl = d->_apiurl + "/" + d->_cursorid;
        d->_loadMore = o["hasMore"].toBool();
        d->_counter = o["count"].toInt();
        d->_data = o["result"].toArray();
        d->clearErrors();
        return true;
    }
    d->setError(req.dataToJsonObject()["errorNum"].toInt(), req.dataToJsonObject()["errorMessage"].toString());
    return false;
}

bool AQLQuery::next()
{
    Q_D(AQLQuery);
    d->_pos++;
    if (d->_pos < d->_counter) {
        if (d->_pos < d->_data.count()) {
           return true;
        }
        if (d->loadBatch() > 0)
            return true;
        return false;
    }
    return false;
}

bool AQLQuery::first()
{
    Q_D(AQLQuery);
    if (d->_data.isEmpty())
        return false;
    d->_pos = 0;
    return true;
}

bool AQLQuery::last()
{
    Q_D(AQLQuery);
    if (d->_data.isEmpty())
        return false;
    fetchAll();
    d->_pos = d->_data.count() - 1;
    return true;
}

bool AQLQuery::previous()
{
    Q_D(AQLQuery);
    if (d->_pos > 0) {
        d->_pos--;
        return true;
    }
    return false;
}

int AQLQuery::count() const
{
    Q_D(const AQLQuery);
    return d->_counter;
}

QJsonObject AQLQuery::current() const
{
    Q_D(const AQLQuery);
    if (d->_pos >= 0 && d->_pos < d->_counter)
        return d->_data[d->_pos].toObject();
    return QJsonObject();
}

QJsonValue AQLQuery::value(const QString &key, const QJsonValue &defaultValue) const
{
    Q_D(const AQLQuery);
    if (d->checkBoundaries(d->_pos)) {
        if (d->_data.at(d->_pos).toObject().contains(key))
            return d->_data.at(d->_pos).toObject().value(key);
        return defaultValue;
    }
    return QJsonValue();
}

QJsonArray AQLQuery::objects()
{
    Q_D(AQLQuery);
    fetchAll();
    return d->_data;
}

QJsonObject AQLQuery::objectAt(int pos)
{
    Q_D(AQLQuery);
    if (d->checkBoundaries(pos)) {
        if (pos < d->_data.count())
            return d->_data.at(pos).toObject();
        else {
            fetchAll();
            return d->_data.at(pos).toObject();
        }
    }
    return QJsonObject();
}

bool AQLQuery::fetchAll()
{
    Q_D(AQLQuery);
    while (d->_loadMore)
        d->loadBatch();
    return true;
}

bool AQLQuery::atEnd() const
{
    Q_D(const AQLQuery);
    return d->_pos == (d->_counter - 1);
}

bool AQLQuery::atBeginning() const
{
    Q_D(const AQLQuery);
    return d->_pos >= 1;
}

AQLQuery &AQLQuery::operator++()
{
    if (!atEnd())
        next();
    return *this;

}

AQLQuery &AQLQuery::operator--()
{
    Q_D(AQLQuery);
    if (!d->_pos == 0)
        previous();
    return *this;
}

} // namespace Arango
