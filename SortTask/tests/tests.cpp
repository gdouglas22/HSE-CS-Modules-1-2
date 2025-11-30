#include "lesson.h"
#include "messages.h"
#include "validation.h"
#include "types.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

static const auto& msg = GetStrings();

static void resetData()
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

TEST(Validation, DateFormat)
{
    int d, m, y;
    EXPECT_EQ(ValidationStatus::BadFormat, ValidateDate("2024-01-01", d, m, y));
}

TEST(Validation, DateRange)
{
    int d, m, y;
    EXPECT_EQ(ValidationStatus::OutOfRange, ValidateDate("32.01.2024", d, m, y));
}

TEST(Validation, TimeFormat)
{
    int h, m;
    EXPECT_EQ(ValidationStatus::BadFormat, ValidateTime("9.00", h, m));
}

TEST(Validation, TimeRange)
{
    int h, m;
    EXPECT_EQ(ValidationStatus::OutOfRange, ValidateTime("25:61", h, m));
}

TEST(Types, ParseLessonType)
{
    LessonType t;
    EXPECT_TRUE(ParseLessonType("Lecture", t));
    EXPECT_EQ(LessonType::Lecture, t);
    EXPECT_TRUE(ParseLessonType("3", t));
    EXPECT_EQ(LessonType::Lab, t);
    EXPECT_FALSE(ParseLessonType("Unknown", t));
}

TEST(Types, ParseLessonPriority)
{
    LessonPriority p;
    EXPECT_TRUE(ParseLessonPriority("High", p));
    EXPECT_EQ(LessonPriority::High, p);
    EXPECT_TRUE(ParseLessonPriority("1", p));
    EXPECT_EQ(LessonPriority::Low, p);
    EXPECT_FALSE(ParseLessonPriority("Ultra", p));
}

TEST(Indexing, BuildTitleIndexSorted)
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

    BuildTitleIndex();

    EXPECT_EQ(3, TitleIdxCount);
    EXPECT_EQ("Algebra", TitleIdx[0].key);
    EXPECT_EQ("Math", TitleIdx[1].key);
    EXPECT_EQ("Physics", TitleIdx[2].key);
}

TEST(Indexing, BuildDateIndexSorted)
{
    resetData();
    Lesson a, b, c;
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

    BuildDateIndex();

    EXPECT_EQ(3, DateIdxCount);
    EXPECT_EQ(2, DateIdx[0].pos);
    EXPECT_EQ(1, DateIdx[1].pos);
    EXPECT_EQ(0, DateIdx[2].pos);
}

TEST(IO, LoadSkipsInvalidAndStopsAtCapacity)
{
    resetData();
    const string fileName = "test_overflow.txt";
    {
        ofstream out(fileName);
        for (int i = 0; i < MAX_LESSONS + 5; ++i)
        {
            out << "Title" << i << ";Teacher" << i << ";01.01.2024;10:00;101;Lecture;High;0\n";
        }
    }
    LoadFromFile(fileName);
    EXPECT_EQ(MAX_LESSONS, LessonCount);
}

TEST(IO, LoadSkipsInvalidFormats)
{
    resetData();
    const string fileName = "test_invalid.txt";
    {
        ofstream out(fileName);
        out << "Valid;Teach;01.01.2024;10:00;101;Lecture;High;0\n";
        out << "BadDate;Teach;2024-01-01;10:00;101;Lecture;High;0\n";
        out << "BadTime;Teach;01.01.2024;25:61;101;Lecture;High;0\n";
        out << "BadType;Teach;01.01.2024;10:00;101;Unknown;High;0\n";
        out << "BadPriority;Teach;01.01.2024;10:00;101;Lecture;Ultra;0\n";
    }
    LoadFromFile(fileName);
    EXPECT_EQ(1, LessonCount);
    EXPECT_EQ("Valid", Lessons[0].title);
}

