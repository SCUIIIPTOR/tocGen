#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_checkMissingTags : public QObject
{
    Q_OBJECT

private slots:
    void noMissingTagsForHeader();
    void missedClosingTag();
    void missedOpeningTag();
    void missedClosingTagForNestedHeaderWithSameLevel();
    void missedOpeningTagForNestedHeaderWithSameLevel();
    void nestedHeaderWithSameLevel();
    void missedClosingTagForNestedHeaderWithDifferentLevels();
    void missedOpeningTagForNestedHeaderWithDifferentLevels();
    void missedClosingTagForHeaderAndCorrectHeader();
    void missedOpeningTagForHeaderAndCorrectHeader();
};

void tests_checkMissingTags::noMissingTagsForHeader()
{
    // <h1></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,9,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "");
    }
}

void tests_checkMissingTags::missedClosingTag()
{
    // <h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '1', отсутствует закрывающий тег");
    }
}

void tests_checkMissingTags::missedOpeningTag()
{
    // </h1>

    QList<HeaderTag> headerTagsInfo = {{1,5,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '1', отсутствует открывающий тег");
    }
}

void tests_checkMissingTags::missedClosingTagForNestedHeaderWithSameLevel()
{
    // <h1><h1></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,8,1,OPEN_TAG}, {9,13,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '5', отсутствует закрывающий тег");
    }
}

void tests_checkMissingTags::missedOpeningTagForNestedHeaderWithSameLevel()
{
    // <h1></h1></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,9,1,CLOSE_TAG}, {10,14,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '10', отсутствует открывающий тег");
    }
}

void tests_checkMissingTags::nestedHeaderWithSameLevel()
{
    // <h1><h1></h1></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,8,1,OPEN_TAG}, {9,13,1,CLOSE_TAG}, {14,18,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "");
    }
}

void tests_checkMissingTags::missedClosingTagForNestedHeaderWithDifferentLevels()
{
    // <h1><h2></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,8,2,OPEN_TAG}, {9,13,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '5', отсутствует закрывающий тег");
    }
}

void tests_checkMissingTags::missedOpeningTagForNestedHeaderWithDifferentLevels()
{
    // <h1></h2></h1>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,9,2,OPEN_TAG}, {10,14,1,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '5', отсутствует закрывающий тег");
    }
}

void tests_checkMissingTags::missedClosingTagForHeaderAndCorrectHeader()
{
    // <h1></h1><h6>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,9,1,CLOSE_TAG}, {10,13,6,OPEN_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '10', отсутствует закрывающий тег");
    }
}

void tests_checkMissingTags::missedOpeningTagForHeaderAndCorrectHeader()
{
    // <h1></h1><h6>

    QList<HeaderTag> headerTagsInfo = {{1,4,1,OPEN_TAG}, {5,9,1,CLOSE_TAG}, {10,14,6,CLOSE_TAG}};

    try
    {
        checkMissingTags(headerTagsInfo);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Для тега, который начинается на позиции '10', отсутствует открывающий тег");
    }
}

QTEST_APPLESS_MAIN(tests_checkMissingTags)

#include "tst_tests_checkmissingtags.moc"
