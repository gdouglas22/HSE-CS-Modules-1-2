#pragma once

#include <string>

enum class LessonType
{
    Lecture = 1,
    Practice = 2,
    Lab = 3,
    Seminar = 4,
    Other = 5
};

enum class LessonPriority
{
    Low = 1,
    Medium = 2,
    High = 3
};

bool ParseLessonType(const std::string& text, LessonType& value);
std::string FormatLessonType(LessonType value);

bool ParseLessonPriority(const std::string& text, LessonPriority& value);
std::string FormatLessonPriority(LessonPriority value);
