/* 
 * MemSpd2K Benchmark Version 3.12 Copyright (c) Roy Longbottom 2004 
 *
 * The program employs three different sequences of operations, on 64 bit 
 * double precision floating point numbers, 32 bit single precision numbers 
 * and 32 bit integers via data arrays:
 *
 *               s=s+x[m]*y[m] (Integer +y[m])
 *               x[m]=x[m]+y[m]
 *               x[m]=y[m] 
 *
 * These are executed using Assembly code which uses the same instructions
 * as the original MemSpeed benchmark.
 *
 * The memory loading speed is calculated in terms of millions of bytes
 * per second (MB/S). Measurements are made at 4000, 8000, 1600 etc. memory
 * bytes up to 25% of the main RAM size to produce speed ratings via data
 * from different levels of cache and from RAM. Results are displayed as the
 * program is running and saved in file XferMBPS.txt which should appear in
 * the same directory as the EXE file.
 *
 * WARNING - The two arrays are allocated with addresses in multiples of
 * 1048576 bytes appart. This identifies a design limitation with the Intel
 * P4 CPU, producing false cache flushing and some very slow speeds.
 *
 *  Modules
 *
 *  Config.cpp - via WhatConfig() identifies CPU type/speed, memory size
 *  and OS version.
 *
 *  MemSpd2K.rc - menus and dialog boxes
 *     MENU_ABOUT    - AboutDlgProc()
 *     MENU_RUNALL   - runTests();
 *     MENU_DISPLAY  - displayWhatever()
 *     MENU_EXIT     - CleanUpAndPostQuit()
 *     DIALOG_8      - TestPanel()
 *     DIALOG_9      - RunPanel()
 *     AboutBox      - AboutDlgProc()
 *
 *  MemSpd2K.h and MemSpd2K.ico
 *
 *  MemSpd2K.CPP
 *
 *  Functions
 *
 *  WinMain() - usual Windows functions, InitApp()
 *  InitApp() - usual Windows functions, allocates variable values,
 *     calculates maximum memory to be tested, uses WhatConfig()
 *     and displayWhatever(), processes command line.
 *  WindowProc() - usual Windows functions for menus   
 *  AboutDlgProc() - displays About text
 *  CleanUpAndPostQuit() - set up exit
 *  TestPanel() - dialog box displayed at start with information
 *     including RAM size, two buttons:
 *         RUN_ALL as MENU_RUNALL
 *         EXIT_BUTTON as MENU_EXIT
 *     Displayed after tests showing results
 *  RunPanel() - dialog box cleared and refreshed after each test
 *      showing all results so far, no buttons
 *  displayWhatever() clears dialog boxes and displays TestPanel()
 *  runTests() - Opens log file, executes local_time(), logs date/time
 *  and headings, VirtualAlloc() maximum RAM, loop j for memory sizes
 *  running each\of the following tests using checkTime(), logs speed
 *  results, destroys/creates RunPanel() for each test, VirtualFree(),
 *  logs end time. 
 *     DPOne()    Assembly code s=s+x[m]*y[m] DPFP
 *     SPOne()    Assembly code s=s+x[m]*y[m] SPFP
 *     intOne()   Assembly code s=s+x[m]+y[m] Integers
 *     DPTwo()    Assembly code x[m]=x[m]+y[m] DPFP
 *     SPTwo()    Assembly code x[m]=x[m]+y[m] SPFP
 *     intTwo()   Assembly code x[m]=x[m]+y[m] Integers
 *     DPThree()  Assembly code x[m]=y[m] DPFP
 *     SPThree()  Assembly code x[m]=y[m] SPFP
 *     intThree() Assembly code x[m]=y[m] Integers
 *  all are called using start_time(), end_time() and checkTime()
 *  checkTime() - recalibrates jmax passes so tests run for at least
 *      runTime (0.1 seconds)
 *  local_time() - date/time of day
 *  start_time() - start time from high resolution timer
 *  end_time() - end time and running time secs
*/
#define NAME "Test"
#define TITLE " Test Memory"
#define INITGUID

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "MemSpd2k.h"

double * xd;
double * yd;
float  * xs;
float  * ys;
int   * xi;   
int   * yi;
double dsum[1];
float  ssum[1];
int   isum[1];

void DPOne(int kd)
{
    __asm
    {
       mov     edx, kd
       mov     eax, xd
       mov     ecx, yd
       lea     ebx, dsum
       mov     edi,0                  
       fld     qword ptr [ebx]        
   lp: fld     qword ptr [eax+edi]    
       fmul    qword ptr [ecx+edi]    
       fld     qword ptr [eax+edi+8]  
       fmul    qword ptr [ecx+edi+8]  
       fld     qword ptr [eax+edi+16] 
       fmul    qword ptr [ecx+edi+16] 
       fld     qword ptr [eax+edi+24] 
       fmul    qword ptr [ecx+edi+24] 
       add     edi,32               
       fxch    st(3)                     
       faddp   st(4),st                     
       fxch    st(1)                     
       faddp   st(3),st                
       faddp   st(2),st                
       faddp   st(1),st                
       sub     edx,4                
       jle     lq                    
       jmp     lp                   
   lq: fstp    qword ptr [ebx]
    }
}                         

     
void SPOne(int ks)
{
    __asm
    {
       mov     edx, ks
       mov     eax, xd
       mov     ecx, yd
       lea     ebx, ssum
       mov     edi,0                  
       fld     dword ptr [ebx]        
   lp: fld     dword ptr [eax+edi+4]  
       fmul    dword ptr [ecx+edi+4]  
       fld     dword ptr [eax+edi+8]  
       fmul    dword ptr [ecx+edi+8]  
       fld     dword ptr [eax+edi+12] 
       fmul    dword ptr [ecx+edi+12] 
       fld     dword ptr [eax+edi]    
       fmul    dword ptr [ecx+edi]    
       add     edi,16               
       faddp   st(4),st                     
       fxch    st(2)                     
       faddp   st(3),st                
       faddp   st(2),st                
       faddp   st(1),st                
       sub     edx,4                
       jg      lp                   
       fstp    dword ptr [ebx]
    }
}
         
