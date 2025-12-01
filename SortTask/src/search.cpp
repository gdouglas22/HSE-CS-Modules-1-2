#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include "types.h"
#include <iostream>
#include <limits>

using namespace std;

static const auto& msg = GetStrings();

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
    if (StringTreeRoot == -1)
    {
        cout << msg.buildTitleIndexFirst << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << msg.searchTitlePrompt;
    string key;
    getline(cin, key);

    int found[MAX_LESSONS];
    int foundCount = 0;
    struct StackItem { int node; };
    StackItem stack[MAX_LESSONS];
    int top = 0;
    if (StringTreeRoot != -1)
    {
        stack[top].node = StringTreeRoot;
        top++;
    }
    while (top > 0)
    {
        StackItem item = stack[--top];
        int node = item.node;
        if (node == -1) continue;
        const string& k = StringTreeNodes[node].key;
        if (k == key)
        {
            if (StringTreeNodes[node].right != -1)
            {
                stack[top].node = StringTreeNodes[node].right;
                top++;
            }
            if (StringTreeNodes[node].left != -1)
            {
                stack[top].node = StringTreeNodes[node].left;
                top++;
            }
            found[foundCount++] = StringTreeNodes[node].pos;
        }
        else if (key < k)
        {
            if (StringTreeNodes[node].left != -1)
            {
                stack[top].node = StringTreeNodes[node].left;
                top++;
            }
        }
        else
        {
            if (StringTreeNodes[node].right != -1)
            {
                stack[top].node = StringTreeNodes[node].right;
                top++;
            }
        }
    }

    if (foundCount == 0)
    {
        cout << msg.titleNotFound << endl;
    }
    else
    {
        cout << msg.recordFound << endl;
        for (int i = 0; i < foundCount; ++i)
        {
            int pos = found[i];
            ::PrintLesson(Lessons[pos], pos);
        }
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
        cout << msg.recordFound << endl;
        int matches[MAX_LESSONS];
        int count = 0;
        int left = idx;
        while (left >= 0 && DateIdx[left].key == key)
        {
            matches[count++] = DateIdx[left].pos;
            left--;
        }
        int right = idx + 1;
        while (right < DateIdxCount && DateIdx[right].key == key)
        {
            matches[count++] = DateIdx[right].pos;
            right++;
        }
        for (int i = 0; i < count; i++)
        {
            ::PrintLesson(Lessons[matches[i]], matches[i]);
        }
    }
}

static int BinarySearchIntIndex(int key)
{
    int L = 0;
    int R = IntIdxCount - 1;
    while (L <= R)
    {
        int M = ((R - L) / 2) + L;
        if (IntIdx[M].key == key) return M;
        if (IntIdx[M].key > key) R = M - 1;
        else L = M + 1;
    }
    return -1;
}

