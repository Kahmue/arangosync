#ifndef ARANGO_DOCUMENT_H
#define ARANGO_DOCUMENT_H

#include <QObject>

namespace Arango {

class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = 0);

signals:

public slots:

};

} // namespace Arango

#endif // ARANGO_DOCUMENT_H
