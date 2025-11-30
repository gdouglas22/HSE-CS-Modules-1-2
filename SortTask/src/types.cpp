#include "types.h"
#include <algorithm>
#include <cctype>

static std::string ToLower(const std::string& s)
{
    std::string res = s;
    for (size_t i = 0; i < res.size(); ++i)
    {
        res[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(res[i])));
    }
    return res;
}

static bool TryParseInt(const std::string& text, int& value)
{
    value = 0;
    if (text.empty())
    {
        return false;
    }
    bool negative = false;
    size_t pos = 0;
    if (text[0] == '-')
    {
        negative = true;
        pos = 1;
    }
    for (; pos < text.size(); ++pos)
    {
        char c = text[pos];
        if (c < '0' || c > '9')
        {
            return false;
        }
        value = value * 10 + (c - '0');
    }
    if (negative)
    {
        value = -value;
    }
    return true;
}

bool ParseLessonType(const std::string& text, LessonType& value)
{
    int numeric = 0;
    bool parsedNumber = TryParseInt(text, numeric);
    if (parsedNumber)
    {
        if (numeric >= 1 && numeric <= 5)
        {
            value = static_cast<LessonType>(numeric);
            return true;
        }
        return false;
    }

    std::string norm = ToLower(text);
    if (norm == "lecture" || norm == "лекция")
    {
        value = LessonType::Lecture;
        return true;
    }
    if (norm == "practice" || norm == "практика")
    {
        value = LessonType::Practice;
        return true;
    }
    if (norm == "lab" || norm == "лабораторная" || norm == "лабораторная работа")
    {
        value = LessonType::Lab;
        return true;
    }
    if (norm == "seminar" || norm == "семинар")
    {
        value = LessonType::Seminar;
        return true;
    }
    if (norm == "other" || norm == "другое")
    {
        value = LessonType::Other;
        return true;
    }
    return false;
}

std::string FormatLessonType(LessonType value)
{
    switch (value)
    {
    case LessonType::Lecture: return "Lecture";
    case LessonType::Practice: return "Practice";
    case LessonType::Lab: return "Lab";
    case LessonType::Seminar: return "Seminar";
    default: return "Other";
    }
}

bool ParseLessonPriority(const std::string& text, LessonPriority& value)
{
    int numeric = 0;
    bool parsedNumber = TryParseInt(text, numeric);
    if (parsedNumber)
    {
        if (numeric >= 1 && numeric <= 3)
        {
            value = static_cast<LessonPriority>(numeric);
            return true;
        }
        return false;
    }

    std::string norm = ToLower(text);
    if (norm == "low" || norm == "низкий")
    {
        value = LessonPriority::Low;
        return true;
    }
    if (norm == "medium" || norm == "средний")
    {
        value = LessonPriority::Medium;
        return true;
    }
    if (norm == "high" || norm == "высокий")
    {
        value = LessonPriority::High;
        return true;
    }
    return false;
}

std::string FormatLessonPriority(LessonPriority value)
{
    switch (value)
    {
    case LessonPriority::Low: return "Low";
    case LessonPriority::Medium: return "Medium";
    case LessonPriority::High: return "High";
    default: return "Medium";
    }
}
