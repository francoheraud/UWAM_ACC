#ifndef UNITY_H
#define UNITY_H

#include <setjmp.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*UnityTestFunction)(void);

extern jmp_buf UnityAbortFrame;
extern int UnityNumberOfTests;
extern int UnityNumberOfFailures;

void UnityBegin(void);
int  UnityEnd(void);
void UnityDefaultTestRun(UnityTestFunction func, const char *name, int line_number);
void UnityFail(const char *message, unsigned int line_number);

void UnityAssertEqualInt(int expected, int actual, unsigned int line_number);
void UnityAssertTrue(int condition, const char *message, unsigned int line_number);

#define UNITY_BEGIN()   do { UnityBegin(); } while (0)
#define UNITY_END()     UnityEnd()
#define RUN_TEST(func)  UnityDefaultTestRun(func, #func, __LINE__)

#define TEST_ASSERT_TRUE(condition) \
    UnityAssertTrue((condition), "Expected condition to be true", __LINE__)

#define TEST_ASSERT_FALSE(condition) \
    UnityAssertTrue(!(condition), "Expected condition to be false", __LINE__)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UnityAssertEqualInt((expected), (actual), __LINE__)

#define TEST_ASSERT_EQUAL_MESSAGE(expected, actual, message)                \
    do {                                                                    \
        if ((expected) != (actual)) {                                       \
            UnityFail((message), __LINE__);                                 \
        }                                                                   \
    } while (0)

#define TEST_ASSERT_NOT_NULL_MESSAGE(pointer, message)                      \
    do {                                                                    \
        if ((pointer) == NULL) {                                            \
            UnityFail((message), __LINE__);                                 \
        }                                                                   \
    } while (0)

#define TEST_FAIL_MESSAGE(message) UnityFail((message), __LINE__)

void setUp(void);
void tearDown(void);

#ifdef __cplusplus
}
#endif

#endif // UNITY_H
