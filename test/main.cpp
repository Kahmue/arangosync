#include <QCoreApplication>
#include <QtCore>
#include <QtDebug>
#include "net.h"
#include "arango.h"
#include "arangodata.h"

using namespace Arango;

void test_Document()
{
    Document empty;

    Document doc1, doc2;
    doc1["lastname"] = u8"Müllenholz";
    doc1["firstname"] = u8"Kristian";
    doc1["fileas"] = u8"Müllenholz, Kristian";
    doc1["age"] = 42;

    doc2["lastname"] = u8"Müllenholz";
    doc2["firstname"] = u8"Kristian";
    doc2["fileas"] = u8"Müllenholz, Kristian";
    doc2["age"] = 42;

    qDebug() << "equal" << (doc1 == doc2) << "empty" << empty.isEmpty();


    doc1["age"] = QJsonValue::Null;
    qDebug() << "doc1" << doc1.jsonObject();
    doc1.removeNulls();
    qDebug() << "doc1" << doc1.jsonObject();
}

void printCollectionInfo(CollectionInfo *info)
{
    qDebug() << "Name" << info->name;
    qDebug() << "isSystem" << info->isSystem;
    qDebug() << "isLoaded" << info->isLoaded;
    qDebug() << "id" << info->id;
    qDebug() << "status" << (int)info->status;
    qDebug() << "type" << (int)info->type;
}

void create_and_drop_test()
{
    qDebug() << "CREATE" << ArangoDB::createDB("https://localhost:11235", "testdb");

    CollectionDescriptor cold;
    cold.name = "testcol";
    cold.keyoptions.allowUserKeys = true;

    ArangoDB db_test("https://localhost:11235", "testdb");
    qDebug() << "create collection" << db_test.createCollection(cold);
    CollectionInfoPtrVector cipv_test = db_test.collectionInfo();
    for(CollectionInfo *p : cipv_test)
        printCollectionInfo(p);

    qDebug() << "DROP" << ArangoDB::dropDB("https://localhost:11235", "testdb");
}

void print_collectioninfo(const QJsonObject& o)
{
    qDebug() << o["key"].toString() << "$$" << o["rev"].toString() << "$$" << o["id"].toString();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Net::setSslErrorFlags(Net::Ignore | Net::Log);

    ArangoDB db("https://localhost:11235", "hvdb");

    DBDescriptor *dbd = db.descriptor();
    if (dbd == nullptr)
        qDebug() << "loadDBInfo failed";

    qDebug() << "DB Name" << dbd->name;
    qDebug() << "DB ID" << dbd->id;
    qDebug() << "DB Path" << dbd->path;
    qDebug() << "DB isSystem" << dbd->isSystem;
    qDebug() << "Collections" << db.collections();


    Collection *col = db["persons"];
    Document d;
    d["lastname"] = "Dumbledore";
    d["firstname"] = "Albus";
    d["gender"] = "M";
    d["profession"] = "Zauberer";

    qDebug() << "REV1" << col->revision();

    Document *ds = col->insert(&d);
    if (ds == nullptr)
        qDebug() << "SDOC failed";
    else
        qDebug() << "SDOC" << ds->jsonObject();

    (*ds)["age"] = 158;

    qDebug() << "Update" << col->update(ds);
    qDebug() << ds->jsonObject();
    ds->remove("age");
    qDebug() << ">>>>remove";
    qDebug() << ds->jsonObject();
    qDebug() << "Replace" << col->update(ds);

    return 0;
}
