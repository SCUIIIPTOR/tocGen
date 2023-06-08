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


