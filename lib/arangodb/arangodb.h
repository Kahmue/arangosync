#ifndef ARANGO_ARANGODB_H
#define ARANGO_ARANGODB_H

#include <QObject>

namespace Arango {

class ArangoDB : public QObject
{
    Q_OBJECT
public:
    explicit ArangoDB(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_ARANGODB_H
