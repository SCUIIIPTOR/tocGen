/*!
*\file
*
*\brief Данный файл содержит главную функцию программы
*
*\mainpage Документация для программы "Table Of Contents Generator (TOCGen)"
Программа предназначена для автоматической генерации оглавления HTML-страницы.
Для функционирования программы необходима операционная система Windows 7 или выше.
Программа разработана на языке С++ с использованием стандартных библиотек C++, библиотеки Qt и фреймворка Qt Creator.
Стандарт языка ISO C++17 (/std:c++17).
Программа должна получать один аргумент командной строки: имя входного файла в формате 'html'.

Пример команды запуска программы:
*\code
tocGen.exe "C:\input file.html" или tocGen.exe ./input.html
*\endcode

*\author Visocky Oleg
*\date May 2023
*\version 1.0
*/

#include <tocgen.h>

/*!
*\brief Главная функция программы
*\param[in] argv[0]- аргумент запуска программы
*\param[in] argv[1] - аргумент с путем к файлу с входными данными
*\return 0 - запуск программы прошел успешно
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Включить поддержку русских символов в консоли
    setlocale(LC_ALL, "Russian");

    // Объявление переменных
    QString tableOfContents;
    QString fileData;
    QList<Header> headers;

    try
    {
        // Выгрузить содержимое файла в строку
        uploadDataFromFile(argc, argv, fileData);

        // Найти заголовки
        findHeaders(fileData, headers);

        // Сгенерировать оглавление
        generateTableOfContents(headers, tableOfContents);

        // Сохранить данные в файл
        saveDataToFile(tableOfContents);

        // Вывод сообщения о завершении работы программы
        qDebug() << "Работа программы завершена!";
    }
    catch (QString exception)
    {
        // Вывод ошибки
        qDebug() << exception;
    }

    return a.exec();
}
