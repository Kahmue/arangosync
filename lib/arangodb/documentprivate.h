#ifndef ARANGO_DOCUMENTPRIVATE_H
#define ARANGO_DOCUMENTPRIVATE_H

#include <QObject>

namespace Arango {

class DocumentPrivate : public QObject
{
    Q_OBJECT
public:
    explicit DocumentPrivate(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_DOCUMENTPRIVATE_H
