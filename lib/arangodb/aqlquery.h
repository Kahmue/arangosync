#ifndef ARANGO_AQLQUERY_H
#define ARANGO_AQLQUERY_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "arangodata.h"

namespace Arango {

class AQLQueryPrivate;
class ArangoDB;

class AQLQuery : public QObject
{
    Q_OBJECT
public:
    explicit AQLQuery(const QString& query, int batchsize = 50, ArangoDB *parent = 0);
    bool isQueryValid();
    QString error() const;
    bool exec();
    bool next();
    bool first();
    bool last();
    bool previous();
    int count() const;
    QJsonObject current() const;
    QJsonValue value(const QString& key, const QJsonValue& defaultValue = QJsonValue()) const;
    QJsonArray objects();
    QJsonObject objectAt(int pos);
    bool fetchAll();
    bool atEnd() const;
    bool atBeginning() const;
    AQLQuery& operator++();
    AQLQuery& operator--();
signals:

public slots:
private:
    AQLQueryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(AQLQuery)
};

MK_SHR_PTR(AQLQuery);

} // namespace Arango

#endif // ARANGO_AQLQUERY_H
