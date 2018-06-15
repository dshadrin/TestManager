#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SConnectionData
{
    char target[128];
    char user[64];
    char password[64];
} SConnectionData;

int IsEntityExists(SConnectionData* connData, const char * kind, const char * name);

#ifdef __cplusplus
}
#endif