TEST(Search, ByTitleFoundAndNotFound)
{
    resetData();
    Lesson l1;
    l1.title = "Math";
    Lessons[0] = l1;
    LessonCount = 1;
    BuildTitleIndex();

    stringstream input("\nMath\n");
    stringstream output;
    streambuf* origIn = cin.rdbuf();
    streambuf* origOut = cout.rdbuf();
    cin.rdbuf(input.rdbuf());
    cout.rdbuf(output.rdbuf());

    SearchByTitle();

    cin.rdbuf(origIn);
    cout.rdbuf(origOut);
    string out = output.str();
    EXPECT_NE(string::npos, out.find(msg.recordFound));

    stringstream input2("\nPhysics\n");
    stringstream output2;
    cin.rdbuf(input2.rdbuf());
    cout.rdbuf(output2.rdbuf());
    SearchByTitle();
    cin.rdbuf(origIn);
    cout.rdbuf(origOut);
    string out2 = output2.str();
    EXPECT_NE(string::npos, out2.find(msg.titleNotFound));
}

TEST(Search, ByDateTimeFound)
{
    resetData();
    Lesson l1;
    l1.title = "Math";
    l1.date = "15.04.2024";
    l1.time = "09:00";
    Lessons[0] = l1;
    LessonCount = 1;
    BuildDateIndex();

    stringstream input("\n15.04.2024\n09:00\n");
    stringstream output;
    streambuf* origIn = cin.rdbuf();
    streambuf* origOut = cout.rdbuf();
    cin.rdbuf(input.rdbuf());
    cout.rdbuf(output.rdbuf());

    SearchByDateTime();

    cin.rdbuf(origIn);
    cout.rdbuf(origOut);
    string out = output.str();
    EXPECT_NE(string::npos, out.find(msg.recordFound));
}

TEST(Edit, LogicalDeleteRestorePhysical)
{
    resetData();
    Lesson l1;
    l1.title = "Math";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::Medium;
    Lessons[0] = l1;
    LessonCount = 1;
    BuildTitleIndex();

    stringstream inputDel("\nMath\n");
    streambuf* origIn = cin.rdbuf();
    cin.rdbuf(inputDel.rdbuf());
    LogicalDeleteLesson();
    cin.rdbuf(origIn);
    EXPECT_TRUE(Lessons[0].deleted);

    BuildTitleIndex();
    stringstream inputRestore("\nMath\n");
    cin.rdbuf(inputRestore.rdbuf());
    RestoreDeletedLesson();
    cin.rdbuf(origIn);
    EXPECT_FALSE(Lessons[0].deleted);

    Lessons[0].deleted = true;
    PhysicalDeleteMarked();
    EXPECT_EQ(0, LessonCount);
}

TEST(Edit, EditLessonUpdatesAllFields)
{
    resetData();
    Lesson l1;
    l1.title = "Math";
    l1.teacher = "Old Teacher";
    l1.date = "15.04.2024";
    l1.time = "09:00";
    l1.room = "101";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::Medium;
    Lessons[0] = l1;
    LessonCount = 1;
    BuildTitleIndex();

    stringstream input(
        "\nMath\n"          // ключ для поиска записи
        "Advanced Math\n"   // новое название
        "New Teacher\n"     // новый преподаватель
        "16.04.2024\n"      // новая дата
        "10:30\n"           // новое время
        "202\n"             // новая аудитория
        "2\n"               // тип: Practice
        "3\n"               // приоритет: High
    );

    streambuf* origIn = cin.rdbuf();
    cin.rdbuf(input.rdbuf());

    EditLesson();

    cin.rdbuf(origIn);

    EXPECT_EQ(1, LessonCount);
    EXPECT_EQ("Advanced Math", Lessons[0].title);
    EXPECT_EQ("New Teacher", Lessons[0].teacher);
    EXPECT_EQ("16.04.2024", Lessons[0].date);
    EXPECT_EQ("10:30", Lessons[0].time);
    EXPECT_EQ("202", Lessons[0].room);
    EXPECT_EQ(LessonType::Practice, Lessons[0].type);
    EXPECT_EQ(LessonPriority::High, Lessons[0].priority);
}

