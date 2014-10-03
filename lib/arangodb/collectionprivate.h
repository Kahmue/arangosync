#ifndef ARANGO_COLLECTIONPRIVATE_H
#define ARANGO_COLLECTIONPRIVATE_H

#include <QObject>

namespace Arango {

class CollectionPrivate : public QObject
{
    Q_OBJECT
public:
    explicit CollectionPrivate(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_COLLECTIONPRIVATE_H
