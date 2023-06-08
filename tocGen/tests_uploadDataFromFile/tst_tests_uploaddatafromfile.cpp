#include <QtTest>
#include <../tocGen/tocgen.h>

class tests_uploadDataFromFile : public QObject
{
    Q_OBJECT

private slots:
    void numberOfInputArgsIsOne();
    void numberIfInputArgsIsMoreThanTwo();
    void noAccessToFile();
    void accessToFileIsAvailable();
    void fileIsNotAvailable();
    void wrongFormatFile();
};

void tests_uploadDataFromFile::numberOfInputArgsIsOne()
{
    int argsCounter = 1;
    char* inputArgs[1];
    QString fileData = "";

    try
    {
        uploadDataFromFile(argsCounter, inputArgs, fileData);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Аргумент с входными данными не был передан с запуском программы");
    }

    QCOMPARE(fileData, "");
}

void tests_uploadDataFromFile::numberIfInputArgsIsMoreThanTwo()
{
    int argsCounter = 3;
    char* inputArgs[3];
    QString fileData = "";

    try
    {
        uploadDataFromFile(argsCounter, inputArgs, fileData);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Количество аргументов, переданных с запуском программы, не соответствует требованиям ПО");
    }

    QCOMPARE(fileData, "");
}

void tests_uploadDataFromFile::noAccessToFile()
{
    int argsCounter = 2;
    char* inputArgs[2];
    char inputPath[100] = "../../TESTS/uploadDataFromFile/noAccess.html";
    inputArgs[1] = inputPath;
    QString fileData = "";

    try
    {
        uploadDataFromFile(argsCounter, inputArgs, fileData);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "К файлу по пути '../../TESTS/uploadDataFromFile/noAccess.html' отсутствует доступ (возможно, недостаточно прав)");
    }

    QCOMPARE(fileData, "");
}

void tests_uploadDataFromFile::fileIsNotAvailable()
{
    int argsCounter = 2;
    char* inputArgs[2];
    char inputPath[100] = "../../TESTS/uploadDataFromFile/noFile.html";
    inputArgs[1] = inputPath;
    QString fileData = "";

    try
    {
        uploadDataFromFile(argsCounter, inputArgs, fileData);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Файла по пути '../../TESTS/uploadDataFromFile/noFile.html' не существует");
    }

    QCOMPARE(fileData, "");
}

void tests_uploadDataFromFile::wrongFormatFile()
{
    int argsCounter = 2;
    char* inputArgs[2];
    char inputPath[100] = "../../TESTS/uploadDataFromFile/wrongFormat.txt";
    inputArgs[1] = inputPath;
    QString fileData = "";

    try
    {
        uploadDataFromFile(argsCounter, inputArgs, fileData);
    }
    catch (QString exception)
    {
        QCOMPARE(exception, "Формат входного файла по пути '../../TESTS/uploadDataFromFile/wrongFormat.txt' не 'html' (требование ПО)");
    }

    QCOMPARE(fileData, "");
}

void tests_uploadDataFromFile::accessToFileIsAvailable()
{
    int argsCounter = 2;
    char* inputArgs[2];
    char inputPath[100] = "../../TESTS/uploadDataFromFile/accessIsAvailable.html";
    inputArgs[1] = inputPath;
    QString fileData;
    QString expectedFileData = "<html><h1>First header in my Code!</h1></html>";

    uploadDataFromFile(argsCounter, inputArgs, fileData);

    QCOMPARE(fileData, expectedFileData);
}

QTEST_APPLESS_MAIN(tests_uploadDataFromFile)

#include "tst_tests_uploaddatafromfile.moc"