TEST(IO, PromptHelpersAndMenuAndAddPrint)
{
    resetData();

    stringstream input(
        "\n"
        "Math\n"
        "Teacher\n"
        "01.05.2024\n"
        "12:30\n"
        "303\n"
        "1\n"   // Lecture
        "2\n"   // Medium
    );
    streambuf* origIn = cin.rdbuf();
    streambuf* origOut = cout.rdbuf();
    stringstream output;
    cin.rdbuf(input.rdbuf());
    cout.rdbuf(output.rdbuf());

    AddLessonFromKeyboard();

    stringstream menuOut;
    cout.rdbuf(menuOut.rdbuf());
    MenuDisplay();

    stringstream printOut;
    cout.rdbuf(printOut.rdbuf());
    PrintAllLessons();

    cin.rdbuf(origIn);
    cout.rdbuf(origOut);

    EXPECT_EQ(1, LessonCount);
    EXPECT_EQ("Math", Lessons[0].title);
    EXPECT_EQ("Teacher", Lessons[0].teacher);
    EXPECT_EQ("01.05.2024", Lessons[0].date);
    EXPECT_EQ("12:30", Lessons[0].time);
    EXPECT_EQ("303", Lessons[0].room);
    EXPECT_EQ(LessonType::Lecture, Lessons[0].type);
    EXPECT_EQ(LessonPriority::Medium, Lessons[0].priority);

    string printed = printOut.str();
    EXPECT_NE(string::npos, printed.find("Math"));
    EXPECT_NE(string::npos, printed.find("Teacher"));
    EXPECT_NE(string::npos, printed.find("12:30"));
}

TEST(IO, SaveToFileCreateAndAppend)
{
    resetData();
    Lesson l1;
    l1.title = "First";
    l1.teacher = "T1";
    l1.date = "01.01.2024";
    l1.time = "09:00";
    l1.room = "101";
    l1.type = LessonType::Lecture;
    l1.priority = LessonPriority::Low;

    Lesson l2 = l1;
    l2.title = "Second";
    l2.teacher = "T2";
    l2.time = "10:00";
    l2.priority = LessonPriority::High;

    Lessons[0] = l1;
    Lessons[1] = l2;
    LessonCount = 2;

    const string fileName = "test_save.txt";
    remove(fileName.c_str());
    SaveToFile(fileName, false);

    ifstream in1(fileName);
    string line;
    int lines = 0;
    string lastLine;
    while (getline(in1, line))
    {
        if (!line.empty())
        {
            lines++;
            lastLine = line;
        }
    }
    in1.close();
    EXPECT_EQ(2, lines);
    EXPECT_NE(string::npos, lastLine.find("Second"));

    resetData();
    Lessons[0] = l1;
    LessonCount = 1;
    SaveToFile(fileName, false);

    resetData();
    Lessons[0] = l2;
    LessonCount = 1;
    SaveToFile(fileName, true);

    ifstream in2(fileName);
    lines = 0;
    string last;
    while (getline(in2, line))
    {
        if (!line.empty())
        {
            lines++;
            last = line;
        }
    }
    in2.close();
    EXPECT_EQ(2, lines);
    EXPECT_NE(string::npos, last.find("Second"));
    remove(fileName.c_str());
}

TEST(Indexing, PrintByTitleAndDate)
{
    resetData();
    Lesson a;
    a.title = "Alpha";
    a.date = "01.01.2024";
    a.time = "08:00";
    Lessons[0] = a;
    LessonCount = 1;

    BuildTitleIndex();
    BuildDateIndex();

    stringstream titleAsc;
    streambuf* origOut = cout.rdbuf();
    cout.rdbuf(titleAsc.rdbuf());
    PrintByTitleIndex(true);
    cout.rdbuf(origOut);
    EXPECT_NE(string::npos, titleAsc.str().find("Alpha"));

    stringstream dateDesc;
    cout.rdbuf(dateDesc.rdbuf());
    PrintByDateIndex(false);
    cout.rdbuf(origOut);
    EXPECT_NE(string::npos, dateDesc.str().find("Alpha"));
}

int main(int argc, char** argv)
{
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
