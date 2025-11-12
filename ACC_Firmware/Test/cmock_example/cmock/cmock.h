#ifndef CMOCK_H
#define CMOCK_H

#include <stddef.h>

typedef void (*CMockLifecycleCallback)(void);

void CMock_Init(void);
void CMock_Verify(void);
void CMock_Destroy(void);
void CMock_RegisterVerify(CMockLifecycleCallback callback);
void CMock_RegisterDestroy(CMockLifecycleCallback callback);

#endif // CMOCK_H
