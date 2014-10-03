#ifndef ARANGO_AQLQUERYPRIVATE_H
#define ARANGO_AQLQUERYPRIVATE_H

#include <QObject>
#include "aqlquery.h"

namespace Arango {
class ArangoDB;

class AQLQueryPrivate : public QObject
{
    Q_OBJECT
public:
    explicit AQLQueryPrivate(AQLQuery *parent = 0);
    QString _apiurl;
    QString _cursorurl;
    QString _query;
    int _batchsize = 50;
    bool _count = true;
    ArangoDB *db_ptr;
    AQLQuery *q_ptr;
    Q_DECLARE_PUBLIC(AQLQuery)

    QJsonObject _reqObject;
    bool _loadMore = false;
    QJsonArray _data;
    int _counter = -1;
    QString _cursorid;
    int _pos = -1;
    QString _error;
    int _error_no;
    void setError(int num, const QString& msg);
    void clearErrors();
    int loadBatch();
    bool checkBoundaries(int pos) const;
signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_AQLQUERYPRIVATE_H