void SearchByCurrentIndex()
{
    if (CurrentIndexField == FieldTitle || CurrentIndexField == FieldTeacher || CurrentIndexField == FieldRoom)
    {
        if (StringTreeRoot == -1)
        {
            cout << msg.buildTitleIndexFirst << endl;
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << msg.genericSearchPrompt;
        string key;
        getline(cin, key);

        int found[MAX_LESSONS];
        int foundCount = 0;
        struct StackItem { int node; };
        StackItem stack[MAX_LESSONS];
        int top = 0;
        if (StringTreeRoot != -1)
        {
            stack[top].node = StringTreeRoot;
            top++;
        }
        while (top > 0)
        {
            StackItem item = stack[--top];
            int node = item.node;
            if (node == -1) continue;
            const string& k = StringTreeNodes[node].key;
            if (k == key)
            {
                if (StringTreeNodes[node].right != -1)
                {
                    stack[top].node = StringTreeNodes[node].right;
                    top++;
                }
                if (StringTreeNodes[node].left != -1)
                {
                    stack[top].node = StringTreeNodes[node].left;
                    top++;
                }
                found[foundCount++] = StringTreeNodes[node].pos;
            }
            else if (key < k)
            {
                if (StringTreeNodes[node].left != -1)
                {
                    stack[top].node = StringTreeNodes[node].left;
                    top++;
                }
            }
            else
            {
                if (StringTreeNodes[node].right != -1)
                {
                    stack[top].node = StringTreeNodes[node].right;
                    top++;
                }
            }
        }

        if (foundCount == 0)
        {
            cout << msg.titleNotFound << endl;
        }
        else
        {
            cout << msg.recordFound << endl;
            for (int i = 0; i < foundCount; ++i)
            {
                int pos = found[i];
                ::PrintLesson(Lessons[pos], pos);
            }
        }
        return;
    }

    if (CurrentIndexField == FieldDateTime)
    {
        if (DateIdxCount == 0)
        {
            cout << msg.buildDateIndexFirst << endl;
            return;
        }
        Lesson tmp;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        bool validDate = false;
        while (!validDate)
        {
            cout << msg.searchDatePrompt;
            getline(cin, tmp.date);
            int d, m, y;
            ValidationStatus s = ValidateDate(tmp.date, d, m, y);
            validDate = (s == ValidationStatus::Ok);
            if (!validDate)
            {
                if (s == ValidationStatus::BadFormat) cout << msg.invalidDateFormat << endl;
                else cout << msg.invalidDateOutOfRange << endl;
            }
        }
        bool validTime = false;
        while (!validTime)
        {
            cout << msg.searchTimePrompt;
            getline(cin, tmp.time);
            int h, m;
            ValidationStatus s = ValidateTime(tmp.time, h, m);
            validTime = (s == ValidationStatus::Ok);
            if (!validTime)
            {
                if (s == ValidationStatus::BadFormat) cout << msg.invalidTimeFormat << endl;
                else cout << msg.invalidTimeOutOfRange << endl;
            }
        }
        int key = ComputeDateTimeKey(tmp);
        int idx = IterBinarySearchDate(key);
        if (idx == -1)
        {
            cout << msg.dateNotFound << endl;
            return;
        }
        int matches[MAX_LESSONS];
        int count = 0;
        int left = idx;
        while (left >= 0 && DateIdx[left].key == key)
        {
            matches[count++] = DateIdx[left].pos;
            left--;
        }
        int right = idx + 1;
        while (right < DateIdxCount && DateIdx[right].key == key)
        {
            matches[count++] = DateIdx[right].pos;
            right++;
        }
        cout << msg.recordFound << endl;
        for (int i = 0; i < count; i++)
        {
            ::PrintLesson(Lessons[matches[i]], matches[i]);
        }
        return;
    }

    if (CurrentIndexField == FieldType || CurrentIndexField == FieldPriority)
    {
        if (IntIdxCount == 0)
        {
            cout << msg.dateIndexEmpty << endl;
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        int key = 0;
        bool ok = false;
        if (CurrentIndexField == FieldType)
        {
            cout << msg.promptType << endl;
            cout << msg.promptTypeOptions << endl;
            string input;
            getline(cin, input);
            LessonType t;
            ok = ParseLessonType(input, t);
            if (ok) key = static_cast<int>(t);
        }
        else
        {
            cout << msg.promptPriority << endl;
            cout << msg.promptPriorityOptions << endl;
            string input;
            getline(cin, input);
            LessonPriority p;
            ok = ParseLessonPriority(input, p);
            if (ok) key = static_cast<int>(p);
        }
        if (!ok)
        {
            cout << msg.invalidGenericValue << endl;
            return;
        }
        int idx = BinarySearchIntIndex(key);
        if (idx == -1)
        {
            cout << msg.titleNotFound << endl;
            return;
        }
        int matches[MAX_LESSONS];
        int count = 0;
        int left = idx;
        while (left >= 0 && IntIdx[left].key == key)
        {
            matches[count++] = IntIdx[left].pos;
            left--;
        }
        int right = idx + 1;
        while (right < IntIdxCount && IntIdx[right].key == key)
        {
            matches[count++] = IntIdx[right].pos;
            right++;
        }
        cout << msg.recordFound << endl;
        for (int i = 0; i < count; i++)
        {
            ::PrintLesson(Lessons[matches[i]], matches[i]);
        }
    }
}
