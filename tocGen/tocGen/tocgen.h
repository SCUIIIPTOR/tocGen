/*!
*\file
*\brief Данный  файл содержит в себе заголовки функций, которые используются в ходе работы программы tocGen.
*/

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <stdio.h>
using namespace std;

/*!
* \brief Перечисление типов тега в HTML-коде
*/
enum TagType
{
    OPEN_TAG = 0,
    CLOSE_TAG = 1,
    UNKNOWN_TAG = 2
};

/*!
* \brief Структура для хранения информации об найденном теге заголовка
* \param startPos - начальная позиция тега в HTML-коде
* \param endPos - конечная позиция тега в HTML-коде
* \param level - уровень тега
* \param type - тип тега
*/
struct HeaderTag
{
    int startPos;
    int endPos;
    int level;
    TagType type;

    //! Оператор перегрузки '==', для сравнения двух HeaderTag (используется при тестировании)
    bool operator == (const HeaderTag& other) const
    {
        return
            startPos == other.startPos &&
            endPos == other.endPos &&
            level == other.level &&
            type == other.type;
    }
};

/*!
* \brief Структура для хранения информации об найденном заголовке
* \param level - уровень заголовка
* \param content - содержимое заголовка
*/
struct Header
{
    int level;
    QString content;

    //! Оператор перегрузки '==', для сравнения двух Header (используется при тестировании)
    bool operator == (const Header& other) const
    {
        return
            level == other.level &&
            content == other.content;
    }
};

/*!
* \brief Выгрузить содержимое файла
* \param[in] argsCounter - количество аргументов, переданных с запуском программы
* \param[in] inputArgs - аргументы, переданные с запуском программы
* \param[in,out] fileData - строка с содержимым файла
* \throw выбрасывает исключения в случае возникновения ошибки
*/
void uploadDataFromFile (const int argsCounter, char* inputArgs[], QString& fileData);

/*!
* \brief Найти h заголовки в HTML-коде
* \param[in] htmlCode - HTML-код страницы, хранящийся в виде строки
* \param[in,out] headersInfo - контейнер с найденными заголовками
* \throw выбрасывает исключения в случае возникновения ошибки
*/
void findHeaders(const QString& htmlCode, QList<Header>& headersInfo);

/*!
* \brief Найти закомментированые теги h заголовков в HTML-коде
* \param[in] htmlCode - HTML-код страницы
* \param[in,out] commentedHeaderTagsInfo - контейнер с позициями закомментированных тегов заголовков и их типов
*/
void findCommentedHeadersTags(const QString& htmlCode, QMap<int, TagType>& commentedHeaderTagsInfo);

/*!
* \brief Найти незакомментированые теги h заголовков в HTML-коде
* \param[in] htmlCode - HTML-код страницы
* \param[in] commentedHeaderTagsInfo - контейнер с позициями закомментированных тегов заголовков и их типов
* \param[in,out] headerTagsInfo - контейнер с незакомментироваными тегами заголовков
*/
void findUncommentedHeadersTags(const QString& htmlCode, const QMap<int, TagType>& commentedHeaderTagsInfo,  QList<HeaderTag>& headerTagsInfo);

/*!
* \brief Найти пропущенные теги среди найденных
* \param[in] headerTagsInfo - контейнер с незакомментироваными тегами заголовков
* \throw выбрасывает исключения в случае возникновения ошибки
*/
void checkMissingTags(const QList<HeaderTag>& headerTagsInfo);

/*!
* \brief Найти вложенные в друг друга заголовки
* \param[in] headerTagsInfo - контейнер с незакомментироваными тегами заголовков (все заголовки - корректные)
* \throw выбрасывает исключения в случае возникновения ошибки
*/
void checkHeadersForNesting(const QList<HeaderTag>& headerTagsInfo);
