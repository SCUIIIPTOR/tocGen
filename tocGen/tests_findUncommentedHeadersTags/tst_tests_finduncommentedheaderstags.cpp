#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_findUncommentedHeadersTags : public QObject
{
    Q_OBJECT


private slots:
    void commentedCloseHeaderTag();
    void commentedOpenHeaderTag();
    void commentedBothHeaderTags();
    void commentedFullHeader();
    void commentedCloseNestedHeaderTag();
    void commentedOpenNestedHeaderTag();
    void commentedNestedHeaderTags();
    void noCommentedTagsWithNestedHeader();
    void noCommentedTagsWithoutNestedHeader();
    void commentedTagsWithDifferentsLevels();
    void commentedTagsOnDifferentLines();
    void uncommentedTagsOnDifferentLines();
};

void tests_findUncommentedHeadersTags::commentedCloseHeaderTag()
{
    QString htmlCode = "<h1>This is my first header!<!--</h1>-->";
    QMap<int, TagType> commentedHeaderTagsInfo = {{37, CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1, 4, 1, OPEN_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedOpenHeaderTag()
{
    QString htmlCode = "<!--<h1>-->This is my first header!</h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {{5, OPEN_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{36, 40, 1, CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedBothHeaderTags()
{
    QString htmlCode = "<!--<h1>-->This is my first header!<!--</h1>-->";
    QMap<int, TagType> commentedHeaderTagsInfo = {{5, OPEN_TAG}, {44, CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedFullHeader()
{
    QString htmlCode = "<!--<h1>This is my first header!</h1>-->";
    QMap<int, TagType> commentedHeaderTagsInfo = {{5, OPEN_TAG}, {37, CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedCloseNestedHeaderTag()
{
    QString htmlCode = "<h1>This is my first header! <h1> THIS IS NESTED HEADER! <!--</h1>--> </h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {{66,CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1,4,1,OPEN_TAG}, {30,33,1,OPEN_TAG}, {71,75,1,CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedOpenNestedHeaderTag()
{
    QString htmlCode = "<h1>This is my first header! <!--<h1>--> THIS IS NESTED HEADER! </h1> </h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {{34,OPEN_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1,4,1,OPEN_TAG}, {65,69,1,CLOSE_TAG}, {71,75,1,CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedNestedHeaderTags()
{
    QString htmlCode = "<h1>This is my first header! <!--<h1>--> THIS IS NESTED HEADER! <!--</h1>--> </h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {{34,OPEN_TAG}, {73,CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1,4,1,OPEN_TAG}, {78,82,1,CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::noCommentedTagsWithNestedHeader()
{
    QString htmlCode = "<h1>This is my first header!<h2>THIS IS NESTED HEADER!</h2></h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1,4,1,OPEN_TAG}, {29,32,2,OPEN_TAG}, {55,59,2,CLOSE_TAG}, {60,64,1,CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::noCommentedTagsWithoutNestedHeader()
{
    QString htmlCode = "<h1>This is my first header!</h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1,4,1,OPEN_TAG}, {29,33,1,CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedTagsWithDifferentsLevels()
{
    QString htmlCode = "<!--<h1>-->This is my first header!<!--</h1>--> <!--<h2>-->This is my second header!<!--</h2>--> <!--<h3>This is my third header!</h3>-->";
    QMap<int, TagType> commentedHeaderTagsInfo = {{5, OPEN_TAG}, {44, CLOSE_TAG}, {53, OPEN_TAG}, {93, CLOSE_TAG}, {102, OPEN_TAG}, {134, CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::commentedTagsOnDifferentLines()
{
    QString htmlCode = "<!--<h1>-->\n<h1>\nThis is my first header!\n<!--</h1>-->";
    QMap<int, TagType> commentedHeaderTagsInfo = {{5, OPEN_TAG}, {51, CLOSE_TAG}};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{13, 16, 1, OPEN_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

void tests_findUncommentedHeadersTags::uncommentedTagsOnDifferentLines()
{
    QString htmlCode = "<h1>\nThis is my first header!\n</h1>";
    QMap<int, TagType> commentedHeaderTagsInfo = {};
    QList<HeaderTag> headerTagsInfo;
    QList<HeaderTag> expectedHeaderTagsInfo = {{1, 4, 1, OPEN_TAG}, {31, 35, 1, CLOSE_TAG}};

    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    QCOMPARE(headerTagsInfo, expectedHeaderTagsInfo);
}

QTEST_APPLESS_MAIN(tests_findUncommentedHeadersTags)

#include "tst_tests_finduncommentedheaderstags.moc"
