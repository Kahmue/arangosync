#ifndef ARANGO_COLLECTIONPRIVATE_H
#define ARANGO_COLLECTIONPRIVATE_H

#include <QObject>
#include "arangodata.h"
#include "document.h"

namespace Arango {
class Collection;
class ArangoDB;

class CollectionPrivate : public QObject
{
    Q_OBJECT
public:
    explicit CollectionPrivate(Collection *parent);
    ~CollectionPrivate();
    ArangoDB *db_ptr;
    QString _name;
    QString _apiurl;
    QString _stored_revision;
    UserDescriptor *_user;
    CollectionDescriptor *_desc = nullptr;
    DocumentDescriptorMap _doc_headers;
    int _docCount = -1;
    QMap<QString, Document *> _documents;
    quint64 getChecksum(ChecksumMode mode);
    Document *loadDocument(const QString& key, const QString& rev = QString(), MatchPolicy match = MatchPolicy::None);
    Document *storeDocument(Document *doc);
    bool verifyRevision(Document *doc);
    bool removeDocument(Document *doc, MatchPolicy match = MatchPolicy::None);
    bool updateDocument(Document *doc, MatchPolicy match);
    bool replaceDocument(Document *doc, MatchPolicy match);
    bool loadDescription();
    DocumentDescriptorMap loadDocInfo();
    QString getRevision() const;
    Collection *q_ptr;
    Q_DECLARE_PUBLIC(Collection)
signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_COLLECTIONPRIVATE_H