void intOne(int kj)
{
    __asm
    {
       mov     edx, kj
       mov     eax, xd
       mov     ecx, yd
       lea     edi, isum
       mov     ebx,[edi]       
       mov     esi,0           
   lp: add     ebx,[eax+esi]   
       add     ebx,[ecx+esi]   
       add     ebx,[eax+esi+4] 
       add     ebx,[ecx+esi+4]    
       add     ebx,[eax+esi+8] 
       add     ebx,[ecx+esi+8]    
       add     ebx,[eax+esi+12]
       add     ebx,[ecx+esi+12]   
       add     esi,16          
       cmp     esi,edx         
       jl      lp              
       mov     [edi], ebx      
    }
}


void DPTwo(int kd)
{
    __asm
    {
       mov     edx, kd
       mov     eax, xd
       mov     ecx, yd
       mov     edi, 0
   lp: fld     qword ptr [eax+edi]    
       fld     qword ptr [eax+edi+8]  
       fld     qword ptr [eax+edi+16] 
       fld     qword ptr [eax+edi+24] 
       fxch    st(3)                  
       fadd    qword ptr [ecx+edi]    
       fxch    st(2)                  
       fadd    qword ptr [ecx+edi+8]  
       fxch    st(1)                  
       fadd    qword ptr [ecx+edi+16] 
       fxch    st(3)                  
       fadd    qword ptr [ecx+edi+24] 
       fxch    st(2)                  
       fstp    qword ptr [eax+edi]    
       fstp    qword ptr [eax+edi+8]  
       fxch    st(1)                  
       fstp    qword ptr [eax+edi+16] 
       fstp    qword ptr [eax+edi+24]  
       add     edi,32       
       sub     edx,4        
       jg      lp                        
    }
}


void SPTwo(int ks)
{
    __asm
    {
       mov     edx, ks
       mov     eax, xd
       mov     ecx, yd
       mov     edi, 0
   lp: fld     dword ptr [eax+edi]    
       fld     dword ptr [eax+edi+4]  
       fld     dword ptr [eax+edi+8]  
       fld     dword ptr [eax+edi+12] 
       fxch    st(3)                  
       fadd    dword ptr [ecx+edi]    
       fxch    st(2)                  
       fadd    dword ptr [ecx+edi+4]  
       fxch    st(1)                  
       fadd    dword ptr [ecx+edi+8]  
       fxch    st(3)                  
       fadd    dword ptr [ecx+edi+12] 
       fxch    st(2)                  
       fstp    dword ptr [eax+edi]    
       fstp    dword ptr [eax+edi+4]  
       fxch    st(1)                  
       fstp    dword ptr [eax+edi+8]  
       fstp    dword ptr [eax+edi+12]  
       add     edi,16       
       sub     edx,4        
       jg      lp
    }
}

void intTwo(int ki)
{
    __asm
    {
       mov     edx, ki
       mov     eax, xd
       mov     ecx, yd
   lp: mov     ebx,[eax]    
       mov     edi,[ecx]    
       add     ebx,edi      
       mov     [eax], ebx   
       mov     ebx,[ecx+4]  
       add     [eax+4],ebx  
       mov     ebx,[ecx+8]  
       add     [eax+8],ebx  
       mov     ebx,[ecx+12] 
       add     [eax+12],ebx        
       add     eax,16                   
       add     ecx,16       
       sub     edx,4        
       jg      lp                        
    }
}

void DPThree(int kd)
{
    __asm
    {
       mov     edx, kd
       mov     eax, yd
       mov     ecx, xd
       mov     edi, 0
   lp: fld     qword ptr [eax+edi]    
       fld     qword ptr [eax+edi+8]  
       fld     qword ptr [eax+edi+16] 
       fld     qword ptr [eax+edi+24] 
       fxch    st(3)                  
       fstp    qword ptr [ecx+edi]    
       fxch    st(1)                  
       fstp    qword ptr [ecx+edi+8]  
       fstp    qword ptr [ecx+edi+16] 
       fstp    qword ptr [ecx+edi+24]               
       add     edi,32       
       sub     edx,4        
       jg      lp
    }
}
 
void SPThree(int ks)
{
    __asm
    {
       mov     edx, ks
       mov     eax, yd
       mov     ecx, xd
       mov     edi, 0
   lp: fld     dword ptr [eax+edi]    
       fld     dword ptr [eax+edi+4]  
       fld     dword ptr [eax+edi+8]  
       fld     dword ptr [eax+edi+12] 
       fxch    st(3)                  
       fstp    dword ptr [ecx+edi]    
       fxch    st(1)                  
       fstp    dword ptr [ecx+edi+4]  
       fstp    dword ptr [ecx+edi+8]  
       fstp    dword ptr [ecx+edi+12]               
       add     edi,16       
       sub     edx,4        
       jg      lp                        
    }
}

void intThree(int kj)
{
    __asm
    {
       mov     edx, kj
       mov     eax, yd
       mov     ecx, xd
       mov     edi, 0
   lp: mov     ebx,[eax+edi]    
       mov     [ecx+edi],ebx    
       mov     ebx,[eax+edi+4]  
       mov     [ecx+edi+4],ebx  
       mov     ebx,[eax+edi+8]  
       mov     [ecx+edi+8],ebx  
       mov     ebx,[eax+edi+12] 
       mov     [ecx+edi+12],ebx                     
       add     edi,16
       sub     edx,4          
       jg      lp                            
    }
}


//  ************** PROTOPTYPES *****************

