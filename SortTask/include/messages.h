#pragma once

struct LocaleConfig
{
    const char* localeName;
};

struct Strings
{
    const char* roomLabel;
    const char* deletedMarker;

    const char* promptFileName;
    const char* createOrAppendPrompt;
    const char* searchMenuHeading;
    const char* searchByTitleOption;
    const char* searchByDateOption;
    const char* searchChoicePrompt;
    const char* searchInvalidOption;
    const char* exitProgram;
    const char* invalidMenuOption;
    const char* invalidDateFormat;
    const char* invalidTimeFormat;
    const char* invalidDateOutOfRange;
    const char* invalidTimeOutOfRange;
    const char* invalidFileRecordSkippedFormat;
    const char* invalidFileRecordSkippedRange;
    const char* invalidFileRecordSkipped;
    const char* invalidTypePriorityInFile;

    const char* menuTitle;
    const char* menuDataStructure;
    const char* menuAuthorsHeading;
    const char* menuAuthor1;
    const char* menuAuthor2;
    const char* menuAbilities;
    const char* menuOptionsIntro;
    const char* menuOpt1;
    const char* menuOpt2;
    const char* menuOpt3;
    const char* menuOpt4;
    const char* menuOpt5;
    const char* menuOpt6;
    const char* menuOpt7;
    const char* menuOpt8;
    const char* menuOpt9;
    const char* menuOpt10;
    const char* menuOpt11;
    const char* menuOpt0;
    const char* menuChoicePrompt;

    const char* tooManyLessons;
    const char* promptTitle;
    const char* promptTeacher;
    const char* promptDate;
    const char* promptTime;
    const char* promptRoom;
    const char* promptType;
    const char* promptTypeOptions;
    const char* invalidTypeOption;
    const char* promptPriority;
    const char* promptPriorityOptions;
    const char* invalidPriorityOption;
    const char* addedPrefix;

    const char* noRecordsToPrint;
    const char* printHeading;
    const char* printHeader;

    const char* fileWriteError;
    const char* fileWrittenPrefix;
    const char* fileReadError;
    const char* arrayFullWarning;
    const char* loadFinishedPrefix;

    const char* titleIndexBuiltPrefix;
    const char* dateIndexBuiltPrefix;
    const char* titleIndexEmpty;
    const char* dateIndexEmpty;
    const char* ascendingLabel;
    const char* descendingLabel;
    const char* titleIndexHeadingPrefix;
    const char* dateIndexHeadingPrefix;

    const char* buildTitleIndexFirst;
    const char* searchTitlePrompt;
    const char* titleNotFound;
    const char* recordFound;
    const char* buildDateIndexFirst;
    const char* searchDatePrompt;
    const char* searchTimePrompt;
    const char* dateNotFound;

    const char* noRecordsToEdit;
    const char* editTitlePrompt;
    const char* recordNotFound;
    const char* currentRecordHeading;
    const char* editInstructions;
    const char* newTitlePromptPrefix;
    const char* newTeacherPromptPrefix;
    const char* newDatePromptPrefix;
    const char* newTimePromptPrefix;
    const char* newRoomPromptPrefix;
    const char* newTypePromptPrefix;
    const char* newPriorityPromptPrefix;
    const char* recordUpdated;

    const char* noRecordsToDelete;
    const char* logicalDeletePrompt;
    const char* restorePrompt;
    const char* recordMarked;
    const char* recordRestored;
    const char* physicalDeletePrefix;
};

const Strings& GetStrings();
const LocaleConfig& GetLocaleConfig();
