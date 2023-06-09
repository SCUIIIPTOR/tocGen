/*!
*\file
*\brief Данный  файл содержит в себе реализацию функций, которые используются в ходе работы программы tocGen.
*/

#include "tocgen.h"

QRegularExpression headerTags("(?<!<)<h([1-6])[^>]*>(?!>)|(?<!<)</h([1-6])[^>]*>(?!>)");
QRegularExpression comments("<!--\\s*(.*?)\\s*-->", QRegularExpression::DotMatchesEverythingOption);

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

        // Найти в текущем комментарии все теги заголовков
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
    QRegularExpressionMatchIterator matchIterator;
    QRegularExpressionMatch match;
    HeaderTag tag;

    // Найти все теги заголовков в HTML-коде
    matchIterator = headerTags.globalMatch(htmlCode);

    // Для каждого найденного тега...
    while (matchIterator.hasNext())
    {
        match = matchIterator.next();

        // Если найденный тег - закрывающий и не закомментированный...
        if(match.captured().startsWith("</h") && !commentedHeaderTagsInfo.contains(match.capturedEnd()))
        {
            // Сохранить найденный тег с информацией о нем в контейнер headersTags
            tag.startPos = match.capturedStart() + 1;
            tag.endPos = match.capturedEnd();
            tag.level = match.captured(2).toInt();
            tag.type = CLOSE_TAG;
            headersTags.append(tag);
        }

        // Если найденный тег - открывающий и не закомментированный...
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
    HeaderTag nextTag;

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

        // Если тег из первого цикла еще не обработан...
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
                // Запомнить текущий тег в качестве предыдущего
                nextTag = secondTag;
            }

            // Если тег из первого цикла - закрывающийся...
            if(firstTag.type == CLOSE_TAG)
            {
                // Выкинуть ошибку: "Для тега, который начинается на позиции '*', отсутствует открывающий тег"
                throw QString("Для тега, который начинается на позиции '" + QString::number(firstTag.startPos) + "', отсутствует открывающий тег");
            }

            // Если тег из первого цикла - открывающийся и во втором цикле были рассмотрены все теги из контейнера с найденными тегами...
            if(firstTag.type == OPEN_TAG && headerTagsInfoCopy.last() == nextTag)
            {
                // Выкинуть ошибку: "Для тега, который начинается на позиции '*', отсутствует закрывающий тег"
                throw QString("Для тега, который начинается на позиции '" + QString::number(firstTag.startPos) + "', отсутствует закрывающий тег");
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

    // Для каждого тега из контейнера headerTagsInfo...
    for (QList<HeaderTag>::const_iterator currentTag = headerTagsInfo.begin(); currentTag != headerTagsInfo.end(); ++currentTag)
    {
        // Если текущий тег - открывающий...
        if(currentTag->type == OPEN_TAG)
        {
            // Сохранить уровень и содержимое заголовка в контейнер headers
            header.level = currentTag->level;
            startPos = currentTag->endPos;
            currentTag += 1;
            length = currentTag->startPos - startPos - 1;
            header.content = htmlCode.mid(startPos, length);
            headers.append(header);
        }
    }
}

void findHeaders(const QString& htmlCode, QList<Header>& headers)
{
    QList<HeaderTag> headerTagsInfo;
    QMap<int, TagType> commentedHeaderTagsInfo;

    // Найти все закомментированные теги заголовков в HTML-коде...
    findCommentedHeadersTags(htmlCode, commentedHeaderTagsInfo);

    // Найти все незакомментированные теги заголовков в HTML-коде...
    findUncommentedHeadersTags(htmlCode, commentedHeaderTagsInfo, headerTagsInfo);

    // Если в HTML-коде отсутствуют незакомментированные теги заголовков...
    if(headerTagsInfo.isEmpty())
    {
        // Выкинуть ошибку: "В HTML-коде отсутствуют незакомментированные теги, объявляющие h заголовки"
        throw QString("В HTML-коде отсутствуют незакомментированные теги, объявляющие h заголовки");
    }

    // Проверить, не отсутствует ли какой-либо тег открытия/закрытия для найденных незакомментированных тегов заголовков...
    checkMissingTags(headerTagsInfo);

    // Проверить найденные незакомментированные теги заголовков на вложенность...
    checkHeadersForNesting(headerTagsInfo);

    // Узнать содержимое незакомментированных заголовков...
    findHeadersInfo(htmlCode, headerTagsInfo, headers);
}

void generateTableOfContents(const QList<Header>& headers, QString& tableOfContents)
{
    // Считать, что уровень предыдущего заголовка неизвестен
    int prevHeaderLvl = 0;

    // Для всех найденных незакомментированных h заголовков...
    for (QList<Header>::const_iterator currentHeader = headers.begin(); currentHeader != headers.end(); ++currentHeader)
    {
        // До тех пор, пока уровень текущего заголовка больше, чем уровень предыдущего заголовка...
        for(; prevHeaderLvl < currentHeader->level; prevHeaderLvl++)
        {
            // Добавить открывающий список тег в оглавление
            tableOfContents.append("<ul>\n");
        }

        // До тех пор, пока уровень текущего заголовка меньше, чем уровень предыдущего заголовка...
        for(; prevHeaderLvl > currentHeader->level; prevHeaderLvl--)
        {
            // Добавить закрывающий список тег в оглавление
            tableOfContents.append("</ul>\n");
        }

        // Добавить содержимое текущего заголовка в оглавление
        tableOfContents.append("<li>" + currentHeader->content.simplified() + "</li>\n");
    }

    // До тех пор, пока имеются незакрытые списки...
    for(; prevHeaderLvl > 0; prevHeaderLvl--)
    {
        // Добавить закрывающий список тег в оглавление
        tableOfContents.append("</ul>\n");
    }
}

void saveDataToFile(const QString& data)
{
    QString outputPath = "./result.html";
    QFile outputFile(outputPath);

    // Если доступ к файлу 'result.html' отсутствует...
    if(!outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        // Выбросить ошибку: "Отсутствует доступ к файлу 'result.html', по пути '*'"
        throw QString("Отсутствует доступ к файлу 'result.html', по пути '" + outputFile.fileName() + "'");
    }

    // Сохранить данные в файл
    QTextStream out(&outputFile);
    out << data;

    // Закрыть доступ к файлу
    outputFile.close();
}
