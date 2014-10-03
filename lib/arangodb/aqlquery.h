#ifndef ARANGO_AQLQUERY_H
#define ARANGO_AQLQUERY_H

#include <QObject>

namespace Arango {

class AQLQuery : public QObject
{
    Q_OBJECT
public:
    explicit AQLQuery(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_AQLQUERY_H
