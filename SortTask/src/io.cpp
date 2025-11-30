#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include "types.h"
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

static const auto& msg = GetStrings();

static string PromptValidatedDate(const string& prompt)
{
    string value;
    bool done = false;
    while (!done)
    {
        cout << prompt;
        getline(cin, value);
        int d, m, y;
        ValidationStatus status = ValidateDate(value, d, m, y);
        if (status == ValidationStatus::Ok)
        {
            done = true;
        }
        else if (status == ValidationStatus::BadFormat)
        {
            cout << msg.invalidDateFormat << endl;
        }
        else
        {
            cout << msg.invalidDateOutOfRange << endl;
        }
    }
    return value;
}

static string PromptValidatedTime(const string& prompt)
{
    string value;
    bool done = false;
    while (!done)
    {
        cout << prompt;
        getline(cin, value);
        int h, m;
        ValidationStatus status = ValidateTime(value, h, m);
        if (status == ValidationStatus::Ok)
        {
            done = true;
        }
        else if (status == ValidationStatus::BadFormat)
        {
            cout << msg.invalidTimeFormat << endl;
        }
        else
        {
            cout << msg.invalidTimeOutOfRange << endl;
        }
    }
    return value;
}

static LessonType PromptLessonType()
{
    LessonType result = LessonType::Other;
    bool accepted = false;
    while (!accepted)
    {
        cout << msg.promptType << endl;
        cout << msg.promptTypeOptions << endl;
        string input;
        getline(cin, input);
        if (ParseLessonType(input, result))
        {
            accepted = true;
        }
        else
        {
            cout << msg.invalidTypeOption << endl;
        }
    }
    return result;
}

static LessonPriority PromptLessonPriority()
{
    LessonPriority result = LessonPriority::Medium;
    bool accepted = false;
    while (!accepted)
    {
        cout << msg.promptPriority << endl;
        cout << msg.promptPriorityOptions << endl;
        string input;
        getline(cin, input);
        if (ParseLessonPriority(input, result))
        {
            accepted = true;
        }
        else
        {
            cout << msg.invalidPriorityOption << endl;
        }
    }
    return result;
}

void PrintLesson(const Lesson& L, int index)
{
    cout << index << ") "
        << L.title << " | "
        << L.teacher << " | "
        << L.date << " " << L.time << " | "
        << msg.roomLabel << L.room << " | "
        << FormatLessonType(L.type) << " | "
        << FormatLessonPriority(L.priority);
    if (L.deleted)
    {
        cout << msg.deletedMarker;
    }
    cout << endl;
}

void menu_display()
{
    cout << msg.menuTitle << endl;
    cout << msg.menuDataStructure << endl;
    cout << msg.menuAuthorsHeading << endl;
    cout << msg.menuAuthor1 << endl;
    cout << msg.menuAuthor2 << endl;
    cout << endl;
    cout << msg.menuAbilities << endl;
    cout << msg.menuOptionsIntro << endl;
    cout << msg.menuOpt1 << endl;
    cout << msg.menuOpt2 << endl;
    cout << msg.menuOpt3 << endl;
    cout << msg.menuOpt4 << endl;
    cout << msg.menuOpt5 << endl;
    cout << msg.menuOpt6 << endl;
    cout << msg.menuOpt7 << endl;
    cout << msg.menuOpt8 << endl;
    cout << msg.menuOpt9 << endl;
    cout << msg.menuOpt10 << endl;
    cout << msg.menuOpt11 << endl;
    cout << msg.menuOpt0 << endl;
    cout << msg.menuChoicePrompt;
}

void addLessonFromKeyboard()
{
    if (LessonCount >= MAX_LESSONS)
    {
        cout << msg.tooManyLessons << endl;
        return;
    }

    Lesson L;
    L.deleted = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << msg.promptTitle;
    getline(cin, L.title);

    cout << msg.promptTeacher;
    getline(cin, L.teacher);

    L.date = PromptValidatedDate(msg.promptDate);
    L.time = PromptValidatedTime(msg.promptTime);

    cout << msg.promptRoom;
    getline(cin, L.room);

    L.type = PromptLessonType();
    L.priority = PromptLessonPriority();

    Lessons[LessonCount] = L;
    LessonCount++;

    cout << msg.addedPrefix << LessonCount << endl;
}

