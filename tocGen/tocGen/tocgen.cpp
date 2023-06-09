#include "tocgen.h"

QRegularExpression headerTags("(?<!<)<h([1-6])[^>]*>(?!>)|(?<!<)</h([1-6])[^>]*>(?!>)");
QRegularExpression comments("<!--\\s*(.*?)\\s*-->");

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
            // Сохранить найденный тег с информацией о нем в контейнер headersTags
            tag.startPos = match.capturedStart() + 1;
            tag.endPos = match.capturedEnd();
            tag.level = match.captured(2).toInt();
            tag.type = CLOSE_TAG;
            headersTags.append(tag);
        }

        // Если найденный тег - открывающий и незакомментированный...
        if(match.captured().startsWith("<h") && !commentedHeaderTagsInfo.contains(match.capturedStart() + 1))
        {
            // Сохранить найденный тег с информацией о нем в контейнер headersTags
            tag.startPos = match.capturedStart() + 1;
            tag.endPos = match.capturedEnd();
            tag.level = match.captured(1).toInt();
            tag.type = OPEN_TAG;
            headersTags.append(tag);
        }
    }
}

void checkMissingTags(const QList<HeaderTag>& headerTagsInfo)
{
    // Считать, что пара тегов не найдена
    bool isPairFound = false;

    // Создать копию контейнера headerTagsInfo
    QList<HeaderTag> headerTagsInfoCopy = headerTagsInfo;

    // Если в контейнере с найденными тегами содержится только один тег и этот тег закрывающий...
    if(headerTagsInfoCopy.count() == 1 && headerTagsInfoCopy.at(0).type == CLOSE_TAG)
    {
            // Выкинуть ошибку: "Для тега, который начинается на позиции '*', отсутствует открывающий тег"
            throw QString("Для тега, который начинается на позиции '" + QString::number(headerTagsInfoCopy.at(0).startPos) + "', отсутствует открывающий тег");
    }

    // Если в контейнере с найденными тегами содержится только один тег и этот тег открывающий...
    if(headerTagsInfoCopy.count() == 1 && headerTagsInfoCopy.at(0).type == OPEN_TAG)
    {
            // Выкинуть ошибку: "Для тега, который начинается на позиции '*', отсутствует закрывающий тег"
            throw QString("Для тега, который начинается на позиции '" + QString::number(headerTagsInfoCopy.at(0).startPos) + "', отсутствует закрывающий тег");
    }

    // Для каждого тега из контейнера с найденными тегами начиная с первого (первый цикл)...
    for(QList<HeaderTag>::iterator firstIterator = headerTagsInfoCopy.begin(); firstIterator != headerTagsInfoCopy.end(); ++firstIterator)
    {
        HeaderTag& firstTag = *firstIterator;

        // Считать, что пара тегов не найдена
        isPairFound = false;

        // Если тег из первого цикла еще не обработан
        if(firstTag.type != UNKNOWN_TAG)
        {
            // Для каждого тега из контейнера с найденными тегами, которые следуют после тега из первого цикла, и пока пара тегов не была найдена (второй цикл)...
            for(QList<HeaderTag>::iterator secondIterator = firstIterator + 1; secondIterator != headerTagsInfoCopy.end() && isPairFound != true; ++secondIterator)
            {
                HeaderTag& secondTag = *secondIterator;

                // Если тег из первого цикла - открывающийся...
                if(firstTag.type == OPEN_TAG)
                {
                    // Если тег из второго цикла - закрывающийся и его уровень совпадает с уровнем тега из первого цикла...
                    if(secondTag.type == CLOSE_TAG && firstTag.level == secondTag.level)
                    {
                        // Считать тег из первого цикла и из второго - обработанными
                        firstTag.type = UNKNOWN_TAG;
                        secondTag.type = UNKNOWN_TAG;

                        // Считать, что пара тегов была найдена
                        isPairFound = true;
                    }
                }

                // Если тег из первого цикла - закрывающийся...
                if(firstTag.type == CLOSE_TAG)
                {
                    // Выкинуть ошибку: "Для тега, который начинается на позиции '*', отсутствует открывающий тег"
                    throw QString("Для тега, который начинается на позиции '" + QString::number(firstTag.startPos) + "', отсутствует открывающий тег");
                }

                // Если тег из первого цикла - открывающийся и во втором цикле были рассмотрены все теги из контейнера с найденными тегами...
                if(firstTag.type == OPEN_TAG && headerTagsInfoCopy.last() == secondTag)
                {
                    // Выкинуть ошибку: "Для тега, который начинается на позиции '*', отсутствует закрывающий тег"
                    throw QString("Для тега, который начинается на позиции '" + QString::number(firstTag.startPos) + "', отсутствует закрывающий тег");
                }
            }
        }
    }
}

void checkHeadersForNesting(const QList<HeaderTag>& headerTagsInfo)
{
    HeaderTag prevTag = {-1, -1, -1, UNKNOWN_TAG};

    // Считать, что счетчик открывающих h заголовки тегов обнулен
    int openTagsCounter = 0;

    // Для каждого тега из контейнера с найденными тегами...
    for(const HeaderTag& currentTag : headerTagsInfo)
    {
        // Если текущий тег - открывающий...
        if(currentTag.type == OPEN_TAG)
        {
            // Инкрементировать значение счетчика openTagsCounter
            openTagsCounter++;
        }

        // Если текущий тег - закрывающий, предыдущий тег - открывающий и уровень текущего тега совпадает с уровнем предыдущего тега...
        if(currentTag.type == CLOSE_TAG  && currentTag.level == prevTag.level && prevTag.type == OPEN_TAG)
        {
            // Декрементировать значение счетчика openTagsCounter
            openTagsCounter--;
        }

        // Если значение счетчика открывающих тегов два и более...
        if(openTagsCounter >= 2)
        {
            // Выкинуть ошибку: "В заголовке, который начинается с позиции '*' имеется вложенный заголовок"
            throw QString("В заголовке, который начинается с позиции '" + QString::number(prevTag.startPos) + "' имеется вложенный заголовок");
        }

        // Запомнить текущий тег в качестве предыдущего
        prevTag = currentTag;
    }
}

void findHeadersInfo(const QString& htmlCode, const QList<HeaderTag>& headerTagsInfo, QList<Header>& headers)
{
    int startPos;
    int length;
    Header header;

    for (QList<HeaderTag>::const_iterator currentTag = headerTagsInfo.begin(); currentTag != headerTagsInfo.end(); ++currentTag)
    {
        if(currentTag->type == OPEN_TAG)
        {
            header.level = currentTag->level;
            startPos = currentTag->endPos;
            currentTag += 1;
            length = currentTag->startPos - startPos - 1;
            header.content = htmlCode.mid(startPos, length);
            headers.append(header);
        }
    }
}
