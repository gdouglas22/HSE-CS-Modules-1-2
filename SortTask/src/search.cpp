#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include <iostream>
#include <limits>

using namespace std;

static const auto& msg = GetStrings();

static int RecBinarySearchTitle(int L, int R, const string& K)
{
    int M;
    if (L <= R)
    {
        M = ((R - L) / 2) + L;
        if (TitleIdx[M].key == K)
        {
            return M;
        }
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
        if (DateIdx[M].key > K)
        {
            R = M - 1;
        }
        else
        {
            L = M + 1;
        }
    }
    return -1;
}

void SearchByTitle()
{
    if (TitleIdxCount == 0)
    {
        cout << msg.buildTitleIndexFirst << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << msg.searchTitlePrompt;
    string key;
    getline(cin, key);

    int idx = FindTitleIndex(key);
    if (idx == -1)
    {
        cout << msg.titleNotFound << endl;
    }
    else
    {
        int pos = TitleIdx[idx].pos;
        cout << msg.recordFound << endl;
        ::PrintLesson(Lessons[pos], pos);
    }
}

void SearchByDateTime()
{
    if (DateIdxCount == 0)
    {
        cout << msg.buildDateIndexFirst << endl;
        return;
    }

    Lesson tmp;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool validDateEntered = false;
    while (!validDateEntered)
    {
        cout << msg.searchDatePrompt;
        getline(cin, tmp.date);
        int d, m, y;
        ValidationStatus status = ValidateDate(tmp.date, d, m, y);
        validDateEntered = (status == ValidationStatus::Ok);
        if (!validDateEntered)
        {
            if (status == ValidationStatus::BadFormat)
            {
                cout << msg.invalidDateFormat << endl;
            }
            else
            {
                cout << msg.invalidDateOutOfRange << endl;
            }
        }
    }

    bool validTimeEntered = false;
    while (!validTimeEntered)
    {
        cout << msg.searchTimePrompt;
        getline(cin, tmp.time);
        int h, m;
        ValidationStatus status = ValidateTime(tmp.time, h, m);
        validTimeEntered = (status == ValidationStatus::Ok);
        if (!validTimeEntered)
        {
            if (status == ValidationStatus::BadFormat)
            {
                cout << msg.invalidTimeFormat << endl;
            }
            else
            {
                cout << msg.invalidTimeOutOfRange << endl;
            }
        }
    }

    int key = ComputeDateTimeKey(tmp);
    int idx = IterBinarySearchDate(key);

    if (idx == -1)
    {
        cout << msg.dateNotFound << endl;
    }
    else
    {
        int pos = DateIdx[idx].pos;
        cout << msg.recordFound << endl;
        ::PrintLesson(Lessons[pos], pos);
    }
}
