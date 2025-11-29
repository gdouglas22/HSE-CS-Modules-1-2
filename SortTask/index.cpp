#include "lesson.h"
#include <iostream>

using namespace std;

// парсим строку s, начиная с позиции start, длиной length, в целое число
static int ParseInt(const string& s, int start, int length)
{
    int x = 0;
    for (int i = 0; i < length; i++)
    {
        if (start + i >= (int)s.size()) return 0;
        char c = s[start + i];
        if (c < '0' || c > '9') return 0;
        x = x * 10 + (c - '0');
    }
    return x;
}

// вычисляем ключ для индекса по дате и времени
int ComputeDateTimeKey(const Lesson& L)
{
	// ожидаемый формат даты: "DD.MM.YYYY"
    if (L.date.size() < 10 || L.time.size() < 5) return 0;
    int day = ParseInt(L.date, 0, 2);
    int month = ParseInt(L.date, 3, 2);
    int year = ParseInt(L.date, 6, 4);
    int hour = ParseInt(L.time, 0, 2);
    int mins = ParseInt(L.time, 3, 2);
    if (year == 0) return 0;
	// формируем ключ в виде YYYYMMDDHHMM
    int key = year * 100000000 + month * 1000000 + day * 10000 + hour * 100 + mins;
    return key;
}

static string KeyTitle(TitleIndex index)
{
	// ключ — это просто название занятия
    return index.key;
}

static void OrderTitleIndexBySelection(TitleIndex A[], int N)
{
	// сортировка выбором
    int i, j;
    string MinEl;
    int MinIndex;
    TitleIndex x;

	// проходим по всем элементам массива
    for (i = 0; i < N - 1; i++)
    {
		// ищем минимальный элемент в неотсортированной части массива
        MinEl = KeyTitle(A[i]);
        MinIndex = i;

		// проходим по остальным элементам
        for (j = i + 1; j < N; j++)
        {
			// сравниваем текущий элемент с найденным минимумом
            if (KeyTitle(A[j]) < MinEl)
            {
				// нашли новый минимум
				// обновляем минимум
				// сохраняем значение и индекс
                MinEl = KeyTitle(A[j]);
                MinIndex = j;
            }
        }
		// меняем местами найденный минимум с первым элементом неотсортированной части
        x = A[MinIndex];
        A[MinIndex] = A[i];
        A[i] = x;
    }
}

void buildTitleIndex()
{
	// строим индекс по названию
    TitleIdxCount = 0;
    for (int i = 0; i < LessonCount; i++)
    {
		// учитываем только не удалённые записи
        if (!Lessons[i].deleted)
        {
			// добавляем запись в индекс
			// ключ — название занятия
            TitleIdx[TitleIdxCount].key = Lessons[i].title;
            TitleIdx[TitleIdxCount].pos = i;
            TitleIdxCount++;
        }
    }

	// сортируем индекс по названию
    if (TitleIdxCount > 1)
    {
        OrderTitleIndexBySelection(TitleIdx, TitleIdxCount);
    }

    cout << "Индекс по названию построен. Элементов в индексе: " << TitleIdxCount << endl;
}

static int KeyDate(DateTimeIndex index)
{
	// ключ — это просто целое число, вычисленное из даты и времени
	// в формате YYYYMMDDHHMM
    return index.key;
}

static void OrderDateIndexByInsert(DateTimeIndex* A, int AN)
{
    int C = 0;
    DateTimeIndex x;
    int i, j;

    for (i = 1; i < AN; i++)
    {
		// вставка A[i] в отсортированную часть A[0..i-1]
		// сохраняем текущий элемент
        x = A[i];
		// ищем позицию для вставки
        j = i - 1;
        while (j >= 0 && KeyDate(x) < KeyDate(A[j]))
        {
			// сдвигаем элементы вправо
            C++;
            A[j + 1] = A[j];
            j = j - 1;
        }
		// вставляем элемент на найденную позицию
        A[j + 1] = x;
    }
}

void buildDateIndex()
{
	// строим индекс по дате и времени
    DateIdxCount = 0;
    for (int i = 0; i < LessonCount; i++)
    {
		// учитываем только не удалённые записи
        if (!Lessons[i].deleted)
        {
			// добавляем запись в индекс
			// ключ — вычисленное целое число из даты и времени
            DateIdx[DateIdxCount].key = ComputeDateTimeKey(Lessons[i]);
            DateIdx[DateIdxCount].pos = i;
            DateIdxCount++;
        }
    }

    if (DateIdxCount > 1)
    {
        OrderDateIndexByInsert(DateIdx, DateIdxCount);
    }

    cout << "Индекс по дате и времени построен. Элементов в индексе: " << DateIdxCount << endl;
}


void printByTitleIndex(bool ascending)
{
	// выводим записи по индексу названий
    if (TitleIdxCount == 0)
    {
        cout << "Индекс по названию пуст. Сначала постройте индекс." << endl;
        return;
    }

    cout << "Вывод по индексу названий (" << (ascending ? "по возрастанию" : "по убыванию") << "):" << endl;

    if (ascending)
    {
        for (int i = 0; i < TitleIdxCount; i++)
        {
            int pos = TitleIdx[i].pos;
            ::PrintLesson(Lessons[pos], pos);
        }
    }
    else
    {
        for (int i = TitleIdxCount - 1; i >= 0; i--)
        {
            int pos = TitleIdx[i].pos;
            ::PrintLesson(Lessons[pos], pos);
        }
    }
}

void printByDateIndex(bool ascending)
{
	// выводим записи по индексу даты и времени
	// проверяем, что индекс не пуст
    if (DateIdxCount == 0)
    {
        cout << "Индекс по дате и времени пуст. Сначала постройте индекс." << endl;
        return;
    }

    cout << "Вывод по индексу даты и времени (" << (ascending ? "по возрастанию" : "по убыванию") << "):" << endl;

	// выводим записи в порядке, заданном индексом
    if (ascending)
    {
        for (int i = 0; i < DateIdxCount; i++)
        {
            int pos = DateIdx[i].pos;
            ::PrintLesson(Lessons[pos], pos);
        }
    }
    else
    {
        for (int i = DateIdxCount - 1; i >= 0; i--)
        {
            int pos = DateIdx[i].pos;
            ::PrintLesson(Lessons[pos], pos);
        }
    }
}