BOOL FAR PASCAL AboutDlgProc( HWND, UINT, WPARAM, LPARAM);
static BOOL InitApp(HINSTANCE, LPSTR, int);
long FAR PASCAL WindowProc(HWND, UINT, WPARAM, LPARAM);                  
static void CleanUpAndPostQuit(void);
void displayWhatever(HWND);

BOOL FAR PASCAL TestPanel( HWND, UINT, WPARAM, LPARAM);
BOOL FAR PASCAL RunPanel( HWND, UINT, WPARAM, LPARAM);

void local_time();
void end_time();
void start_time();
void checkTime();
BOOL runTests();


// ************* GLOBAL VARIABLES **********
 
char   version[30] = "Version 3.12";
char    timeday[30];
LARGE_INTEGER starttime;
double  secs;
int     n1;
double  runTime = 0.1;

BOOL bQuit = FALSE;

HWND   mainHWND;
HWND   testHwnd = NULL;
HWND   runHwnd = NULL;
HINSTANCE MyInstance;
char    windowText[256];
char    commandLine[1024] = "";

short     keyPress;
BOOL  autoRun = FALSE;
BOOL  doTesting = FALSE;
char   writeMsg[256];
double results[30][9];
double mips[30][5];

int    memUsed[100];
int  passesDone;
BOOL  runDone = FALSE;
int  ramKBused;
int  ramKB;
int runs = 10; // 2 MB;
int vector[20];
int allocMem[20];
RECT     winRect;
int     screenWide;
int     screenHigh;
double  DPMflops;
double  SPMflops;
int    maxrows = 13;
int    userows;
double hmfp, hmint;
double hmfp2, hmint2;
double sdfp, sdint;
double sdfp2, sdint2;
char  startRam[200];


void displayWhatever(HWND hwnd)
{
    sprintf(windowText, " MemSpeed Memory Test");
    SetWindowText(mainHWND, windowText);        
        
    if (doTesting)
    {
        if (testHwnd)   DestroyWindow(testHwnd);
    }
    else
    {    
        if (runHwnd)    DestroyWindow(runHwnd);
        testHwnd = CreateDialog( MyInstance,"DIALOG_8", hwnd, (DLGPROC)TestPanel);
    }  
} // displayWhatever


// WinMain - initialization, message loop

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR cmdline, int cmdshow)
{
    MSG         msg;
    
    hPrevInstance = hPrevInstance;

    if (!InitApp(hInstance, cmdline, cmdshow))
         return FALSE;

    if (autoRun)
    {
         runTests();
         CleanUpAndPostQuit();
    }        
    while(!bQuit)
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            
           if (msg.message == WM_QUIT)
           {
                CleanUpAndPostQuit();
                break;
           }
   
           if( !GetMessage( &msg, NULL, 0, 0 ) ) return msg.wParam;
           TranslateMessage(&msg);
           DispatchMessage(&msg);
        }
        else
        {
           // make sure we go to sleep if we have nothing else to do
           WaitMessage();
        }
    }
    return msg.wParam;
    
} // WinMain

// ***************************************************************************
//                    Initialization and object creation                     *
// ***************************************************************************

// InitApp
// Creates window

static BOOL
InitApp(HINSTANCE hInstance, LPSTR cmdline, int cmdshow)
{
    HWND hwnd;
    WNDCLASS wc;
    int   mult = 20;
    int   i, j;
    char *delims = {" ,"};
    char commands[20][256];
    char *p;
    int  KBytes;
    char buf[256] = "";
    HDC  hdc;
    int msecs = 0;
  
    
    // set up and registers the window class
    
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(DWORD);
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, "GenericIcon");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = "GenericMenu";
    wc.lpszClassName = NAME;
    if (!RegisterClass(&wc))
        return FALSE;

    // Create the window

    hwnd =
        CreateWindow(
            NAME,
            TITLE,
            WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
            0,
            0,
            640,
            480,
            NULL, 
            NULL, 
            hInstance, 
            NULL 
        );

    if (!hwnd) return FALSE;

    mainHWND = hwnd;
    strcpy(commandLine, strupr(cmdline));

    hdc = GetDC(mainHWND);
    screenWide = GetDeviceCaps(hdc, HORZRES);
    screenHigh = GetDeviceCaps(hdc, VERTRES);   
    ReleaseDC(mainHWND, hdc);

    winRect.left = 0;
    winRect.top = 0;
    winRect.right = screenWide;
    winRect.bottom = screenHigh;

    WhatConfig();
    strcpy(startRam, configData3);
    ramKB = ramsize / 1024;

    allocMem[0] = 2048;  // two arrays
    for (i=1; i<19; i++)
    {
        allocMem[i] = allocMem[i-1] * 2;
    }
    vector[0] = 250;       //      4 KB
    vector[1] = 500;       //      8 KB
    vector[2] = 1000;      //     16 KB
    vector[3] = 2000;      //     32 KB
    vector[4] = 4000;      //     64 KB
    vector[5] = 8000;      //    128 KB
    vector[6] = 16000;     //    256 KB
    vector[7] = 32000;     //    512 KB
    vector[8] = 64000;     //    1 MB
    vector[9] = 128000;    //    2 MB
    vector[10] = 256000;   //    4 MB
    vector[11] = 512000;   //    8 MB
    vector[12] = 1024000;  //   16 MB
    vector[13] = 2048000;  //   32 MB
    vector[14] = 4096000;  //   64 MB
    vector[15] = 8192000;  //  128 MB
    vector[16] = 16384000; //  256 MB
    vector[17] = 32768000; //  512 MB
    vector[18] = 65536000; // 1024 MB

    if (ramKB >   14000) runs = 11;
    if (ramKB >   30000) runs = 12;
    if (ramKB >   60000) runs = 13;
    if (ramKB >  120000) runs = 14;
    if (ramKB >  250000) runs = 15;
    if (ramKB >  500000) runs = 16;
    if (ramKB > 1000000) runs = 17;
    if (ramKB > 2000000) runs = 18;
    if (ramKB > 4000000) runs = 19;


    i = 0;
    p = strtok(commandLine, delims);
    while (p != NULL)
    {
        strcpy(commands[i], p);
        strcat(buf, commands[i]);
        strcat(buf, " ");
        i = i + 1;
        if (i > 4) break;
        p = strtok(NULL, delims);
    }
    if (i > 0)
    {
        strcpy(commandLine, buf);
    }
    else
    {
        strcpy(commandLine, "None");
    }
    autoRun = FALSE;
    KBytes = 0;
       
    for (j=0; j<i; j++)
    {
        switch(commands[j][0])
        {            
            case 'A': // Auto Run
             autoRun = TRUE;
             break;            

            case 'K': // KBytes
             if (j < i -1)
             {
                 sscanf(commands[j+1], "%d", &KBytes);
             }
             break;            
       
            case 'M': // millisecs
             if (j < i -1)
             {
                 sscanf(commands[j+1], "%d", &msecs);
             }
             break;
        }            
    }
    if (KBytes > 0)
    {
        runs = 1;
        j = 8;
        for (i=0; i<18; i++)
        {
            if (KBytes > j - 1) runs = runs + 1;
            j = j * 2; 
        }
    }
    if (msecs > 0) runTime = ((double) msecs) / 1000;


    ramKBused = vector[runs-1] * 16 / 1000;

    keyPress = GetAsyncKeyState(VK_ESCAPE);

    // Display the window
    
    ShowWindow(hwnd, cmdshow);    
    UpdateWindow(hwnd);

    if (!autoRun) displayWhatever(mainHWND);
    
    return TRUE;
    
}  // InitApp



