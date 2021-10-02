#include "MemSpd2K.h"
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void getCPUstring(void);
void getCPUDetails(void);
void calculateMHz(void); 
double getRunSecs(void);
void   getStartTime(void);

char idString[20];
unsigned int megaHz;
unsigned int eaxCode;
unsigned int eaxCode1;
unsigned int edxCode;
LARGE_INTEGER astarttime;
double        runSecs;

char    configData1[200];
char    configData2[200];
char    configData3[200];
int     hasMMX;
DWORD   ramsize;
DWORD   freesize;

#define MM_EXTENSION 0x00800000


void WhatConfig()
{
    OSVERSIONINFO vers;
    char szosBuild[20] = "";
    char os[100];
    SYSTEM_INFO info;
    BOOL  OldSys;    
    MEMORYSTATUS lpBuffer;

    hasMMX = 0;

    // **************** OS *********************

    vers.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&vers);
    
    if (vers.dwPlatformId == VER_PLATFORM_WIN32s)
    {
        strcpy(os, " Windows 3.1");
    }
    else if (vers.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        strcpy(os, " Windows 95");
        if (vers.dwMinorVersion > 0) strcpy(os, " Windows 98");
        sprintf(szosBuild, "build %d,", (DWORD)(LOWORD(vers.dwBuildNumber)));
    }
    else if (vers.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        strcpy(os, " Windows NT");
        sprintf(szosBuild, "build %d,", vers.dwBuildNumber);

    }
    else
    {
        strcpy(os, "OS?");
    }
    sprintf (configData1, "%s Version %d.%d, %s %s",
                        os, vers.dwMajorVersion,
                        vers.dwMinorVersion, szosBuild, vers.szCSDVersion);

    // **************** CPU *********************

    lpBuffer.dwLength = sizeof(MEMORYSTATUS);

    GetSystemInfo(&info);
        
    OldSys = FALSE;
    if (info.dwProcessorType == PROCESSOR_INTEL_386)
    {
        strcpy(configData2, " CPU Type 80386");
        OldSys = TRUE;
    }
    if (info.dwProcessorType == PROCESSOR_INTEL_486)
    {
        strcpy(configData2, " CPU Type 80486");
        OldSys = TRUE;
    }

    if (!OldSys)
    {
       getCPUstring();
       getCPUDetails();
       calculateMHz();
       if (edxCode & MM_EXTENSION) hasMMX = 1;
       sprintf(configData2, " CPU %s, Features Code %8.8X, Model Code %8.8X, %d MHz",
                           idString, edxCode, eaxCode, megaHz);
    } 
    GlobalMemoryStatus(&lpBuffer);
    ramsize = lpBuffer.dwTotalPhys;
    freesize = lpBuffer.dwAvailPhys;
    sprintf(configData3, " From GlobalMemoryStatus: Size %d KB, Free %d KB", ramsize / 1024, freesize / 1024);
}

void getCPUstring(void)
{
    __asm
    {
        lea  edi, idString
        mov  eax, 0
        CPUID
        mov[edi],   ebx
        mov[edi+4], edx
        mov[edi+8], ecx
        mov[edi+12], 0
        mov eaxCode1, eax
    }
}

void getCPUDetails(void)
{
    __asm
    {
        mov  eax, 1
        CPUID
        mov eaxCode, eax  ; Features Code
        mov edxCode, edx  ; family/model/stepping
    }
}

void calculateMHz(void)
{
    unsigned int i, max;
    unsigned int startCount = 0;
    unsigned int endCount = 0;
    unsigned int cycleCount = 0;

    max = 0;
    for (i=0; i<10; i++)
    {
        getStartTime();
        __asm
        {
            RDTSC                 // Pentium op code to obtain instruction cycle count
            mov startCount, eax   
        }
    
        while (getRunSecs() < 0.01)
        {
            __asm
            {
                mov edx, 1000
              lp:
                dec edx            // misc instructions
                jnz lp
                RDTSC              // end instruction cycle count
                mov endCount, eax  
            }
        }
        cycleCount = endCount - startCount;
        megaHz = (unsigned int)((double)cycleCount / 1000000.0 / runSecs + 0.5);
        if (megaHz > max) max = megaHz;
    }
    megaHz = max;
}

void getStartTime(void)
{
    QueryPerformanceCounter(&astarttime);
}
 
double getRunSecs(void)
{
    LARGE_INTEGER liDiff;
    LARGE_INTEGER liFreq;

    QueryPerformanceCounter(&liDiff);

    liDiff.QuadPart -= astarttime.QuadPart;

    (void)QueryPerformanceFrequency(&liFreq);
    
    runSecs = (double)liDiff.QuadPart / (double) liFreq.QuadPart;
    return runSecs;
}



