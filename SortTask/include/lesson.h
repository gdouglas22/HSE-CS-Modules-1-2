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

struct TitleIndex
{
    std::string key;
    int pos;
};

struct DateTimeIndex
{
    int key;
    int pos;
};

extern Lesson Lessons[MAX_LESSONS];
extern int LessonCount;

extern TitleIndex TitleIdx[MAX_LESSONS];
extern int TitleIdxCount;

extern DateTimeIndex DateIdx[MAX_LESSONS];
extern int DateIdxCount;

void menu_display();

void addLessonFromKeyboard();
void printAllLessons();
void saveToFile(const std::string& fileName, bool appendMode);
void loadFromFile(const std::string& fileName);

void buildTitleIndex();
void buildDateIndex();

void printByTitleIndex(bool ascending);
void printByDateIndex(bool ascending);

void searchByTitle();
void searchByDateTime();

void editLesson();
void logicalDeleteLesson();
void restoreDeletedLesson();
void physicalDeleteMarked();

int ComputeDateTimeKey(const Lesson& L);
void PrintLesson(const Lesson& L, int index);
int FindTitleIndex(const std::string& key);
