#include "cmock.h"

#include <string.h>

#include "unity.h"

#define CMOCK_MAX_CALLBACKS 16

static CMockLifecycleCallback verify_callbacks[CMOCK_MAX_CALLBACKS];
static CMockLifecycleCallback destroy_callbacks[CMOCK_MAX_CALLBACKS];
static size_t verify_count = 0U;
static size_t destroy_count = 0U;

void CMock_Init(void)
{
    verify_count  = 0U;
    destroy_count = 0U;
    memset(verify_callbacks, 0, sizeof(verify_callbacks));
    memset(destroy_callbacks, 0, sizeof(destroy_callbacks));
}

static void register_callback(CMockLifecycleCallback *array, size_t *count, CMockLifecycleCallback callback)
{
    if (*count >= CMOCK_MAX_CALLBACKS)
    {
        TEST_FAIL_MESSAGE("CMock callback capacity exceeded. Increase CMOCK_MAX_CALLBACKS.");
    }

    array[*count] = callback;
    (*count)++;
}

void CMock_RegisterVerify(CMockLifecycleCallback callback)
{
    register_callback(verify_callbacks, &verify_count, callback);
}

void CMock_RegisterDestroy(CMockLifecycleCallback callback)
{
    register_callback(destroy_callbacks, &destroy_count, callback);
}

void CMock_Verify(void)
{
    for (size_t i = 0; i < verify_count; ++i)
    {
        if (verify_callbacks[i] != NULL)
        {
            verify_callbacks[i]();
        }
    }
}

void CMock_Destroy(void)
{
    for (size_t i = 0; i < destroy_count; ++i)
    {
        if (destroy_callbacks[i] != NULL)
        {
            destroy_callbacks[i]();
        }
    }

    CMock_Init();
}
