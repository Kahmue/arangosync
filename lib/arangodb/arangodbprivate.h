#ifndef ARANGO_ARANGODBPRIVATE_H
#define ARANGO_ARANGODBPRIVATE_H

#include <QObject>

namespace Arango {

class ArangoDBPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ArangoDBPrivate(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_ARANGODBPRIVATE_H
