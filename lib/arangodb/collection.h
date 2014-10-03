#ifndef ARANGO_COLLECTION_H
#define ARANGO_COLLECTION_H

#include <QObject>
#include "arangodb_global.h"
#include "arangodata.h"
#include "document.h"
namespace Arango {

class CollectionPrivate;
class ArangoDB;


class ARANGODBSHARED_EXPORT Collection : public QObject
{
    Q_OBJECT
public:
    explicit Collection(const QString& name, ArangoDB *parent = 0);
    UserDescriptor *user();
    CollectionDescriptor *descriptor();
    int count() const;
    QString revision() const;
    QString storedRevision() const;
    void storeRevision(const QString& rev);
    bool revisionChanged() const;
    quint64 checksum(ChecksumMode mode = ChecksumMode::keysOnly);
    Document *document(const QString& key);
    Document *insert(Document *doc);
    bool remove(Document *doc, MatchPolicy match = MatchPolicy::None);
    bool remove(const QString& key, MatchPolicy match = MatchPolicy::None);
    bool update(const QString key, MatchPolicy match = MatchPolicy::None);
    bool update(Document *doc, MatchPolicy match = MatchPolicy::None);
    bool replace(const QString& key, MatchPolicy match = MatchPolicy::None);
    bool replace(Document *doc, MatchPolicy match = MatchPolicy::None);
    bool verifyRevision(const QString& key);
    bool verifyRevision(Document *doc);
    Document *operator[](const QString& key);
    QStringList keys() const;
signals:

public slots:
private:
    CollectionPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Collection)
};

MK_SHR_PTR(Collection);

} // namespace Arango

#endif // ARANGO_COLLECTION_H
