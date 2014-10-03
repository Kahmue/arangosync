#include "aqlqueryprivate.h"
#include "request.h"
#include "arangodb.h"

#include <QtDebug>

namespace Arango {

AQLQueryPrivate::AQLQueryPrivate(AQLQuery *parent) : QObject(parent)
{
    q_ptr = parent;
}

void AQLQueryPrivate::setError(int num, const QString &msg)
{
    _error = msg;
    _error_no = num;
}

void AQLQueryPrivate::clearErrors()
{
    _error = "";
    _error_no = 0;
}

int AQLQueryPrivate::loadBatch()
{
    if (!_loadMore)
        return 0;
    Request req(_cursorurl, Net::PUT, db_ptr->user());
    if (req.sendWithResponseCode({200})) {
        QJsonObject o = req.dataToJsonObject();
        _loadMore = o["hasMore"].toBool();
        _counter = o["count"].toInt();
        for (QJsonValue v : o["result"].toArray())
            _data.append(v);
        return o["result"].toArray().count();
    }
    return 0;
}

bool AQLQueryPrivate::checkBoundaries(int pos) const
{
    return pos >= 0 && pos < _counter;
}

} // namespace Arango
