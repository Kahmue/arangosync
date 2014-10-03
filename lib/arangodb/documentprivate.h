#ifndef ARANGO_DOCUMENTPRIVATE_H
#define ARANGO_DOCUMENTPRIVATE_H

#include <QObject>
#include <QJsonObject>
#include <QSet>

#include "document.h"
namespace Arango {

class Collection;
//class ArangoDB;

class DocumentPrivate : public QObject
{
    Q_OBJECT
public:
    explicit DocumentPrivate(Document *parent = 0);
    QString _handle;
    bool _synced; // Some version of this exists in DB
    bool _dataValid; // Documents data have been loaded
    bool _docNeedsReplace = false;
    QJsonObject _data;
    Collection *c_ptr;
//    ArangoDB *db_ptr;
    UserDescriptor *_user;
    QMap<QString, AttributeState> _changed;
    QMap<QString, QJsonValue> _watch;

//    void copyMetaData();

//    void loadData(const QString& id);
    QJsonObject delta() const;
    Document *q_ptr;
    Q_DECLARE_PUBLIC(Document)
signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_DOCUMENTPRIVATE_H
