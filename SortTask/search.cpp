#include "lesson.h"
#include <iostream>
#include <limits>

using namespace std;

static int RecBinarySearchTitle(int L, int R, const string& K)
{
	// рекурсивный бинарный поиск по индексу названий
	// L - левая граница поиска
	// R - правая граница поиска
	// K - искомый ключ (название занятия)
	// возвращает индекс в TitleIdx массива, если найдено, иначе -1
	// базовый случай рекурсии - если левая граница меньше или равна правойs
    int M;
    if (L <= R)
    {
        M = ((R - L) / 2) + L;
        if (TitleIdx[M].key == K)
        {
            return M;
        }
        else
        {
            if (TitleIdx[M].key > K)
            {
                R = M - 1;
            }
            else
            {
                L = M + 1;
            }
            return RecBinarySearchTitle(L, R, K);
        }
    }
    return -1;
}

int FindTitleIndex(const string& key)
{
    if (TitleIdxCount == 0)
    {
        return -1;
    }
    return RecBinarySearchTitle(0, TitleIdxCount - 1, key);
}

static int IterBinarySearchDate(int K)
{
	// итеративный бинарный поиск по индексу даты и времени
	// K - искомый ключ (вычисленное целое число из даты и времени)
	// возвращает индекс в DateIdx массива, если найдено, иначе -1
	// устанавливаем границы поиска
	// L - левая граница
	// R - правая граница
    int L = 0;
    int R = DateIdxCount - 1;
    int M;

    while (L <= R)
    {
        M = ((R - L) / 2) + L;
        if (DateIdx[M].key == K)
        {
            return M;
        }
        else
        {
            if (DateIdx[M].key > K)
            {
                R = M - 1;
            }
            else
            {
                L = M + 1;
            }
        }
    }
    return -1;
}


void searchByTitle()
{
	// поиск по названию занятия с использованием рекурсивного бинарного поиска
	// проверяем, что индекс не пуст
	// если пуст, выводим сообщение и выходим
	// если не пуст, запрашиваем у пользователя название занятия
	// выполняем рекурсивный бинарный поиск по индексу названий
	// если запись найдена, выводим её
	// если не найдена, выводим сообщение
	// проверяем, что индекс не пуст
	// если пуст, выводим сообщение и выходим
	// если не пуст, запрашиваем у пользователя название занятия
    if (TitleIdxCount == 0)
    {
        cout << "Сначала постройте индекс по названию." << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите название занятия для поиска> ";
    string key;
    getline(cin, key);

    int idx = FindTitleIndex(key);
    if (idx == -1)
    {
        cout << "Запись с таким названием не найдена." << endl;
    }
    else
    {
        int pos = TitleIdx[idx].pos;
        cout << "Найдена запись:" << endl;
        ::PrintLesson(Lessons[pos], pos);
    }
}

void searchByDateTime()
{
    if (DateIdxCount == 0)
    {
        cout << "Сначала постройте индекс по дате и времени." << endl;
        return;
    }

    Lesson tmp;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Введите дату (DD.MM.YYYY)> ";
    getline(cin, tmp.date);
    cout << "Введите время (HH:MM)> ";
    getline(cin, tmp.time);

    int key = ComputeDateTimeKey(tmp);
    int idx = IterBinarySearchDate(key);

    if (idx == -1)
    {
        cout << "Занятие с такой датой и временем не найдено." << endl;
    }
    else
    {
        int pos = DateIdx[idx].pos;
        cout << "Найдена запись:" << endl;
        ::PrintLesson(Lessons[pos], pos);
    }
}
