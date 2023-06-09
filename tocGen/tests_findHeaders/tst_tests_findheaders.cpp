#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_findHeaders : public QObject
{
    Q_OBJECT

private slots:
    void noCommentedHeaders();
    void commentedOpenHeaderTag();
    void commentedCloseHeaderTag();
    void commentedHeader();
    void noHeadersInHtmlCode();
    void nestedHeaderIsAvailable();
    void complexTest();
};

void tests_findHeaders::noCommentedHeaders()
{
    QString htmlCode = "<html><h1>This is my first header!</h1></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {{1, "This is my first header!"}};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "");
    }
    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeaders::commentedOpenHeaderTag()
{
    QString htmlCode = "<html><!--<h1>-->This is my first header!</h1></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '42', отсутствует открывающий тег");
    }
    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeaders::commentedCloseHeaderTag()
{
    QString htmlCode = "<html><h1>This is my first header!<!--</h1>--></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '7', отсутствует закрывающий тег");
    }
    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeaders::commentedHeader()
{
    QString htmlCode = "<html><!--<h1>This is my first header!</h1>--><h2>And this is second!</h2></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {{2, "And this is second!"}};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "");
    }
    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeaders::noHeadersInHtmlCode()
{
    QString htmlCode = "<html><!--<h1>This is my first header!</h1>--></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "В HTML-коде отсутствуют незакомментированные теги, объявляющие h заголовки");
    }
    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeaders::nestedHeaderIsAvailable()
{
    QString htmlCode = "<html><h2>And this is second!<h1>Nested header!</h1></h2></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "В заголовке, который начинается с позиции '7' имеется вложенный заголовок");
    }
    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeaders::complexTest()
{
    QString htmlCode = "<html><h1></h1><h2>Second Header</h2><h6><strong>Sixth header</strong></h6></html>";
    QList<Header> headers = {};
    QList<Header> expectedHeaders = {{1,""}, {2, "Second Header"}, {6, "<strong>Sixth header</strong>"}};
    try
    {
        findHeaders(htmlCode, headers);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "");
    }
    QCOMPARE(headers, expectedHeaders);
}

QTEST_APPLESS_MAIN(tests_findHeaders)

#include "tst_tests_findheaders.moc"
