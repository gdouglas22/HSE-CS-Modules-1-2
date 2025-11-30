#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include <iostream>

using namespace std;

static const auto& msg = GetStrings();

int ComputeDateTimeKey(const Lesson& L)
{
    int day, month, year, hour, mins;
    if (ValidateDate(L.date, day, month, year) != ValidationStatus::Ok) return 0;
    if (ValidateTime(L.time, hour, mins) != ValidationStatus::Ok) return 0;
    return year * 100000000 + month * 1000000 + day * 10000 + hour * 100 + mins;
}

static string KeyTitle(TitleIndex index)
{
    return index.key;
}

static void OrderTitleIndexBySelection(TitleIndex A[], int N)
{
    for (int i = 0; i < N - 1; i++)
    {
        string minEl = KeyTitle(A[i]);
        int minIndex = i;
        for (int j = i + 1; j < N; j++)
        {
            if (KeyTitle(A[j]) < minEl)
            {
                minEl = KeyTitle(A[j]);
                minIndex = j;
            }
        }
        TitleIndex x = A[minIndex];
        A[minIndex] = A[i];
        A[i] = x;
    }
}

void BuildTitleIndex()
{
    TitleIdxCount = 0;
    for (int i = 0; i < LessonCount; i++)
    {
        if (!Lessons[i].deleted)
        {
            TitleIdx[TitleIdxCount].key = Lessons[i].title;
            TitleIdx[TitleIdxCount].pos = i;
            TitleIdxCount++;
        }
    }

    if (TitleIdxCount > 1)
    {
        OrderTitleIndexBySelection(TitleIdx, TitleIdxCount);
    }

    cout << msg.titleIndexBuiltPrefix << TitleIdxCount << endl;
}

static int KeyDate(DateTimeIndex index)
{
    return index.key;
}

static void OrderDateIndexByInsert(DateTimeIndex* A, int AN)
{
    for (int i = 1; i < AN; i++)
    {
        DateTimeIndex x = A[i];
        int j = i - 1;
        while (j >= 0 && KeyDate(x) < KeyDate(A[j]))
        {
            A[j + 1] = A[j];
            j = j - 1;
        }
        A[j + 1] = x;
    }
}

void BuildDateIndex()
{
    DateIdxCount = 0;
    for (int i = 0; i < LessonCount; i++)
    {
        if (!Lessons[i].deleted)
        {
            DateIdx[DateIdxCount].key = ComputeDateTimeKey(Lessons[i]);
            DateIdx[DateIdxCount].pos = i;
            DateIdxCount++;
        }
    }

    if (DateIdxCount > 1)
    {
        OrderDateIndexByInsert(DateIdx, DateIdxCount);
    }

    cout << msg.dateIndexBuiltPrefix << DateIdxCount << endl;
}

void PrintByTitleIndex(bool ascending)
{
    if (TitleIdxCount == 0)
    {
        cout << msg.titleIndexEmpty << endl;
        return;
    }

    cout << msg.titleIndexHeadingPrefix << (ascending ? msg.ascendingLabel : msg.descendingLabel) << "):" << endl;

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

void PrintByDateIndex(bool ascending)
{
    if (DateIdxCount == 0)
    {
        cout << msg.dateIndexEmpty << endl;
        return;
    }

    cout << msg.dateIndexHeadingPrefix << (ascending ? msg.ascendingLabel : msg.descendingLabel) << "):" << endl;

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
