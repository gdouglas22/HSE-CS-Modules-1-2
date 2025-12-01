#pragma once
#include <string>
#include "types.h"

const int MAX_LESSONS = 1000;

struct Lesson
{
    std::string title;
    std::string teacher;
    std::string date;
    std::string time;
    std::string room;
    LessonType type = LessonType::Other;
    LessonPriority priority = LessonPriority::Medium;
    bool deleted = false;
};

struct TitleTreeNode
{
    std::string key;
    int pos;
    int left = -1;
    int right = -1;
    int height = 1;
};

struct DateTimeIndex
{
    int key;
    int pos;
};

struct IntIndex
{
    int key;
    int pos;
};

extern Lesson Lessons[MAX_LESSONS];
extern int LessonCount;

extern TitleTreeNode StringTreeNodes[MAX_LESSONS];
extern int StringTreeRoot;
extern int StringTreeCount;

extern DateTimeIndex DateIdx[MAX_LESSONS];
extern int DateIdxCount;

extern IntIndex IntIdx[MAX_LESSONS];
extern int IntIdxCount;

enum IndexField
{
    FieldTitle = 1,
    FieldTeacher = 2,
    FieldDateTime = 3,
    FieldRoom = 4,
    FieldType = 5,
    FieldPriority = 6
};

extern int CurrentIndexField;

void MenuDisplay();

void AddLessonFromKeyboard();
void PrintAllLessons();
void SaveToFile(const std::string& fileName, bool appendMode);
void LoadFromFile(const std::string& fileName);

void BuildTitleIndex();
void BuildDateIndex();
void BuildIndexForCurrentField();
void PrintByCurrentIndex(bool ascending);

void PrintByTitleIndex(bool ascending);
void PrintByDateIndex(bool ascending);

void SearchByTitle();
void SearchByDateTime();
void SearchByCurrentIndex();
void ChooseIndexField();

void EditLesson();
void LogicalDeleteLesson();
void RestoreDeletedLesson();
void PhysicalDeleteMarked();

int ComputeDateTimeKey(const Lesson& L);
void PrintLesson(const Lesson& L, int index);
int RecursiveFindTitlePos(const std::string& key);
int IterativeFindTitlePos(const std::string& key);
