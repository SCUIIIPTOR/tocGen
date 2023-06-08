#include "tocgen.h"

void uploadDataFromFile (const int argsCounter, char* inputArgs[], QString& fileData)
{
    // Если с запуском программы был передан один аргумент...
    if(argsCounter <= 1)
    {
        // Выкинуть ошибку: "Аргумент с входными данными не был передан с запуском программы"
        throw QString("Аргумент с входными данными не был передан с запуском программы");
    }

    // Если с запуском программы было передано больше двух аргументов...
    if(argsCounter > 2)
    {
        // Выкинуть ошибку: "Количество аргументов, переданных с запуском программы, не соответствует требованиям ПО"
        throw QString("Количество аргументов, переданных с запуском программы, не соответствует требованиям ПО");
    }

    QFileInfo fileInfo(inputArgs[1]);
    // Если файл по пути, переданном с запуском программы, существует...
    if(fileInfo.exists())
    {
        // Если формат входного файла ".html"...
        if(fileInfo.suffix() != "html")
        {
            // Выкинуть ошибку: "Формат входного файла по пути '*' не 'html' (требование ПО)".
            throw QString("Формат входного файла по пути '" + QString::fromUtf8(inputArgs[1]) + "' не 'html' (требование ПО)");
        }
    }
    // Иначе...
    else
    {
        // Выкинуть ошибку: "Файла по пути '*' не существует".
        throw QString("Файла по пути '" + QString::fromUtf8(inputArgs[1]) + "' не существует");
    }

    QFile file(inputArgs[1]);
    // Если получить доступ к файлу по пути, переданном с запуском программы, не удалось...
    if (!file.open(QIODevice::ReadOnly))
    {
        // Выкинуть ошибку: "К файлу по пути '*' отсутствует доступ (возможно, недостаточно прав)"
        throw QString("К файлу по пути '" + QString::fromUtf8(inputArgs[1]) + "' отсутствует доступ (возможно, недостаточно прав)");
    }

    // Выгрузить данные из файла в fileData
    QByteArray data = file.readAll();
    fileData = QString::fromUtf8(data);

    // Закрыть доступ к файлу
    file.close();
}

void findCommentedHeadersTags(const QString& htmlCode, QMap<int, TagType>& commentedHeaderTagsInfo)
{
    QRegularExpressionMatchIterator firstMatchIterator;
    QRegularExpressionMatchIterator secondMatchIterator;
    QRegularExpressionMatch firstMatch;
    QRegularExpressionMatch secondMatch;

    // Найти все комментарии в HTML-коде
    firstMatchIterator = comments.globalMatch(htmlCode);

    // Для каждого найденного комментария...
    while (firstMatchIterator.hasNext())
    {
        firstMatch = firstMatchIterator.next();

        // Найти в текущем комментарии все теги заголовков...
        secondMatchIterator = headerTags.globalMatch(firstMatch.captured());

        // Для каждого найденного тега...
        while (secondMatchIterator.hasNext())
        {
            secondMatch = secondMatchIterator.next();

            // Сохранить позицию и тип найденного тега в контейнер commentedHeaderTagsInfo
            if(secondMatch.captured().contains("</h"))
            {
                commentedHeaderTagsInfo.insert(firstMatch.capturedStart() + secondMatch.capturedEnd(), CLOSE_TAG);
            }
            else
            {
                commentedHeaderTagsInfo.insert(firstMatch.capturedStart() + 1 + secondMatch.capturedStart(), OPEN_TAG);
            }
        }
    }
}

void findUncommentedHeadersTags(const QString& htmlCode, const QMap<int, TagType>& commentedHeaderTagsInfo,  QList<HeaderTag>& headersTags)
{
    // Найти все открывающие h заголовки теги
    QRegularExpressionMatchIterator matchIterator;
    QRegularExpressionMatch match;
    HeaderTag tag;

    // Найти все теги заголовков в HTML-коде
    matchIterator = headerTags.globalMatch(htmlCode);

    // Для каждого найденного тега...
    while (matchIterator.hasNext())
    {
        match = matchIterator.next();

        // Если найденный тег - закрывающий и незакомментированный...
        if(match.captured().startsWith("</h") && !commentedHeaderTagsInfo.contains(match.capturedEnd()))
        {
            // Сохранить информацию о найденном теге в контейнер headerTagsInfo
            tag.startPos = match.capturedStart() + 1;
            tag.endPos = match.capturedEnd();
            tag.level = match.captured(2).toInt();
            tag.type = CLOSE_TAG;
            headersTags.append(tag);
        }

        // Если найденный тег - открывающий и незакомментированный...
        if(match.captured().startsWith("<h") && !commentedHeaderTagsInfo.contains(match.capturedStart() + 1))
        {
            // Сохранить информацию о найденном теге в контейнер headerTagsInfo
            tag.startPos = match.capturedStart() + 1;
            tag.endPos = match.capturedEnd();
            tag.level = match.captured(1).toInt();
            tag.type = OPEN_TAG;
            headersTags.append(tag);
        }
    }
}
