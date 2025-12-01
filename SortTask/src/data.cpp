#include "lesson.h"

Lesson Lessons[MAX_LESSONS];
int LessonCount = 0;

TitleTreeNode StringTreeNodes[MAX_LESSONS];
int StringTreeRoot = -1;
int StringTreeCount = 0;

DateTimeIndex DateIdx[MAX_LESSONS];
int DateIdxCount = 0;

IntIndex IntIdx[MAX_LESSONS];
int IntIdxCount = 0;

int CurrentIndexField = FieldTitle;
