#ifndef ARANGO_COLLECTION_H
#define ARANGO_COLLECTION_H

#include <QObject>

namespace Arango {

class Collection : public QObject
{
    Q_OBJECT
public:
    explicit Collection(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_COLLECTION_H
