#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct TestCase
{
    std::string name;
    void (*func)();
};

inline std::vector<TestCase>& GetRegistry()
{
    static std::vector<TestCase> tests;
    return tests;
}

inline int& GetFailureCount()
{
    static int fails = 0;
    return fails;
}

inline bool RegisterTest(const std::string& name, void (*func)())
{
    GetRegistry().push_back({name, func});
    return true;
}

inline void ReportFailure(const std::string& expr, const std::string& file, int line)
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
    int total = 0;
    for (const auto& t : GetRegistry())
    {
        total++;
        t.func();
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
            ReportFailure(std::string(#a " == " #b), __FILE__, __LINE__);                 \
        }                                                                                 \
    } while (0)

#define EXPECT_NE(a, b)                                                                   \
    do                                                                                    \
    {                                                                                     \
        if (IsEqual((a), (b)))                                                            \
        {                                                                                 \
            ReportFailure(std::string(#a " != " #b), __FILE__, __LINE__);                 \
        }                                                                                 \
    } while (0)

#define RUN_ALL_TESTS() RunAllTests()
