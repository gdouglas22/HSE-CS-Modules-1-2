#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>

using namespace std;

static const auto& msg = GetStrings();

static int testsPassed = 0;
static int testsFailed = 0;

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        cout << "[FAIL] " << __FUNCTION__ << " : " << #cond \
             << " at line " << __LINE__ << endl; \
        ++testsFailed; \
    } else { \
        ++testsPassed; \
    } \
} while(0)

void resetData()
{
    LessonCount = 0;
    TitleIdxCount = 0;
    DateIdxCount = 0;
    for (int i = 0; i < MAX_LESSONS; ++i)
    {
        Lessons[i].title.clear();
        Lessons[i].teacher.clear();
        Lessons[i].date.clear();
        Lessons[i].time.clear();
        Lessons[i].room.clear();
        Lessons[i].type = LessonType::Other;
        Lessons[i].priority = LessonPriority::Medium;
        Lessons[i].deleted = false;
    }
}

void testComputeDateTimeKey()
{
    resetData();
    Lesson l1;
    l1.date = "15.04.2024";
    l1.time = "09:00";

    Lesson l2;
    l2.date = "15.04.2024";
    l2.time = "10:00";

    Lesson l3;
    l3.date = "16.04.2024";
    l3.time = "08:30";

    int k1 = ComputeDateTimeKey(l1);
    int k2 = ComputeDateTimeKey(l2);
    int k3 = ComputeDateTimeKey(l3);

    ASSERT_TRUE(k1 < k2);
    ASSERT_TRUE(k2 < k3);
}

void testBuildTitleIndex()
{
    resetData();
    Lesson a, b, c;
    a.title = "Math";
    b.title = "Physics";
    c.title = "Algebra";

    Lessons[0] = a;
    Lessons[1] = b;
    Lessons[2] = c;
    LessonCount = 3;

    buildTitleIndex();

    ASSERT_TRUE(TitleIdxCount == 3);
    ASSERT_TRUE(TitleIdx[0].key == "Algebra");
    ASSERT_TRUE(TitleIdx[1].key == "Math");
    ASSERT_TRUE(TitleIdx[2].key == "Physics");
    ASSERT_TRUE(TitleIdx[0].pos == 2);
    ASSERT_TRUE(TitleIdx[1].pos == 0);
    ASSERT_TRUE(TitleIdx[2].pos == 1);

    Lessons[1].deleted = true;
    buildTitleIndex();
    ASSERT_TRUE(TitleIdxCount == 2);
}

void testBuildDateIndex()
{
    resetData();

    Lesson a, b, c;
    a.title = "A";
    b.title = "B";
    c.title = "C";

    a.date = "16.04.2024";
    a.time = "10:00";

    b.date = "15.04.2024";
    b.time = "09:00";

    c.date = "15.04.2024";
    c.time = "08:00";

    Lessons[0] = a;
    Lessons[1] = b;
    Lessons[2] = c;
    LessonCount = 3;

    buildDateIndex();

    ASSERT_TRUE(DateIdxCount == 3);

    int pos0 = DateIdx[0].pos;
    int pos1 = DateIdx[1].pos;
    int pos2 = DateIdx[2].pos;

    ASSERT_TRUE(pos0 == 2);
    ASSERT_TRUE(pos1 == 1);
    ASSERT_TRUE(pos2 == 0);
}

void testSaveLoadFile()
{
    resetData();

    Lesson l1;
    l1.title = "Math";
    l1.teacher = "Petrov";
    l1.date = "15.04.2024";
    l1.time = "09:00";
    l1.room = "201";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::High;
    l1.deleted = false;

    Lesson l2;
    l2.title = "Physics";
    l2.teacher = "Ivanov";
    l2.date = "16.04.2024";
    l2.time = "11:00";
    l2.room = "202";
    l2.type = LessonType::Practice;
    l2.priority = LessonPriority::Medium;
    l2.deleted = true;

    Lessons[0] = l1;
    Lessons[1] = l2;
    LessonCount = 2;

    const string fileName = "test_schedule.txt";
    saveToFile(fileName, false);

    resetData();
    loadFromFile(fileName);

    ASSERT_TRUE(LessonCount == 2);
    ASSERT_TRUE(Lessons[0].title == "Math");
    ASSERT_TRUE(Lessons[0].teacher == "Petrov");
    ASSERT_TRUE(Lessons[0].priority == LessonPriority::High);
    ASSERT_TRUE(Lessons[0].deleted == false);

    ASSERT_TRUE(Lessons[1].title == "Physics");
    ASSERT_TRUE(Lessons[1].teacher == "Ivanov");
    ASSERT_TRUE(Lessons[1].priority == LessonPriority::Medium);
    ASSERT_TRUE(Lessons[1].deleted == true);
}

