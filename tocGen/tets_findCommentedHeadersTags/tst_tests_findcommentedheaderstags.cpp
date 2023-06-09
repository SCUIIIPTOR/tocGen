#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_findCommentedHeadersTags : public QObject
{
    Q_OBJECT

private slots:
    void commentedOpeningHeaderTagWithoutClosingTag();
    void commentedClosingHeaderTagWithoutOpeningTag();
    void commentedFewHeadersTagsWithSameLevel();
    void commentedCorrectHeaderAndHeaderWithoutClosingTagWithSameLevel();
    void commentedCorrectHeaderAndHeaderWithoutOpeningTagWithSameLevel();
    void commentedHeaderWithNestedHeaderWithoutOpeningTag();
    void commentedHeaderWithNestedHeaderWithoutClosingTag();
    void commentedNestedHeaderInHeader();
    void commentedHeaderWithFewNestedHeadersTags();
    void noCommentedHeadersTags();
    void commentedHeaderOpenTagWithoutContent();
    void commentedHeaderCloseTagWithoutContent();
    void commentedTagsOnDifferentLines();
    void uncommentedTagsOnDifferentLines();
    void commentedHeaderAndCorrectHeaderOnDifferentsLines();
    void complexTest();
};

void tests_findCommentedHeadersTags::commentedOpeningHeaderTagWithoutClosingTag()
{
    QString htmlCode = "<!-- <h1>Header without closing tag -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(6, OPEN_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedClosingHeaderTagWithoutOpeningTag()
{
    QString htmlCode = "<!-- Header without opening tag</h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(36, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedFewHeadersTagsWithSameLevel()
{
    QString htmlCode = "<!-- <h1></h1><h1></h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(6, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(15, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(14, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(23, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedCorrectHeaderAndHeaderWithoutClosingTagWithSameLevel()
{
    QString htmlCode = "<!-- <h1>Header<h1>Another Header</h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(6, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(16, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(38, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedCorrectHeaderAndHeaderWithoutOpeningTagWithSameLevel()
{
    QString htmlCode = "<!-- Header</h1><h1>Another Header</h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(17, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(16, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(39, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedHeaderWithNestedHeaderWithoutClosingTag()
{
    QString htmlCode = "<!-- <h1>H1<h2>H2</h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(6, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(12, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(22, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedHeaderWithNestedHeaderWithoutOpeningTag()
{
    QString htmlCode = "<!-- <h1>H1 H2</h2></h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(6, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(19, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(24, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedNestedHeaderInHeader()
{
    QString htmlCode = "<h1>H1<!--<h2>H2</h2>--></h1>";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(11, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(21, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedHeaderWithFewNestedHeadersTags()
{
    QString htmlCode = "<!-- <h1><h2><h3></h3></h2></h1> -->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(6, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(10, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(14, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(22, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(27, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(32, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::noCommentedHeadersTags()
{
    QString htmlCode = "<h1></h1><h2></h2><h3></h3>";
    QMap<int, TagType> commentedHeaderTagsInfo = {};
    QMap<int, TagType> expectedCommentedHeaderTagsInfo = {};

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedHeaderOpenTagWithoutContent()
{
    QString htmlCode = "<!--<h1>-->Header without closing tag</h1>";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(5, OPEN_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedHeaderCloseTagWithoutContent()
{
    QString htmlCode = "<h1>Header without closing tag<!--</h1>-->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(39, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedTagsOnDifferentLines()
{
    QString htmlCode = "<!--<h1>-->\nHeader without closing tag\n<!--</h1>-->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(5, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(48, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::uncommentedTagsOnDifferentLines()
{
    QString htmlCode = "<h1>\nHeader without closing tag\n</h1>";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo = {};

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::commentedHeaderAndCorrectHeaderOnDifferentsLines()
{
    QString htmlCode = "<h1>\nHeader\n<!--<h1>\nHeader commented\n</h1>-->\n</h1></h1>";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo = {{17, OPEN_TAG}, {43, CLOSE_TAG}};

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

void tests_findCommentedHeadersTags::complexTest()
{
    QString htmlCode = "<!--<h1>-->First Header</h1><h2>Second Header<!--</h2>--><!--<h3>-->Third Header<!--</h3>--><!--<h4>Four Header</h4>-->";
    QMap<int, TagType> commentedHeaderTagsInfo;
    QMap<int, TagType> expectedCommentedHeaderTagsInfo;
    expectedCommentedHeaderTagsInfo.insert(5, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(54, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(62, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(89, CLOSE_TAG);
    expectedCommentedHeaderTagsInfo.insert(97, OPEN_TAG);
    expectedCommentedHeaderTagsInfo.insert(116, CLOSE_TAG);

    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    QCOMPARE(commentedHeaderTagsInfo, expectedCommentedHeaderTagsInfo);
}

QTEST_APPLESS_MAIN(tests_findCommentedHeadersTags)

#include "tst_tests_findcommentedheaderstags.moc"
