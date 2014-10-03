#ifndef ARANGO_AQLQUERYPRIVATE_H
#define ARANGO_AQLQUERYPRIVATE_H

#include <QObject>

namespace Arango {

class AQLQueryPrivate : public QObject
{
    Q_OBJECT
public:
    explicit AQLQueryPrivate(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_AQLQUERYPRIVATE_H
