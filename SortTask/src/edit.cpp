#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include "types.h"
#include <iostream>
#include <limits>

using namespace std;

static const auto& msg = GetStrings();

static void RebuildIndexes()
{
    BuildIndexForCurrentField();
}

static int PromptAndFindFirstPos()
{
    if (CurrentIndexField == FieldTitle || CurrentIndexField == FieldTeacher || CurrentIndexField == FieldRoom)
    {
        if (StringTreeRoot == -1)
        {
            BuildIndexForCurrentField();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << msg.genericSearchPrompt;
        string key;
        getline(cin, key);
        return IterativeFindTitlePos(key);
    }
    if (CurrentIndexField == FieldDateTime)
    {
        if (DateIdxCount == 0)
        {
            BuildDateIndex();
        }
        Lesson tmp;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << msg.searchDatePrompt;
        getline(cin, tmp.date);
        cout << msg.searchTimePrompt;
        getline(cin, tmp.time);
        int key = ComputeDateTimeKey(tmp);
        int idx = -1;
        int L = 0, R = DateIdxCount - 1;
        while (L <= R)
        {
            int M = (R - L) / 2 + L;
            if (DateIdx[M].key == key)
            {
                idx = M;
                break;
            }
            if (DateIdx[M].key > key) R = M - 1;
            else L = M + 1;
        }
        if (idx == -1) return -1;
        return DateIdx[idx].pos;
    }
    if (CurrentIndexField == FieldType || CurrentIndexField == FieldPriority)
    {
        if (IntIdxCount == 0)
        {
            BuildIndexForCurrentField();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (CurrentIndexField == FieldType)
        {
            cout << msg.promptType << endl;
            cout << msg.promptTypeOptions << endl;
            string input;
            getline(cin, input);
            LessonType t;
            if (!ParseLessonType(input, t)) return -1;
            int key = static_cast<int>(t);
            int L = 0, R = IntIdxCount - 1;
            while (L <= R)
            {
                int M = (R - L) / 2 + L;
                if (IntIdx[M].key == key) return IntIdx[M].pos;
                if (IntIdx[M].key > key) R = M - 1; else L = M + 1;
            }
            return -1;
        }
        cout << msg.promptPriority << endl;
        cout << msg.promptPriorityOptions << endl;
        string input;
        getline(cin, input);
        LessonPriority p;
        if (!ParseLessonPriority(input, p)) return -1;
        int key = static_cast<int>(p);
        int L = 0, R = IntIdxCount - 1;
        while (L <= R)
        {
            int M = (R - L) / 2 + L;
            if (IntIdx[M].key == key) return IntIdx[M].pos;
            if (IntIdx[M].key > key) R = M - 1; else L = M + 1;
        }
    }
    return -1;
}

static int FindDeletedMatch()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (CurrentIndexField == FieldTitle || CurrentIndexField == FieldTeacher || CurrentIndexField == FieldRoom)
    {
        cout << msg.genericRestorePrompt;
        string key;
        getline(cin, key);
        for (int i = 0; i < LessonCount; i++)
        {
            if (Lessons[i].deleted)
            {
                string value = (CurrentIndexField == FieldTitle) ? Lessons[i].title :
                               (CurrentIndexField == FieldTeacher) ? Lessons[i].teacher :
                               Lessons[i].room;
                if (value == key) return i;
            }
        }
        return -1;
    }
    if (CurrentIndexField == FieldDateTime)
    {
        Lesson tmp;
        cout << msg.searchDatePrompt;
        getline(cin, tmp.date);
        cout << msg.searchTimePrompt;
        getline(cin, tmp.time);
        int key = ComputeDateTimeKey(tmp);
        for (int i = 0; i < LessonCount; i++)
        {
            if (Lessons[i].deleted && ComputeDateTimeKey(Lessons[i]) == key) return i;
        }
        return -1;
    }
    if (CurrentIndexField == FieldType || CurrentIndexField == FieldPriority)
    {
        if (CurrentIndexField == FieldType)
        {
            cout << msg.promptType << endl;
            cout << msg.promptTypeOptions << endl;
            string input;
            getline(cin, input);
            LessonType t;
            if (!ParseLessonType(input, t)) return -1;
            for (int i = 0; i < LessonCount; i++)
            {
                if (Lessons[i].deleted && Lessons[i].type == t) return i;
            }
        }
        else
        {
            cout << msg.promptPriority << endl;
            cout << msg.promptPriorityOptions << endl;
            string input;
            getline(cin, input);
            LessonPriority p;
            if (!ParseLessonPriority(input, p)) return -1;
            for (int i = 0; i < LessonCount; i++)
            {
                if (Lessons[i].deleted && Lessons[i].priority == p) return i;
            }
        }
    }
    return -1;
}


void EditLesson()
{
    if (LessonCount == 0)
    {
        cout << msg.noRecordsToEdit << endl;
        return;
    }

    if (StringTreeRoot == -1)
    {
        BuildTitleIndex();
    }

    int pos = PromptAndFindFirstPos();

    if (pos == -1)
    {
        cout << msg.recordNotFound << endl;
        return;
    }

    Lesson& L = Lessons[pos];

    cout << msg.currentRecordHeading << endl;
    ::PrintLesson(L, pos);

    cout << msg.editInstructions << endl;
    string input;

    auto updateField = [&](const string& promptPrefix, string& field, bool (*validator)(const string&), const char* errorMsg)
    {
        bool done = false;
        while (!done)
        {
            cout << promptPrefix << field << ")> ";
            getline(cin, input);
            if (input.empty())
            {
                done = true;
            }
            else if (!validator || validator(input))
            {
                field = input;
                done = true;
            }
            else if (errorMsg)
            {
                cout << errorMsg << endl;
            }
        }
    };

    updateField(msg.newTitlePromptPrefix, L.title, nullptr, nullptr);
    updateField(msg.newTeacherPromptPrefix, L.teacher, nullptr, nullptr);
    updateField(msg.newDatePromptPrefix, L.date, IsValidDate, msg.invalidDateFormat);
    updateField(msg.newTimePromptPrefix, L.time, IsValidTime, msg.invalidTimeFormat);
    updateField(msg.newRoomPromptPrefix, L.room, nullptr, nullptr);

    bool doneType = false;
    while (!doneType)
    {
        cout << msg.newTypePromptPrefix << FormatLessonType(L.type) << ")> ";
        getline(cin, input);
        if (input.empty())
        {
            doneType = true;
        }
        else
        {
            LessonType parsedTypeValue = L.type;
            bool parsedType = ParseLessonType(input, parsedTypeValue);
            if (parsedType)
            {
                L.type = parsedTypeValue;
                doneType = true;
            }
            else
            {
                cout << msg.invalidTypeOption << endl;
            }
        }
    }

    bool donePriority = false;
    while (!donePriority)
    {
        cout << msg.newPriorityPromptPrefix << FormatLessonPriority(L.priority) << ")> ";
        getline(cin, input);
        if (input.empty())
        {
            donePriority = true;
        }
        else
        {
            LessonPriority parsedPriorityValue = L.priority;
            bool parsedPriority = ParseLessonPriority(input, parsedPriorityValue);
            if (parsedPriority)
            {
                L.priority = parsedPriorityValue;
                donePriority = true;
            }
            else
            {
                cout << msg.invalidPriorityOption << endl;
            }
        }
    }

    cout << msg.recordUpdated << endl;
    RebuildIndexes();
}

void LogicalDeleteLesson()
{
    if (LessonCount == 0)
    {
        cout << msg.noRecordsToDelete << endl;
        return;
    }

    int pos = PromptAndFindFirstPos();

    if (pos == -1)
    {
        cout << msg.recordNotFound << endl;
        return;
    }

    Lessons[pos].deleted = true;

    cout << msg.recordMarked << endl;
    RebuildIndexes();
}

void PhysicalDeleteMarked()
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

    cout << msg.physicalDeletePrefix << LessonCount << endl;
    RebuildIndexes();
}

void RestoreDeletedLesson()
{
    if (LessonCount == 0)
    {
        cout << msg.noRecordsToDelete << endl;
        return;
    }

    int foundPos = FindDeletedMatch();

    if (foundPos == -1)
    {
        cout << msg.recordNotFound << endl;
        return;
    }

    Lessons[foundPos].deleted = false;
    cout << msg.recordRestored << endl;

    RebuildIndexes();
}