// WindowProc - handle messages for the main application window


long FAR PASCAL WindowProc(HWND hwnd, UINT msg,
                            WPARAM wparam, LPARAM lparam)                  
{
           
    switch( msg )
    {
    case WM_ACTIVATEAPP:
        break;
        
    case WM_INITMENUPOPUP:
        break;
        
    case WM_CREATE:
        break;

    case WM_DESTROY:
        CleanUpAndPostQuit();
        break;

    case WM_COMMAND:
        switch( LOWORD( wparam ) )
        {
        case MENU_ABOUT:
            if (testHwnd)   DestroyWindow(testHwnd);
            DialogBox( MyInstance,"AboutBox", hwnd, (DLGPROC)AboutDlgProc );
            displayWhatever(mainHWND);                                      
            break;

       case MENU_RUNALL:
            doTesting = TRUE;
            displayWhatever(mainHWND);                                      
            runTests();
            break;


        case MENU_DISPLAY:
            displayWhatever(mainHWND);                                      
            break;
        
        case MENU_EXIT:
            CleanUpAndPostQuit();
            break;
                
        }
        break;
   
        case WM_PAINT:
             break;
           
        case WM_KEYDOWN:
            break;

       case WM_ACTIVATE:
            break;
    } 
    return DefWindowProc( hwnd, msg, wparam, lparam );

} // WindowProc




// AboutDlgProc - processes messages for the about dialog.

BOOL FAR PASCAL AboutDlgProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    lparam = lparam;

    switch( msg ) {
    case WM_INITDIALOG:
        return( TRUE );

    case WM_COMMAND:
        if( LOWORD( wparam ) == IDOK ) {
            EndDialog( hwnd, TRUE );
            return( TRUE );
        }
        break;
    }
    return( FALSE );

} // AboutDlgProc



  
void local_time()
{
    time_t time_of_day;
    time_of_day = time( NULL ); 
    strcpy (timeday, ctime(&time_of_day)); // timeday = date and time
    return;
}




// CleanUpAndPostQuit
// Release all D3DRM objects, post a quit message and set the bQuit flag

void
CleanUpAndPostQuit(void)
{
    bQuit = TRUE;
    return;
}


BOOL FAR PASCAL RunPanel( HWND hdlg, UINT msg,
                                WPARAM wparam, LPARAM lparam )
{
    wparam = wparam;
    lparam = lparam;
    hdlg = hdlg;
    int  k;
    int FAR  tabs[10] = {1, 30, 60, 90, 120, 150, 180, 210, 240, 270};

     switch( msg )
     {
       case WM_INITDIALOG:
       
        SendDlgItemMessage(hdlg, RESULTS1, LB_SETTABSTOPS, (WPARAM) 10, (LPARAM) (int FAR*) tabs); 
        sprintf(writeMsg, "Maximum RAM size to be tested %d KB", ramKBused);
        SendDlgItemMessage(hdlg, NOTES1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        sprintf(writeMsg, "Flashing disk light suggest insufficient RAM and paging activity "
                      "to/from RAM and disk.");
        SendDlgItemMessage(hdlg, NOTES1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        sprintf(writeMsg, "Press ESC to cancel and exit program. It might take some time.");
        SendDlgItemMessage(hdlg, NOTES1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        
        if (runDone)
        {
            SendDlgItemMessage(hdlg, RESULTS1, LB_SETHORIZONTALEXTENT, 700, 0L);
            sprintf(writeMsg, " Memory Speed Test %s by Roy Longbottom"
                              "%10.3f seconds minimum per test", version, runTime);
            SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, "Memory%c  s=s+x[m]*y[m] Int+ %c  x[m]=x[m]+y[m]%c%c  x[m]=y[m]", 9, 9, 9, 9); 

            SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, "KBytes%c Dble%c Sngl%c  Int%c Dble%c Sngl%c  Int%c Dble%c Sngl%c  Int",
                                     9, 9, 9, 9, 9, 9, 9, 9, 9);   
            SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, "  Used%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S",
                                     9, 9, 9, 9, 9, 9, 9, 9, 9);           
            SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);

            for (k=0; k<passesDone; k++)
            {
               sprintf(writeMsg, "%6d %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f",  
                                memUsed[k]/1000, 9, results[k][0], 9, results[k][3], 9, results[k][6],
                                                 9, results[k][1], 9, results[k][4], 9, results[k][7],
                                                 9, results[k][2], 9, results[k][5], 9, results[k][8]);
               SendDlgItemMessage(hdlg, RESULTS1, LB_ADDSTRING, 0, (LPARAM) writeMsg);
               SendDlgItemMessage(hdlg, RESULTS1, LB_SETCURSEL, k+6, 0L);
            }
        }
        return( TRUE );              
     }
     return( FALSE );
    
} // runPanel

