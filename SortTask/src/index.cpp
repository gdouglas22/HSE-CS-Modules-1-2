#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include <iostream>

using namespace std;

static const auto& msg = GetStrings();

static void ResetStringTree()
{
    StringTreeRoot = -1;
    StringTreeCount = 0;
}

static int Height(int idx)
{
    if (idx == -1) return 0;
    return StringTreeNodes[idx].height;
}

static void UpdateHeight(int idx)
{
    int hl = Height(StringTreeNodes[idx].left);
    int hr = Height(StringTreeNodes[idx].right);
    StringTreeNodes[idx].height = (hl > hr ? hl : hr) + 1;
}

static int RotateRight(int y)
{
    int x = StringTreeNodes[y].left;
    int T2 = StringTreeNodes[x].right;

    StringTreeNodes[x].right = y;
    StringTreeNodes[y].left = T2;

    UpdateHeight(y);
    UpdateHeight(x);
    return x;
}

static int RotateLeft(int x)
{
    int y = StringTreeNodes[x].right;
    int T2 = StringTreeNodes[y].left;

    StringTreeNodes[y].left = x;
    StringTreeNodes[x].right = T2;

    UpdateHeight(x);
    UpdateHeight(y);
    return y;
}

static int GetBalance(int idx)
{
    if (idx == -1) return 0;
    return Height(StringTreeNodes[idx].left) - Height(StringTreeNodes[idx].right);
}

static int AllocateStringNode(const string& key, int pos)
{
    if (StringTreeCount >= MAX_LESSONS)
    {
        return -1;
    }
    int idx = StringTreeCount;
    StringTreeNodes[idx].key = key;
    StringTreeNodes[idx].pos = pos;
    StringTreeNodes[idx].left = -1;
    StringTreeNodes[idx].right = -1;
    StringTreeNodes[idx].height = 1;
    StringTreeCount++;
    return idx;
}

static int InsertStringNode(int root, const string& key, int pos)
{
    if (root == -1)
    {
        return AllocateStringNode(key, pos);
    }

    if (key < StringTreeNodes[root].key)
    {
        int inserted = InsertStringNode(StringTreeNodes[root].left, key, pos);
        StringTreeNodes[root].left = inserted;
    }
    else
    {
        int inserted = InsertStringNode(StringTreeNodes[root].right, key, pos);
        StringTreeNodes[root].right = inserted;
    }

    UpdateHeight(root);
    int balance = GetBalance(root);

    if (balance > 1 && key < StringTreeNodes[StringTreeNodes[root].left].key)
    {
        return RotateRight(root);
    }
    if (balance < -1 && key > StringTreeNodes[StringTreeNodes[root].right].key)
    {
        return RotateLeft(root);
    }
    if (balance > 1 && key > StringTreeNodes[StringTreeNodes[root].left].key)
    {
        StringTreeNodes[root].left = RotateLeft(StringTreeNodes[root].left);
        return RotateRight(root);
    }
    if (balance < -1 && key < StringTreeNodes[StringTreeNodes[root].right].key)
    {
        StringTreeNodes[root].right = RotateRight(StringTreeNodes[root].right);
        return RotateLeft(root);
    }

    return root;
}

int ComputeDateTimeKey(const Lesson& L)
{
    int day, month, year, hour, mins;
    if (ValidateDate(L.date, day, month, year) != ValidationStatus::Ok) return 0;
    if (ValidateTime(L.time, hour, mins) != ValidationStatus::Ok) return 0;
    return year * 100000000 + month * 1000000 + day * 10000 + hour * 100 + mins;
}

static int MakeKeyInt(const Lesson& L, int field)
{
    if (field == FieldDateTime) return ComputeDateTimeKey(L);
    if (field == FieldType) return static_cast<int>(L.type);
    if (field == FieldPriority) return static_cast<int>(L.priority);
    return 0;
}

static string MakeKeyString(const Lesson& L, int field)
{
    if (field == FieldTitle) return L.title;
    if (field == FieldTeacher) return L.teacher;
    if (field == FieldRoom) return L.room;
    return "";
}

static void BuildStringIndex(int field)
{
    ResetStringTree();
    for (int i = 0; i < LessonCount; i++)
    {
        if (!Lessons[i].deleted)
        {
            StringTreeRoot = InsertStringNode(StringTreeRoot, MakeKeyString(Lessons[i], field), i);
        }
    }
    cout << msg.titleIndexBuiltPrefix << StringTreeCount << endl;
}

static int KeyDate(IntIndex index)
{
    return index.key;
}

static void OrderIntIndex(IntIndex* A, int AN)
{
    for (int i = 1; i < AN; i++)
    {
        IntIndex x = A[i];
        int j = i - 1;
        while (j >= 0 && KeyDate(x) < KeyDate(A[j]))
        {
            A[j + 1] = A[j];
            j = j - 1;
        }
        A[j + 1] = x;
    }
}

