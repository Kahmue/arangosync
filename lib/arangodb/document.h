#ifndef ARANGO_DOCUMENT_H
#define ARANGO_DOCUMENT_H

#include <QObject>
#include <QStringList>
#include <QJsonObject>
#include "arangodb_global.h"
#include "arangodata.h"

namespace Arango {

class Collection;
class DocumentPrivate;

class ARANGODBSHARED_EXPORT Document : public QObject
{
    Q_OBJECT
public:
    Document(); // empty Document
    Document(Collection *parent); // empty Document within Collection
    Document(Collection *parent, const QJsonObject& o);
    Document(const QJsonObject& o);
    void setParent(Collection *coll);
//    Document(Collection *parent, const QJsonObject& data);
//    Document(Collection *parent, const QString& key, const QString& rev = QString());
    ~Document();
    QString generateUid();
//    bool sync();
//    void registerWithCollection(Collection *c);
    bool isDirty();
    QJsonValue value(const QString& key) const;
    QStringList keys() const;
    void setValue(const QString& key, const QVariant& value);
    void remove(const QString& key);
    QJsonValueRef operator[] (const QString& key);
    QJsonValue operator [](const QString& key) const;
    QJsonObject jsonObject();
    bool operator== (const Document& other) const;
    int count() const;
    int size() const;
    QString id() const;
    bool contains(const QString &key) const;
    QString revision() const;
    bool hasValidRevision();
//    QString handle() const;
    QString key() const;
    QStringList changedAttributes();
    QStringList watchedAttributes() const;
    void setClean();
    bool isEmpty() const;
    void removeNulls();
    bool needsReplace() const;
//    bool revisionIsValid();
signals:
    void documentChanged(const QString& key);
public slots:
private:
    DocumentPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Document)
};

MK_SHR_PTR(Document);

} // namespace Arango

#endif // ARANGO_DOCUMENT_H
