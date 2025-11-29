#include "lesson.h"
#include <iostream>
#include <limits>

using namespace std;

static void RebuildIndexes()
{
    buildTitleIndex();
    buildDateIndex();
}


void editLesson()
{
	// редактирование записи по названию зан€ти€
	// если записей нет, выводим сообщение и выходим
	// если индекс по названию пуст, строим его
    if (LessonCount == 0)
    {
        cout << "Ќет записей дл€ редактировани€." << endl;
        return;
    }

    if (TitleIdxCount == 0)
    {
        buildTitleIndex();
    }

	// запрашиваем у пользовател€ название зан€ти€
	// выполн€ем итеративный бинарный поиск по индексу названий
	// если запись найдена, выводим текущие данные записи
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "¬ведите название зан€ти€ дл€ редактировани€> ";
    string key;
    getline(cin, key);


    int idx = FindTitleIndex(key);

    if (idx == -1)
    {
        cout << "«апись не найдена." << endl;
        return;
    }

    int pos = TitleIdx[idx].pos;
    Lesson& L = Lessons[pos];

    cout << "“екущие данные записи:" << endl;
    ::PrintLesson(L, pos);

    cout << "¬ведите новые значени€ (пуста€ строка Ч оставить без изменений)." << endl;
    string input;

    cout << "Ќовое название (" << L.title << ")> ";
    getline(cin, input);
    if (!input.empty()) L.title = input;

    cout << "Ќовый преподаватель (" << L.teacher << ")> ";
    getline(cin, input);
    if (!input.empty()) L.teacher = input;

    cout << "Ќова€ дата (" << L.date << ")> ";
    getline(cin, input);
    if (!input.empty()) L.date = input;

    cout << "Ќовое врем€ (" << L.time << ")> ";
    getline(cin, input);
    if (!input.empty()) L.time = input;

    cout << "Ќова€ аудитори€ (" << L.room << ")> ";
    getline(cin, input);
    if (!input.empty()) L.room = input;

    cout << "Ќовый тип (" << L.type << ")> ";
    getline(cin, input);
    if (!input.empty()) L.type = input;

    cout << "Ќовый приоритет (" << L.priority << ")> ";
    getline(cin, input);
    if (!input.empty()) L.priority = input;

    cout << "«апись обновлена." << endl;
    RebuildIndexes();
}

void logicalDeleteLesson()
{
    if (LessonCount == 0)
    {
        cout << "Ќет записей дл€ удалени€." << endl;
        return;
    }

    if (TitleIdxCount == 0)
    {
        buildTitleIndex();
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "¬ведите название зан€ти€ дл€ логического удалени€> ";
    string key;
    getline(cin, key);

    int LBound = 0;
    int RBound = TitleIdxCount - 1;
    int idx = -1;
    while (LBound <= RBound)
    {
        int M = (RBound - LBound) / 2 + LBound;
        if (TitleIdx[M].key == key)
        {
            idx = M;
            break;
        }
        else if (TitleIdx[M].key > key)
        {
            RBound = M - 1;
        }
        else
        {
            LBound = M + 1;
        }
    }

    if (idx == -1)
    {
        cout << "«апись не найдена." << endl;
        return;
    }

    int pos = TitleIdx[idx].pos;
    Lessons[pos].deleted = true;

    cout << "«апись помечена как удалЄнна€." << endl;
    RebuildIndexes();
}

void physicalDeleteMarked()
{
    int newCount = 0;
    for (int i = 0; i < LessonCount; i++)
    {
        if (!Lessons[i].deleted)
        {
            Lessons[newCount] = Lessons[i];
            newCount++;
        }
    }
    LessonCount = newCount;

    cout << "‘изически удалены помеченные записи. “екущее количество: " << LessonCount << endl;
    RebuildIndexes();
}
