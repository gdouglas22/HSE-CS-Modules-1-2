#pragma once

#include <iostream>
#include <string>

struct TestCase
{
    const char* name;
    void (*func)();
};

inline TestCase* GetRegistry()
{
    static TestCase tests[256];
    return tests;
}

inline int& GetTestCount()
{
    static int count = 0;
    return count;
}

inline int& GetFailureCount()
{
    static int fails = 0;
    return fails;
}

inline bool RegisterTest(const char* name, void (*func)())
{
    int idx = GetTestCount();
    GetRegistry()[idx].name = name;
    GetRegistry()[idx].func = func;
    GetTestCount() = idx + 1;
    return true;
}

inline void ReportFailure(const char* expr, const char* file, int line)
{
    std::cerr << file << ":" << line << " EXPECT FAILED: " << expr << std::endl;
    GetFailureCount() += 1;
}

template <typename T, typename U>
inline bool IsEqual(const T& a, const U& b)
{
    return a == b;
}

inline int RunAllTests()
{
    int total = GetTestCount();
    for (int i = 0; i < total; ++i)
    {
        GetRegistry()[i].func();
    }
    std::cout << "[RUN] " << total << " tests, failures: " << GetFailureCount() << std::endl;
    return GetFailureCount() == 0 ? 0 : 1;
}

inline void InitGoogleTest(int*, char**)
{
}

#define TEST(SUITE, NAME)                                                                 \
    static void SUITE##_##NAME##_TestBody();                                              \
    static bool SUITE##_##NAME##_registered = RegisterTest(#SUITE "." #NAME,              \
                                                           &SUITE##_##NAME##_TestBody);   \
    static void SUITE##_##NAME##_TestBody()

#define EXPECT_TRUE(cond)                                                                 \
    do                                                                                    \
    {                                                                                     \
        if (!(cond))                                                                      \
        {                                                                                 \
            ReportFailure(#cond, __FILE__, __LINE__);                                     \
        }                                                                                 \
    } while (0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))

#define EXPECT_EQ(a, b)                                                                   \
    do                                                                                    \
    {                                                                                     \
        if (!IsEqual((a), (b)))                                                           \
        {                                                                                 \
            ReportFailure(#a " == " #b, __FILE__, __LINE__);                              \
        }                                                                                 \
    } while (0)

#define EXPECT_NE(a, b)                                                                   \
    do                                                                                    \
    {                                                                                     \
        if (IsEqual((a), (b)))                                                            \
        {                                                                                 \
            ReportFailure(#a " != " #b, __FILE__, __LINE__);                              \
        }                                                                                 \
    } while (0)

#define RUN_ALL_TESTS() RunAllTests()
