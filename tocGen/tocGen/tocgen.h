/*!
*\file
*\brief Данный  файл содержит в себе заголовки функций, которые используются в ходе работы программы tocGen.
*/

#include <QCoreApplication>
#include <QFile>
#include <QRegularExpression>
#include <stdio.h>
using namespace std;

static QRegularExpression headerTags("(?<!<)<h([1-6])[^>]*>(?!>)|(?<!<)</h([1-6])[^>]*>(?!>)");
static QRegularExpression comments("<!--\\s*(.*?)\\s*-->");

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
