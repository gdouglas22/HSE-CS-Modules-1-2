#include "validation.h"

static bool IsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int DaysInMonth(int month, int year)
{
    switch (month)
    {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return IsLeapYear(year) ? 29 : 28;
    default:
        return 0;
    }
}

static int TwoDigits(const std::string& s, size_t pos)
{
    return (s[pos] - '0') * 10 + (s[pos + 1] - '0');
}

ValidationStatus ValidateDate(const std::string& date, int& day, int& month, int& year)
{
    if (date.size() != 10 || date[2] != '.' || date[5] != '.')
    {
        return ValidationStatus::BadFormat;
    }
    for (size_t i = 0; i < date.size(); ++i)
    {
        bool isSeparator = (i == 2 || i == 5);
        if (!isSeparator)
        {
            if (date[i] < '0' || date[i] > '9') return ValidationStatus::BadFormat;
        }
    }

    day = TwoDigits(date, 0);
    month = TwoDigits(date, 3);
    year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    int maxDay = DaysInMonth(month, year);
    if (year > 0 && month >= 1 && month <= 12 && day >= 1 && day <= maxDay)
    {
        return ValidationStatus::Ok;
    }
    return ValidationStatus::OutOfRange;
}

ValidationStatus ValidateTime(const std::string& time, int& hour, int& mins)
{
    if (time.size() != 5 || time[2] != ':')
    {
        return ValidationStatus::BadFormat;
    }
    for (size_t i = 0; i < time.size(); ++i)
    {
        bool isSeparator = (i == 2);
        if (!isSeparator)
        {
            if (time[i] < '0' || time[i] > '9') return ValidationStatus::BadFormat;
        }
    }

    hour = TwoDigits(time, 0);
    mins = TwoDigits(time, 3);
    if (hour >= 0 && hour <= 23 && mins >= 0 && mins <= 59)
    {
        return ValidationStatus::Ok;
    }
    return ValidationStatus::OutOfRange;
}

bool IsValidDate(const std::string& date)
{
    int d, m, y;
    return ValidateDate(date, d, m, y) == ValidationStatus::Ok;
}

bool IsValidTime(const std::string& time)
{
    int h, m;
    return ValidateTime(time, h, m) == ValidationStatus::Ok;
}
