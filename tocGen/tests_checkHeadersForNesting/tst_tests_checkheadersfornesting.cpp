#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_checkHeadersForNesting : public QObject
{
    Q_OBJECT

private slots:
    void nestedHeaderIsAvailable();
    void noNestedHeader();
};

void tests_checkHeadersForNesting::nestedHeaderIsAvailable()
{
    // <h1><h2></h2></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,8,2,OPEN_TAG}, {9,13,2,CLOSE_TAG}, {14,19,1,CLOSE_TAG}};

    try
    {
        checkHeadersForNesting(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "В заголовке, который начинается с позиции '1' имеется вложенный заголовок");
    }
}

void tests_checkHeadersForNesting::noNestedHeader()
{
    // <h1></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,9,1,CLOSE_TAG}};

    try
    {
        checkHeadersForNesting(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "");
    }
}

QTEST_APPLESS_MAIN(tests_checkHeadersForNesting)

#include "tst_tests_checkheadersfornesting.moc"
