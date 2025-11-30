#pragma once

#include <string>

enum class ValidationStatus
{
    Ok,
    BadFormat,
    OutOfRange
};

ValidationStatus ValidateDate(const std::string& date, int& day, int& month, int& year);
ValidationStatus ValidateTime(const std::string& time, int& hour, int& mins);
bool IsValidDate(const std::string& date);
bool IsValidTime(const std::string& time);
