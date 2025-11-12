#include "unity.h"

#include <setjmp.h>
#include <stdio.h>

jmp_buf UnityAbortFrame;
int UnityNumberOfTests    = 0;
int UnityNumberOfFailures = 0;

static const char *UnityCurrentTestName = NULL;
static int UnityCurrentTestLineNumber   = 0;

void UnityBegin(void)
{
    UnityNumberOfTests    = 0;
    UnityNumberOfFailures = 0;
    UnityCurrentTestName  = NULL;
    UnityCurrentTestLineNumber = 0;
    printf("==== ACC Firmware Unit Tests ====" "\n");
}

static void UnityReportTestResult(void)
{
    if (UnityCurrentTestName != NULL)
    {
        printf("[%s] %s\n",
               (UnityCurrentTestLineNumber == 0) ? "FAIL" : "PASS",
               UnityCurrentTestName);
    }
}

int UnityEnd(void)
{
    printf("\nSummary: %d test(s), %d failure(s)\n",
           UnityNumberOfTests,
           UnityNumberOfFailures);
    return UnityNumberOfFailures;
}

void UnityDefaultTestRun(UnityTestFunction func, const char *name, int line_number)
{
    UnityCurrentTestName       = name;
    UnityCurrentTestLineNumber = line_number;
    UnityNumberOfTests++;

    if (setjmp(UnityAbortFrame) == 0)
    {
        setUp();
        func();
        tearDown();
        UnityCurrentTestLineNumber = line_number;
    }
    else
    {
        tearDown();
        UnityNumberOfFailures++;
        UnityCurrentTestLineNumber = 0;
    }

    UnityReportTestResult();
}

void UnityFail(const char *message, unsigned int line_number)
{
    printf("Assertion failed at line %u: %s\n", line_number, message);
    UnityNumberOfFailures++;
    longjmp(UnityAbortFrame, 1);
}

void UnityAssertEqualInt(int expected, int actual, unsigned int line_number)
{
    if (expected != actual)
    {
        char buffer[128];
        snprintf(buffer, sizeof(buffer),
                 "Expected %d but was %d", expected, actual);
        UnityFail(buffer, line_number);
    }
}

void UnityAssertTrue(int condition, const char *message, unsigned int line_number)
{
    if (!condition)
    {
        UnityFail(message, line_number);
    }
}
