#ifndef ARANGO_ARANGODBPRIVATE_H
#define ARANGO_ARANGODBPRIVATE_H

#include <QObject>
#include "arangodata.h"

namespace Arango {
class ArangoDB;
class Collection;

class ArangoDBPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ArangoDBPrivate(ArangoDB *parent = 0);
    ~ArangoDBPrivate();
    ArangoDB *_db;
    UserDescriptor *_user = nullptr;
    QString _baseurl;
    QString _apiurl;
    DBDescriptor *_dbinfo = nullptr;
    QString _error_str;
    bool _collHeadersLoaded = false;
    QMap<QString, CollectionInfo *> _coll_headers;
    QMap<QString, Collection *> _collections;
    bool loadDBInfo();
    int loadCollectionHeaders();
    bool loadCollection(const QString& name);
signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_ARANGODBPRIVATE_H