static void BuildIntIndex(int field)
{
    IntIdxCount = 0;
    for (int i = 0; i < LessonCount; i++)
    {
        if (!Lessons[i].deleted)
        {
            IntIdx[IntIdxCount].key = MakeKeyInt(Lessons[i], field);
            IntIdx[IntIdxCount].pos = i;
            IntIdxCount++;
        }
    }
    if (IntIdxCount > 1)
    {
        OrderIntIndex(IntIdx, IntIdxCount);
    }
    cout << msg.dateIndexBuiltPrefix << IntIdxCount << endl;
}

void BuildTitleIndex()
{
    CurrentIndexField = FieldTitle;
    BuildStringIndex(FieldTitle);
}

void BuildDateIndex()
{
    CurrentIndexField = FieldDateTime;
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
        for (int i = 1; i < DateIdxCount; i++)
        {
            DateTimeIndex x = DateIdx[i];
            int j = i - 1;
            while (j >= 0 && x.key < DateIdx[j].key)
            {
                DateIdx[j + 1] = DateIdx[j];
                j--;
            }
            DateIdx[j + 1] = x;
        }
    }
    cout << msg.dateIndexBuiltPrefix << DateIdxCount << endl;
}

void BuildIndexForCurrentField()
{
    if (CurrentIndexField == FieldTitle || CurrentIndexField == FieldTeacher || CurrentIndexField == FieldRoom)
    {
        BuildStringIndex(CurrentIndexField);
    }
    else if (CurrentIndexField == FieldDateTime)
    {
        BuildDateIndex();
    }
    else
    {
        BuildIntIndex(CurrentIndexField);
    }
}

static void TraverseStringAsc(int node)
{
    if (node == -1) return;
    TraverseStringAsc(StringTreeNodes[node].left);
    ::PrintLesson(Lessons[StringTreeNodes[node].pos], StringTreeNodes[node].pos);
    TraverseStringAsc(StringTreeNodes[node].right);
}

static void TraverseStringDesc(int node)
{
    if (node == -1) return;
    TraverseStringDesc(StringTreeNodes[node].right);
    ::PrintLesson(Lessons[StringTreeNodes[node].pos], StringTreeNodes[node].pos);
    TraverseStringDesc(StringTreeNodes[node].left);
}

void PrintByTitleIndex(bool ascending)
{
    if (CurrentIndexField == FieldTitle || CurrentIndexField == FieldTeacher || CurrentIndexField == FieldRoom)
    {
        if (StringTreeRoot == -1)
        {
            cout << msg.titleIndexEmpty << endl;
            return;
        }
        cout << msg.titleIndexHeadingPrefix << (ascending ? msg.ascendingLabel : msg.descendingLabel) << "):" << endl;
        if (ascending)
        {
            TraverseStringAsc(StringTreeRoot);
        }
        else
        {
            TraverseStringDesc(StringTreeRoot);
        }
    }
    else
    {
        PrintByDateIndex(ascending);
    }
}

void PrintByDateIndex(bool ascending)
{
    if (CurrentIndexField == FieldDateTime)
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
        return;
    }

    if (IntIdxCount == 0)
    {
        cout << msg.dateIndexEmpty << endl;
        return;
    }

    cout << msg.dateIndexHeadingPrefix << (ascending ? msg.ascendingLabel : msg.descendingLabel) << "):" << endl;

    if (ascending)
    {
        for (int i = 0; i < IntIdxCount; i++)
        {
            int pos = IntIdx[i].pos;
            ::PrintLesson(Lessons[pos], pos);
        }
    }
    else
    {
        for (int i = IntIdxCount - 1; i >= 0; i--)
        {
            int pos = IntIdx[i].pos;
            ::PrintLesson(Lessons[pos], pos);
        }
    }
}

void PrintByCurrentIndex(bool ascending)
{
    if (CurrentIndexField == FieldTitle || CurrentIndexField == FieldTeacher || CurrentIndexField == FieldRoom)
    {
        PrintByTitleIndex(ascending);
    }
    else
    {
        PrintByDateIndex(ascending);
    }
}

static int RecursiveFindNode(int node, const string& key)
{
    if (node == -1) return -1;
    if (StringTreeNodes[node].key == key) return StringTreeNodes[node].pos;
    if (key < StringTreeNodes[node].key) return RecursiveFindNode(StringTreeNodes[node].left, key);
    return RecursiveFindNode(StringTreeNodes[node].right, key);
}

int RecursiveFindTitlePos(const string& key)
{
    if (StringTreeRoot == -1) return -1;
    return RecursiveFindNode(StringTreeRoot, key);
}

int IterativeFindTitlePos(const string& key)
{
    int current = StringTreeRoot;
    while (current != -1)
    {
        if (StringTreeNodes[current].key == key)
        {
            return StringTreeNodes[current].pos;
        }
        if (key < StringTreeNodes[current].key)
        {
            current = StringTreeNodes[current].left;
        }
        else
        {
            current = StringTreeNodes[current].right;
        }
    }
    return -1;
}
