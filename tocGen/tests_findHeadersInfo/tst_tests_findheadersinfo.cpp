#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_findHeadersInfo : public QObject
{
    Q_OBJECT

private slots:
    void oneHeader();
    void fewHeadersWithSameLevel();
    void noHeaders();
    void commentedHeadersAreAvailable();
    void complexTest();
};

void tests_findHeadersInfo::oneHeader()
{
    QString htmlCode = "<html><h1>First Header</h1></html>";
    QList<Header> headers;
    QList<HeaderTag> headerTagsInfo = {{7,10,1,OPEN_TAG}, {23,27,1,CLOSE_TAG}};
    QList<Header> expectedHeaders = {{1, "First Header"}};

    findHeadersInfo(htmlCode, headerTagsInfo, headers);

    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeadersInfo::fewHeadersWithSameLevel()
{
    QString htmlCode = "<html><h1>First Header</h1><h1><strong>Second Header</strong></h1><h1>Third Header</h1></html>";
    QList<Header> headers;
    QList<HeaderTag> headerTagsInfo = {{7,10,1,OPEN_TAG}, {23,27,1,CLOSE_TAG}, {28,31,1,OPEN_TAG}, {62,66,1,CLOSE_TAG}, {67,70,1,OPEN_TAG}, {83,87,1,CLOSE_TAG}};
    QList<Header> expectedHeaders = {{1, "First Header"}, {1, "<strong>Second Header</strong>"}, {1, "Third Header"}};

    findHeadersInfo(htmlCode, headerTagsInfo, headers);

    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeadersInfo::noHeaders()
{
    QString htmlCode = "<html><p>This is my first paragraph!</p></html>";
    QList<Header> headers = {};
    QList<HeaderTag> headerTagsInfo = {};
    QList<Header> expectedHeaders = {};

    findHeadersInfo(htmlCode, headerTagsInfo, headers);

    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeadersInfo::commentedHeadersAreAvailable()
{
    QString htmlCode = "<html><h1>Header</h1><!--<h2>First commented Header</h2>--><!--<h3>Second commented Header</h3>--></html>";
    QList<Header> headers = {};
    QList<HeaderTag> headerTagsInfo = {{7,10,1,OPEN_TAG}, {17,21,1,CLOSE_TAG}};
    QList<Header> expectedHeaders = {{1, "Header"}};

    findHeadersInfo(htmlCode, headerTagsInfo, headers);

    QCOMPARE(headers, expectedHeaders);
}

void tests_findHeadersInfo::complexTest()
{
    QString htmlCode = "<html><h1>First Header</h1><h2><strong>Second Header</strong></h2><h3>Third Header</h3><h4>Fourth Header</h4><h5>Fifth Header</h5><h6>Sixth Header</h6><h3>Another Third Header</h3><!--<h1>Commented Header</h1>--></html>";
    QList<Header> headers;
    QList<HeaderTag> headerTagsInfo = {{7,10,1,OPEN_TAG}, {23,27,1,CLOSE_TAG}, {28,31,2,OPEN_TAG}, {62,66,2,CLOSE_TAG}, {67,70,3,OPEN_TAG}, {83,87,3,CLOSE_TAG}, {88,91,4,OPEN_TAG}, {105,109,4,CLOSE_TAG}, {110,113,5,OPEN_TAG}, {126,130,5,CLOSE_TAG}, {131,134,6,OPEN_TAG}, {147,151,6,CLOSE_TAG}, {152,155,3,OPEN_TAG}, {176,180,3,CLOSE_TAG}};
    QList<Header> expectedHeaders = {{1, "First Header"}, {2, "<strong>Second Header</strong>"}, {3, "Third Header"}, {4, "Fourth Header"}, {5, "Fifth Header"}, {6, "Sixth Header"}, {3, "Another Third Header"}};

    findHeadersInfo(htmlCode, headerTagsInfo, headers);

    QCOMPARE(headers, expectedHeaders);
}

QTEST_APPLESS_MAIN(tests_findHeadersInfo)

#include "tst_tests_findheadersinfo.moc"