void printAllLessons()
{
    if (LessonCount == 0)
    {
        cout << msg.noRecordsToPrint << endl;
        return;
    }
    cout << msg.printHeading << endl;
    cout << msg.printHeader << endl;
    for (int i = 0; i < LessonCount; i++)
    {
        PrintLesson(Lessons[i], i);
    }
}

void saveToFile(const string& fileName, bool appendMode)
{
    ofstream OutFile;

    if (appendMode)
    {
        OutFile.open(fileName, ios::app);
    }
    else
    {
        OutFile.open(fileName);
    }

    if (!OutFile.is_open())
    {
        cout << msg.fileWriteError << endl;
        return;
    }

    for (int i = 0; i < LessonCount; i++)
    {
        Lesson& L = Lessons[i];
        OutFile << L.title << ";"
            << L.teacher << ";"
            << L.date << ";"
            << L.time << ";"
            << L.room << ";"
            << FormatLessonType(L.type) << ";"
            << FormatLessonPriority(L.priority) << ";"
            << (L.deleted ? 1 : 0)
            << endl;
    }

    OutFile.close();
    cout << msg.fileWrittenPrefix << fileName << endl;
}

void loadFromFile(const string& fileName)
{
    ifstream InFile(fileName);

    if (!InFile.is_open())
    {
        cout << msg.fileReadError << endl;
        return;
    }

    LessonCount = 0;
    string InLineVar;
    bool stopLoading = false;

    while (!stopLoading && getline(InFile, InLineVar))
    {
        if (!InLineVar.empty())
        {
            Lesson L;
            L.deleted = false;

            string fields[8];
            int fieldIndex = 0;
            string current = "";

            for (size_t i = 0; i < InLineVar.size(); i++)
            {
                char c = InLineVar[i];
                if (c == ';')
                {
                    if (fieldIndex < 8)
                    {
                        fields[fieldIndex] = current;
                        fieldIndex++;
                        current = "";
                    }
                }
                else
                {
                    current += c;
                }
            }
            if (fieldIndex < 8)
            {
                fields[fieldIndex] = current;
                fieldIndex++;
            }

            bool hasAllMandatoryFields = (fieldIndex >= 7);
            if (hasAllMandatoryFields)
            {
                L.title = fields[0];
                L.teacher = fields[1];
                L.date = fields[2];
                L.time = fields[3];
                L.room = fields[4];
                bool parsedType = ParseLessonType(fields[5], L.type);
                bool parsedPriority = ParseLessonPriority(fields[6], L.priority);
                if (fieldIndex >= 8)
                {
                    L.deleted = (fields[7] == "1");
                }

                int day, month, year, hour, mins;
                ValidationStatus dateStatus = ValidateDate(L.date, day, month, year);
                ValidationStatus timeStatus = ValidateTime(L.time, hour, mins);
                bool validDateTime = (dateStatus == ValidationStatus::Ok) && (timeStatus == ValidationStatus::Ok);
                bool validTypePriority = parsedType && parsedPriority;

                if (validDateTime && validTypePriority)
                {
                    if (LessonCount < MAX_LESSONS)
                    {
                        Lessons[LessonCount] = L;
                        LessonCount++;
                    }
                    else
                    {
                        cout << msg.arrayFullWarning << endl;
                        stopLoading = true;
                    }
                }
                else
                {
                    bool badFormat = (dateStatus == ValidationStatus::BadFormat) || (timeStatus == ValidationStatus::BadFormat);
                    bool rangeIssue = (dateStatus == ValidationStatus::OutOfRange) || (timeStatus == ValidationStatus::OutOfRange);
                    if (!validTypePriority)
                    {
                        cout << msg.invalidTypePriorityInFile << endl;
                    }
                    else if (badFormat)
                    {
                        cout << msg.invalidFileRecordSkippedFormat << endl;
                    }
                    else if (rangeIssue)
                    {
                        cout << msg.invalidFileRecordSkippedRange << endl;
                    }
                    else
                    {
                        cout << msg.invalidFileRecordSkipped << endl;
                    }
                }
            }
        }
    }

    InFile.close();
    cout << msg.loadFinishedPrefix << LessonCount << endl;
}
