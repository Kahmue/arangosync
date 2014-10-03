#ifndef ARANGODATA_H
#define ARANGODATA_H

#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QSharedPointer>

#include "net.h"

#define MK_SHR_PTR(Class) typedef QSharedPointer<Class> Class##Ptr

namespace Arango {

typedef struct USERINFO_STR {
    QString username;
    QString password;
    bool activeUser;
    QJsonObject extra;
    void fromJson(const QJsonObject& o);
    QJsonObject toJson();
} UserDescriptor;

typedef QVector<UserDescriptor> UserVector;

MK_SHR_PTR(UserDescriptor);

typedef struct DBINFO_STR {
    QString name;
    QString path;
    QString id;
    bool isSystem = false;
} DBDescriptor;

MK_SHR_PTR(DBDescriptor);

enum class KeyType { Undefined, Traditional, Autoincrement };
enum class CollectionType { Documents = 2, Edges };
enum class CollectionStatus { Newborn = 1, Unloaded, Loaded, Unloading, Deleted };
enum class ChecksumMode { keysOnly, withRevisions, withData };
enum class MatchPolicy { None, IfMatch, IfNotMatch };
enum class AttributeState { None, Changed, Removed };

typedef struct KEYOPT_STR {
    KeyType type = KeyType::Undefined;
    bool allowUserKeys = false;
    int increment = 1;
    int offset = 0;
    QJsonObject toJson();
    void fromJson(const QJsonObject& o);
} KeyOptions;

typedef struct COLL_INFO {
    QString name;
    QString id;
    bool isSystem;
    CollectionStatus status;
    CollectionType type;
    bool isLoaded;
} CollectionInfo;

MK_SHR_PTR(CollectionInfo);

typedef QVector<CollectionInfo *> CollectionInfoPtrVector;

typedef struct COLINFO_STR {
    QString name;
    QString id;
    bool waitForSync = false;
    bool doCompact = false;
    bool isVolatile = false;
    int journalSize = -1;
    KeyOptions keyoptions;
    CollectionType type = CollectionType::Documents;
    CollectionStatus status;
    QJsonObject toJson();
    void fromJson(const QJsonObject& o);
} CollectionDescriptor;

MK_SHR_PTR(CollectionDescriptor);

typedef struct DOCINFO_STR {
    QString key;
    QString rev;
    QString id;
    bool valid;
} DocumentDescriptor;

MK_SHR_PTR(DocumentDescriptor);

typedef QMap<QString, DocumentDescriptor> DocumentDescriptorMap;

typedef QMap<QString, QString> StringMap;

inline QString bool2string(bool b) { return b ? QString("true") : QString("false"); }
template<class T>
bool isOneOf(const T& s, QVector<T> data)
{
    for (T t : data) {
        if (t == s) return true;
    }
    return false;
}
 }

#endif // ARANGODATA_H