BOOL FAR PASCAL TestPanel( HWND hdlg, UINT msg,
                                WPARAM wparam, LPARAM lparam )
{
    wparam = wparam;
    lparam = lparam;
    hdlg = hdlg;
    int  k, i;
    int FAR  tabs[10] = {1, 30, 60, 90, 120, 150, 180, 210, 240, 270};

     switch( msg )
     {
       case WM_INITDIALOG:
        SendDlgItemMessage(hdlg, RESULTS, LB_SETTABSTOPS, (WPARAM) 10, (LPARAM) (int FAR*) tabs); 
        sprintf(writeMsg, "Maximum RAM size to be tested %d KB at %10.3f seconds per test.",
                                    ramKBused, runTime);
        SendDlgItemMessage(hdlg, NOTES, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        sprintf(writeMsg, "RAM size from GlobalMemoryStatus %d KB", ramKB);
        SendDlgItemMessage(hdlg, NOTES, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        sprintf(writeMsg, "After running, results should be in file XferMBPS.txt and above");
        SendDlgItemMessage(hdlg, NOTES, LB_ADDSTRING, 0, (LPARAM) writeMsg);
       
        if (runDone)
        {
            SendDlgItemMessage(hdlg, RESULTS, LB_SETHORIZONTALEXTENT, 700, 0L);
            sprintf(writeMsg, " Memory Speed Test %s by Roy Longbottom"
                              "%10.3f seconds minimum per test", version, runTime);
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);

            sprintf(writeMsg, "Memory%c  s=s+x[m]*y[m] Int+ %c  x[m]=x[m]+y[m]%c%c  x[m]=y[m]", 9, 9, 9, 9); 
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, "KBytes%c Dble%c Sngl%c  Int%c Dble%c Sngl%c  Int%c Dble%c Sngl%c  Int",
                                     9, 9, 9, 9, 9, 9, 9, 9, 9);   
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, "  Used%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S%c MB/S",
                                     9, 9, 9, 9, 9, 9, 9, 9, 9);           
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);

            for (k=0; k<passesDone; k++)
            {
               sprintf(writeMsg, "%6d %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f",  
                                memUsed[k]/1000, 9, results[k][0], 9, results[k][3], 9, results[k][6],
                                                 9, results[k][1], 9, results[k][4], 9, results[k][7],
                                                 9, results[k][2], 9, results[k][5], 9, results[k][8]);
               SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            }
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            if (passesDone > 9)
            {

                sprintf(writeMsg, " Roy's Memory Ratio for CPUSpeed Tables");
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
                sprintf(writeMsg, " ");
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);            
                sprintf(writeMsg, "%c%cInteger %c%cFloating Point", 9, 9, 9, 9);
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);            
                sprintf(writeMsg, "%c%cAverage %cStdDev %cAverage %cStdDev", 9, 9, 9, 9, 9);
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
                sprintf(writeMsg, " Original 2MB  %c%7.0f %c%7.0f %c%7.0f %c%7.0f",
                                   9, hmint, 9, sdint, 9, hmfp, 9, sdfp);
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        
                if (userows == maxrows)
                {
        
                    sprintf(writeMsg, " New one 16MB %c%7.0f %c%7.0f %c%7.0f %c%7.0f",
                                       9, hmint2, 9, sdint2, 9, hmfp2, 9, sdfp2);
                    SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
                }
                sprintf(writeMsg, " ");
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);            
            }
            sprintf(writeMsg, " Millions of Instructions Per Second Integer Tests");
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, "MemKB%cr+m*m %c m=m+m%c   m=m %cAverage %c  Ratio",
                                     9, 9, 9, 9, 9);
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            for (i=0; i<userows; i++)
            {
                sprintf(writeMsg, "%7.d %c%7.0f %c%7.0f %c%7.0f %c%7.0f %c%7.0f",
                              memUsed[i]/1000, 9, mips[i][0], 9, mips[i][1],
                                                  9, mips[i][2], 9, mips[i][3],
                                                  9, mips[i][4]);
                SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            }
            sprintf(writeMsg, " ");
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            sprintf(writeMsg, " Maximum MFLOPS %7.0f DP, %7.0f SP",
                             DPMflops * 8 / 64, SPMflops * 8 / 32);
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
            SendDlgItemMessage(hdlg, RESULTS, LB_SETCURSEL, passesDone+11, 0L);

        }
        else
        {
            strcpy(writeMsg, "Batch Command ");
            strncat(writeMsg, commandLine, 200);
            SendDlgItemMessage(hdlg, RESULTS, LB_ADDSTRING, 0, (LPARAM) writeMsg);
        }
        return( TRUE );
                    
       case WM_COMMAND:

        if( HIWORD( wparam ) == BN_CLICKED)
        {
            switch (LOWORD (wparam))
            {
                case RUN_ALL:
                  PostMessage(mainHWND, WM_COMMAND, MENU_RUNALL, 0);
                 break;

                case EXIT_BUTTON:
                 PostMessage(mainHWND, WM_COMMAND, MENU_EXIT, 0);
                 break;
            }
        }
        return( TRUE );         
     }
     return( FALSE );
    
} // TestPanel


