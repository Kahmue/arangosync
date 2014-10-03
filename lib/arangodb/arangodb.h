#ifndef ARANGO_ARANGODB_H
#define ARANGO_ARANGODB_H

#include <QObject>

#include "arangodb_global.h"
#include "arangodata.h"
#include "collection.h"
#include "aqlquery.h"

namespace Arango {

class ArangoDBPrivate;
//class Collection;
//class AQLQuery;

class ARANGODBSHARED_EXPORT ArangoDB : public QObject
{
    Q_OBJECT
public:
    static bool createDB(const QString& hosturl, const QString& name, UserVector user = UserVector(), UserDescriptor *curuser = nullptr);
    static bool dropDB(const QString& hosturl, const QString& name, UserDescriptor *curuser = nullptr);
    static QStringList listDatabases(const QString &hosturl, UserDescriptor *user = nullptr);
    explicit ArangoDB(const QString& hosturl, const QString& name, UserDescriptor *user, QObject *parent = 0);
    ArangoDB(const QString& hosturl, const QString& name, QObject *parent = nullptr);
    void setUserContext(UserDescriptor *user);
    QString apiUrl() const;
    QString baseUrl() const;
    DBDescriptor *descriptor();
    CollectionInfoPtrVector collectionInfo();
    UserDescriptor *user() const;
    QString error() const;
    // Query
    AQLQuery *query(const QString& query, int batchSize = 50);
    // Collections
    Collection *createCollection(CollectionDescriptor &desc);
    bool dropCollection(const QString &name);
    QStringList collections() const;
    Collection *operator[](const QString& id);
signals:

public slots:

private:
    ArangoDB(QObject *parent = nullptr);
    ArangoDBPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ArangoDB)
};

MK_SHR_PTR(ArangoDB);

} // namespace Arango

#endif // ARANGO_ARANGODB_H