void testLogicalAndPhysicalDelete()
{
    resetData();

    Lesson l1;
    l1.title = "Math";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::Medium;
    Lesson l2;
    l2.title = "Physics";
    l2.type = LessonType::Practice;
    l2.priority = LessonPriority::High;

    Lessons[0] = l1;
    Lessons[1] = l2;
    LessonCount = 2;

    buildTitleIndex();

    std::streambuf* origIn = cin.rdbuf();
    std::istringstream fakeInput("\nMath\n");
    cin.rdbuf(fakeInput.rdbuf());

    logicalDeleteLesson();

    cin.rdbuf(origIn);

    ASSERT_TRUE(LessonCount == 2);
    ASSERT_TRUE(Lessons[0].deleted == true);
    ASSERT_TRUE(Lessons[1].deleted == false);

    physicalDeleteMarked();

    ASSERT_TRUE(LessonCount == 1);
    ASSERT_TRUE(Lessons[0].title == "Physics");
    ASSERT_TRUE(Lessons[0].deleted == false);
}

void testRestoreLesson()
{
    resetData();

    Lesson l1;
    l1.title = "Math";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::Medium;

    Lessons[0] = l1;
    LessonCount = 1;

    buildTitleIndex();
    Lessons[0].deleted = true;

    std::streambuf* origIn = cin.rdbuf();
    std::istringstream fakeInput("\nMath\n");
    cin.rdbuf(fakeInput.rdbuf());

    restoreDeletedLesson();

    cin.rdbuf(origIn);

    ASSERT_TRUE(LessonCount == 1);
    ASSERT_TRUE(Lessons[0].deleted == false);
}

void testEditLesson()
{
    resetData();

    Lesson l1;
    l1.title = "Math";
    l1.teacher = "Old Teacher";
    l1.date = "15.04.2024";
    l1.time = "09:00";
    l1.room = "201";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::Medium;
    l1.deleted = false;

    Lessons[0] = l1;
    LessonCount = 1;

    buildTitleIndex();

    std::streambuf* origIn = cin.rdbuf();
    std::istringstream fakeInput(
        "\nMath\n"
        "Advanced Math\n"
        "New Teacher\n"
        "\n" 
        "\n" 
        "\n"  
        "\n"  
        "High\n" 
    );
    cin.rdbuf(fakeInput.rdbuf());

    editLesson();

    cin.rdbuf(origIn);

    ASSERT_TRUE(LessonCount == 1);
    ASSERT_TRUE(Lessons[0].title == "Advanced Math");
    ASSERT_TRUE(Lessons[0].teacher == "New Teacher");
    ASSERT_TRUE(Lessons[0].priority == LessonPriority::High);
}

void testSearchByTitle()
{
    resetData();

    Lesson l1;
    l1.title = "Math";
    Lessons[0] = l1;
    LessonCount = 1;
    buildTitleIndex();

    std::streambuf* origIn = cin.rdbuf();
    std::streambuf* origOut = cout.rdbuf();

    std::istringstream fakeInput("\nMath\n");
    std::ostringstream fakeOutput;

    cin.rdbuf(fakeInput.rdbuf());
    cout.rdbuf(fakeOutput.rdbuf());

    searchByTitle();

    cin.rdbuf(origIn);
    cout.rdbuf(origOut);

    std::string out = fakeOutput.str();
    ASSERT_TRUE(out.find(msg.recordFound) != std::string::npos);
    ASSERT_TRUE(out.find("Math") != std::string::npos);
}

void testSearchByDateTime()
{
    resetData();

    Lesson l1;
    l1.title = "Math";
    l1.date = "15.04.2024";
    l1.time = "09:00";
    Lessons[0] = l1;
    LessonCount = 1;
    buildDateIndex();

    std::streambuf* origIn = cin.rdbuf();
    std::streambuf* origOut = cout.rdbuf();

    std::istringstream fakeInput("\n15.04.2024\n09:00\n");
    std::ostringstream fakeOutput;

    cin.rdbuf(fakeInput.rdbuf());
    cout.rdbuf(fakeOutput.rdbuf());

    searchByDateTime();

    cin.rdbuf(origIn);
    cout.rdbuf(origOut);

    std::string out = fakeOutput.str();
    ASSERT_TRUE(out.find(msg.recordFound) != std::string::npos);
    ASSERT_TRUE(out.find("Math") != std::string::npos);
}

int main()
{
    testComputeDateTimeKey();
    testBuildTitleIndex();
    testBuildDateIndex();
    testSaveLoadFile();
    testLogicalAndPhysicalDelete();
    testRestoreLesson();
    testEditLesson();
    testSearchByTitle();
    testSearchByDateTime();

    cout << "Tests passed: " << testsPassed << endl;
    cout << "Tests failed: " << testsFailed << endl;

    if (testsFailed == 0)
    {
        cout << "ALL TESTS PASSED" << endl;
        return 0;
    }
    else
    {
        cout << "SOME TESTS FAILED" << endl;
        return 1;
    }
}
