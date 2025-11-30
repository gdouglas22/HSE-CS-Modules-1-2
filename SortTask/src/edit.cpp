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
    BuildTitleIndex();
    BuildDateIndex();
}


void EditLesson()
{
    if (LessonCount == 0)
    {
        cout << msg.noRecordsToEdit << endl;
        return;
    }

    if (TitleIdxCount == 0)
    {
    BuildTitleIndex();
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << msg.editTitlePrompt;
    string key;
    getline(cin, key);


    int idx = FindTitleIndex(key);

    if (idx == -1)
    {
        cout << msg.recordNotFound << endl;
        return;
    }

    int pos = TitleIdx[idx].pos;
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

    if (TitleIdxCount == 0)
    {
        BuildTitleIndex();
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << msg.logicalDeletePrompt;
    string key;
    getline(cin, key);

    int LBound = 0;
    int RBound = TitleIdxCount - 1;
    int idx = -1;
    while (LBound <= RBound && idx == -1)
    {
        int M = (RBound - LBound) / 2 + LBound;
        if (TitleIdx[M].key == key)
        {
            idx = M;
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
        cout << msg.recordNotFound << endl;
        return;
    }

    int pos = TitleIdx[idx].pos;
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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << msg.restorePrompt;
    string key;
    getline(cin, key);

    int foundPos = -1;
    for (int i = 0; i < LessonCount && foundPos == -1; i++)
    {
        if (Lessons[i].title == key)
        {
            foundPos = i;
        }
    }

    if (foundPos == -1)
    {
        cout << msg.recordNotFound << endl;
        return;
    }

    Lessons[foundPos].deleted = false;
    cout << msg.recordRestored << endl;

    RebuildIndexes();
}