void start_time()
{
    QueryPerformanceCounter(&starttime);
}
 
void end_time()
{
    LARGE_INTEGER liDiff;
    LARGE_INTEGER liFreq;

    QueryPerformanceCounter(&liDiff);

    liDiff.QuadPart -= starttime.QuadPart;

    (void)QueryPerformanceFrequency(&liFreq);
    
    secs = (double)liDiff.QuadPart / (double) liFreq.QuadPart;
}




BOOL runTests()
{
    float *xs = (float *) xd;
    float *ys = (float *) yd;
    int   *xi = (int *) xd;
    int   *yi = (int *) yd;     
    
    int useMem;   
    float  sums;
    float  ones = 1.0;
    float  zeros = 0;
    int i, m, j, nn, mem;
    int kd, ks, ki;
    int   sumi;
    int  zeroi = 0;
    int onei = 1;             
    int inc;
    int opsd, opss, opsi;
    double sumd;
    double oned = 1.0;
    double zerod = 0;
    HDC  hdc;
    double sumx, sumxsq;
    double count;
    double ratio[13][9];
    double r100[13] = {45, 50, 33, 32, 30, 28, 23, 19, 18, 17, 17, 17, 17};
    double p100[13][9] =
        {
            198, 269, 103, 102, 145, 51, 119, 143, 52,
            220, 296, 113, 122, 149, 53, 145, 156, 52,
            105, 111, 94, 76, 85, 46, 87, 84, 46,
            104, 109, 60, 75, 84, 38, 85, 88, 39,
            100, 96, 60, 74, 77, 37, 84, 78, 38,
            90, 82, 58, 68, 67, 35, 77, 66, 37,
            74, 63, 53, 57, 54, 32, 63, 53, 33,
            60, 50, 41, 49, 44, 26, 54, 45, 27,
            57, 47, 33, 47, 42, 22, 51, 42, 23,
            46, 46, 31, 46, 41, 21, 50, 42, 21,
            46, 46, 31, 46, 41, 21, 50, 42, 21,
            46, 46, 31, 46, 41, 21, 50, 42, 21,
            46, 46, 31, 46, 41, 21, 50, 42, 21
       };
        
    FILE    *outfile;

    for (i=0; i<30; i++)
    {
        for (j=0; j<9; j++)
        {
            results[i][j] = 0;
        }
    }

    useMem = allocMem[runs - 1]; // vector[runs - 1] * 8 + 8;    

    hdc = GetDC(mainHWND);
    sprintf(writeMsg, "                          Allocating Memory  ");
    TextOut(hdc, 100, 100, writeMsg, lstrlen(writeMsg));    
    ReleaseDC(mainHWND, hdc);

    outfile = fopen("XferMBPS.txt","a+");
    if (outfile == NULL)
    {
        sprintf(writeMsg, "Cannot open results file");
        if (!autoRun) MessageBox(NULL, writeMsg, "ERROR WILL EXIT", MB_ICONWARNING | MB_OK);
        CleanUpAndPostQuit();
        return FALSE;
    }

    local_time();
    
    fprintf(outfile, "\n      Memory Reading Speed Test %s by Roy Longbottom\n", version);
    fprintf(outfile, "\n %10.3f seconds per test, Start %s\n", runTime, timeday);
    fprintf(outfile, "  Memory    s=s+x[m]*y[m] Int+     x[m]=x[m]+y[m]         x[m]=y[m]\n"); 
    fflush(outfile);

    xd = (double *)VirtualAlloc(NULL, useMem, MEM_COMMIT, PAGE_READWRITE);
    if (xd == NULL)
    {
        sprintf(writeMsg, "Cannot allocate memory");
        if (!autoRun) MessageBox(NULL, writeMsg, "ERROR WILL EXIT", MB_ICONWARNING | MB_OK);
        fprintf(outfile,"Cannot allocate memory\n\n");
        fflush(outfile);
        fclose(outfile);
        CleanUpAndPostQuit();
        return FALSE;
    }        
    yd = (double *)VirtualAlloc(NULL, useMem, MEM_COMMIT, PAGE_READWRITE);
    if (yd == NULL)
    {
        sprintf(writeMsg, "Cannot allocate memory");
        if (!autoRun) MessageBox(NULL, writeMsg, "ERROR WILL EXIT", MB_ICONWARNING | MB_OK);
        fprintf(outfile,"Cannot allocate memory\n\n");
        fflush(outfile);
        fclose(outfile);
        VirtualFree(xd, 0, MEM_RELEASE);
        CleanUpAndPostQuit();
        return FALSE;
    }        

    fprintf(outfile, "  KBytes    Dble   Sngl   Int    Dble   Sngl   Int    Dble   Sngl   Int\n");   
    fflush(outfile);

    xs = (float  * )xd;
    ys = (float  * )yd;
    xi = (int    * )xd;   
    yi = (int    * )yd;

    runDone = TRUE;    
    passesDone = 0;
 
    sprintf(writeMsg, "                           Running Tests                    ");

    hdc = GetDC(mainHWND);
    TextOut(hdc, 100, 100, writeMsg, lstrlen(writeMsg));    
    ReleaseDC(mainHWND, hdc);
    GdiFlush();

    if (runHwnd)    DestroyWindow(runHwnd);
    runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);

    fprintf(outfile, "   Used     MB/S   MB/S   MB/S   MB/S   MB/S   MB/S   MB/S   MB/S   MB/S\n\n");
    fflush(outfile);

    DPMflops = 0;
    SPMflops =0;
    for (j=0; j<runs; j++)
    {
        kd = vector[j];
        nn = 6400000 / kd;
        if (nn < 1) nn = 1;
        
        ks = kd * 2;
        ki = kd * 2;
        
        mem = kd * 16;
        memUsed[j] = mem;
        
        inc = 4;

        // DPOne *******************************  

        n1 = nn;
  
        do
        {
            sumd = 0;
            dsum[0] = 0;
            for (m=0; m<kd; m++)
            {
                  xd[m] = oned;
                  yd[m] = oned;
            }          
    
            start_time();

            for (i=0; i<n1; i++)
            {
                 DPOne(kd);
            }
            end_time();
            checkTime();
        }
        while (secs < runTime);

        sumd = dsum[0];
        opsd = 16;             
        results[j][0] = (double)kd * (double)n1 * opsd / 1000000/ secs;
        if (results[j][0] > DPMflops) DPMflops = results[j][0];
        passesDone = j + 1;
        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
    
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

        //  SPOne ******************************* 

        n1 = nn;
        do
          {
            ssum[0] = 0;
            for (m=0; m<ks; m++)
              {
                  xs[m] = ones;
                  ys[m] = ones;
              }          
            start_time();
            
            for (i=0; i<n1; i++)
              {
                 SPOne(ks);
              }
            end_time();
            checkTime();            
          }
        while (secs < runTime);

        sums = ssum[0];        
        opss = 8;
        results[j][3] = (double)ks * (double)n1 * opss / 1000000 / secs;
        if (results[j][3] > SPMflops) SPMflops = results[j][3];
        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
    
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

       //  intOne ******************************* 

       n1 = nn;
       do
          {
            isum[0] = 0;
            for (m=0; m<ki; m++)
              {
                  xi[m] = zeroi;
                  yi[m] = zeroi;
              }          
            yi[ki-1] = onei;
                    
            start_time();
            for (i=0; i<n1; i++)
              {
                 intOne(ki*4);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);

        sumi = isum[0];
        opsi = 8;
        results[j][6] = (double)ki * (double)n1 * opsi / 1000000 / secs;
        mips[j][0] = results[j][6] * 11 / 32;
        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
    
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

        //  DPTwo ******************************* 
        
        n1 = nn;
        do
          {
            for (m=0; m<kd; m++)
              {
                  xd[m] = zerod;
                  yd[m] = oned;
              }          
            start_time();
            for (i=0; i<n1; i++)
              {
                 DPTwo(kd);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);
    
        sumd = xd[1];
        opsd = 16;
        results[j][1] = (double)kd * (double)n1 * opsd / 1000000 / secs;
        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
    
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

        //  SPTwo ******************************* 

        n1 = nn;
        do
          {
            for (m=0; m<ks; m++)
              {
                  xs[m] = zeros;
                  ys[m] = ones;
              }          
            start_time();
            for (i=0; i<n1; i++)
              {
                 SPTwo(ks);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);
            
        sums = xs[1];
        opss = 8;
        results[j][4] = (double)ks * (double)n1 * opss / 1000000 / secs;
        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
    
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

       //  intTwo ******************************* 

        n1 = nn;
        do
          {
                    
            for (m=0; m<ki; m++)
            {
                  xi[m] = zeroi;
                  yi[m] = onei;
            }          

            start_time();
            for (i=0; i<n1; i++)
              {
                 intTwo(ki);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);
     
        sumi = xi[1];
        opsi = 8;
        results[j][7] = (double)ki * (double)n1 * opsi / 1000000 / secs;
        mips[j][1] = results[j][7] * 14 / 32;

        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

        //  DPThree ******************************* 

        n1 = nn;
        do
          {              
             for (m=0; m<kd+4; m++)
              {
                  xd[m] = zerod;
                  yd[m] = m;
              }        
            start_time();
            for (i=0; i<n1; i++)
              {
                 DPThree(kd);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);
            
        sumd = xd[kd-1]+1;
        opsd = 8;
        results[j][2] = (double)kd * (double)n1 * opsd / 1000000 / secs;

        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

        //  SPThree ******************************* 

        n1 = nn;
        do
          {
             for (m=0; m<ks+inc; m++)
              {
                  xs[m] = zeros;
                  ys[m] = (float)m;
              }        
            start_time();
            for (i=0; i<n1; i++)
              {
                 SPThree(ks);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);
        
        sums = xs[ks-1]+1;
        opss = 4;
        results[j][5] = (double)ks * (double)n1 * opss / 1000000 / secs;

        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

       //  intThree ******************************* 

        n1 = nn;
        do
          {
            for (m=0; m<ki+inc; m++)
              {
                  xi[m] = zeroi;
                  yi[m] = m;
              }            
            start_time();
            for (i=0; i<n1; i++)
              {
                 intThree(ki);
              }
            end_time();
            checkTime();
          }
        while (secs < runTime);

        sumi = xi[ki-1]+1;
        opsi = 4;        
        results[j][8] = (double)ki * (double)n1 * opsi / 1000000 / secs;
        mips[j][2] = results[j][8] * 12 / 16;

        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);
        if (GetAsyncKeyState(VK_ESCAPE) != keyPress)
        {
            fprintf(outfile, "\n\n\n");
            VirtualFree(xd, 0, MEM_RELEASE);
            VirtualFree(yd, 0, MEM_RELEASE);
            fflush(outfile);
            fclose(outfile);
            CleanUpAndPostQuit();
            return FALSE;
        }

        if (runHwnd)    DestroyWindow(runHwnd);
        runHwnd = CreateDialog( MyInstance,"DIALOG_9", mainHWND, (DLGPROC)RunPanel);

        fprintf(outfile, "%7d  %7.0f%7.0f%7.0f", mem /1000,
                                              results[j][0], results[j][3], results[j][6]);
        fprintf(outfile, "%7.0f%7.0f%7.0f",   results[j][1], results[j][4], results[j][7]);
        fprintf(outfile, "%7.0f%7.0f%7.0f\n", results[j][2], results[j][5], results[j][8]);
        fflush(outfile);
    }
    local_time();
    if (autoRun) DestroyWindow(runHwnd);

    fprintf(outfile, "\n                End of test %s", timeday);        
    fprintf(outfile,"\n\n");

    if (passesDone > 9)
    {
        userows = passesDone;
        if (userows > maxrows) userows = maxrows;
    
        for (i=0; i<userows; i++)
        {
            for (j=0; j<9; j++)
            {
                ratio[i][j] = 100.0 * results[i][j] / p100[i][j];
            }
        }
        hmfp = 0;
        sumx = 0;
        sumxsq = 0;
        for (i=0; i<10; i++)
        {
            for (j=0; j<6; j++)
            {
                hmfp = hmfp + 1 / ratio[i][j];
                sumx = sumx + ratio[i][j];
                sumxsq = sumxsq + pow(ratio[i][j], 2.0);
            } 
        }
        hmfp = 60.0 / hmfp;
        sumx = pow(sumx, 2.0);
        sumxsq = sumxsq * 60.0;
        sdfp = (sumxsq - sumx) / (60.0 * 59.0);
        sdfp = sqrt(sdfp);

        hmint = 0;
        sumx = 0;
        sumxsq = 0;
        for (i=0; i<10; i++)
        {
            for (j=6; j<9; j++)
            {
                hmint = hmint + 1 / ratio[i][j];
                sumx = sumx + ratio[i][j];
                sumxsq = sumxsq + pow(ratio[i][j], 2.0);
            } 
        }
        hmint = 30.0 / hmint;
        sumx = pow(sumx, 2.0);
        sumxsq = sumxsq * 30.0;
        sdint = (sumxsq - sumx) / (30.0 * 29.0);
        sdint = sqrt(sdint);

         fprintf(outfile, " **************** Roy's Memory Ratio ***************\n");
         fprintf(outfile, " Plus cache and RAM speeds to update CPUSpeed tables\n\n");
         fprintf(outfile, "                       Integer        Floating Point\n");
         fprintf(outfile, "                   Average  StdDev   Average  StdDev\n\n");
         fprintf(outfile, " Original to 2MB %7.0f %7.0f   %7.0f %7.0f\n",
                            hmint, sdint, hmfp, sdfp);

         if (userows == maxrows)
         {
            hmfp2 = 0;
            sumx = 0;
            sumxsq = 0;
            count = userows * 6;
            for (i=0; i<userows; i++)
            {
                for (j=0; j<6; j++)
                {
                    hmfp2 = hmfp2 + 1 / ratio[i][j];
                    sumx = sumx + ratio[i][j];
                    sumxsq = sumxsq + pow(ratio[i][j], 2.0);
                } 
            }
            hmfp2 = count / hmfp2;
            sumx = pow(sumx, 2.0);
            sumxsq = sumxsq * count;
            sdfp2 = (sumxsq - sumx) / (count * (count - 1));
            sdfp2 = sqrt(sdfp2);
    
            hmint2 = 0;
            sumx = 0;
            sumxsq = 0;
            count = userows * 3;
            for (i=0; i<userows; i++)
            {
                for (j=6; j<9; j++)
                {
                    hmint2 = hmint2 + 1 / ratio[i][j];
                    sumx = sumx + ratio[i][j];
                    sumxsq = sumxsq + pow(ratio[i][j], 2.0);
                } 
            }
            hmint2 = count / hmint2;
            sumx = pow(sumx, 2.0);
            sumxsq = sumxsq * count;
            sdint2 = (sumxsq - sumx) / (count * (count - 1));
            sdint2 = sqrt(sdint2);
            fprintf(outfile, " New one to 16MB %7.0f %7.0f   %7.0f %7.0f\n",
                               hmint2, sdint2, hmfp2, sdfp2);
         }
         fprintf(outfile,"\n\n");        
    }
    userows = passesDone;
    if (userows > maxrows) userows = maxrows;

    fprintf(outfile, " Millions of Instructions Per Second Integer Tests\n\n");
    fprintf(outfile, "  MemKB  r=r+m+m   m=m+m    m=m    Average  Ratio\n\n");
    for (i=0; i<userows; i++)
    {
        mips[i][3] = 3 / (1 / mips[i][0] + 1 / mips[i][1] + 1 / mips[i][2]);
        mips[i][4] = 100 * mips[i][3] / r100[i];
        fprintf(outfile, "%7.d %7.0f %7.0f %7.0f  %7.0f %7.0f \n",
                          memUsed[i]/1000, mips[i][0], mips[i][1],
                                           mips[i][2], mips[i][3],
                                           mips[i][4]);
    }
    fprintf(outfile,"\n\n");
    fprintf(outfile," Maximum MFLOPS %7.0f DP, %7.0f SP\n\n",
                     DPMflops * 8 / 64, SPMflops * 8 / 32);

    WhatConfig();
    fprintf(outfile,"%s\n", configData1);
    fprintf(outfile,"%s\n", configData2);
    fprintf(outfile,"%s, at end %d KB\n", startRam, freesize / 1024);        
    fprintf(outfile,"\n\n");

    fflush(outfile);
    fclose(outfile);
    VirtualFree(xd, 0, MEM_RELEASE);
    VirtualFree(yd, 0, MEM_RELEASE);
    doTesting = FALSE;
    if (!autoRun) displayWhatever(mainHWND);                                      

    hdc = GetDC(mainHWND);
    Rectangle(hdc, winRect.left, winRect.top,
                   winRect.right, winRect.bottom);
    ReleaseDC(mainHWND, hdc);

    return TRUE;
    
} // runTests


void checkTime()
{
    if (secs < runTime)
    {
          if (secs < runTime / 8.0)
          {
                n1 = n1 * 10;
          }
          else
          {
                n1 = (int)(runTime * 1.25 / secs * (double)n1+1);
           }
      }
}



         
