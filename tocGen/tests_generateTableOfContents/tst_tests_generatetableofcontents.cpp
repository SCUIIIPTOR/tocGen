#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_generateTableOfContents : public QObject
{
    Q_OBJECT

private slots:
    void headersAreAvailable();
    void noHeaders();
    void allHeadersAreSameLevel();
    void tableOfContentsBeginWithSecondLevelHeader();
};

void tests_generateTableOfContents::headersAreAvailable()
{
    // <html><h1>First header</h1><h2>Second header</h2><h3><strong>Third header</strong></h3><h1>Fourth header</h1></html>

    QString tableOfContents;
    QString expectedTableOfContents = "<ul>\n<li>First header</li>\n<ul>\n<li>Second header</li>\n<ul>\n<li><strong>Third header</strong></li>\n</ul>\n</ul>\n<li>Fourth header</li>\n</ul>\n";
    QList<Header> headers = {{1,"First header"}, {2,"Second header"}, {3,"<strong>Third header</strong>"}, {1, "Fourth header"}};

    generateTableOfContents(headers, tableOfContents);

    QCOMPARE(tableOfContents, expectedTableOfContents);
}

void tests_generateTableOfContents::noHeaders()
{
    // <html><p>This is my first paragraph!</p></html>

    QString tableOfContents = "";
    QString expectedTableOfContents = "";
    QList<Header> headers = {};

    generateTableOfContents(headers, tableOfContents);

    QCOMPARE(tableOfContents, expectedTableOfContents);
}

void tests_generateTableOfContents::allHeadersAreSameLevel()
{
    // <html><h1>First header</h1><h1>Second header</h1></html>

    QString tableOfContents;
    QString expectedTableOfContents = "<ul>\n<li>First header</li>\n<li>Second header</li>\n</ul>\n";
    QList<Header> headers = {{1,"First header"}, {1,"Second header"}};

    generateTableOfContents(headers, tableOfContents);

    QCOMPARE(tableOfContents, expectedTableOfContents);
}

void tests_generateTableOfContents::tableOfContentsBeginWithSecondLevelHeader()
{
    // <html><h2>First header</h2><h1>Second header</h1></html>

    QString tableOfContents;
    QString expectedTableOfContents = "<ul>\n<ul>\n<li>First header</li>\n</ul>\n<li>Second header</li>\n</ul>\n";
    QList<Header> headers = {{2,"First header"}, {1,"Second header"}};

    generateTableOfContents(headers, tableOfContents);

    QCOMPARE(tableOfContents, expectedTableOfContents);
}

QTEST_APPLESS_MAIN(tests_generateTableOfContents)

#include "tst_tests_generatetableofcontents.moc"
