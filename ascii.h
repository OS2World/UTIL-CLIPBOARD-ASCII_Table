// ASCII Table 1.0
// Copyright 1998 by D.J. van Enckevort
// This program and sourcecode is released as freeware
// You can use this code for your own programs
// If you use it, please keep a notice of this copyright

// Resource IDs
// Resource IDs for the main window
#define DLG_ASCII                   100
#define SET_ASCII                   101
#define MLE_TEXT                    102
#define ST_FONT                     106
// Resource IDs for the About messagebox
#define DLG_ABOUT                   200
#define ST_TITLE                    201
#define ST_COPYRIGHT                202
#define ST_AUTHOR                   203
#define ST_EMAIL                    204
#define PB_OK                       205
// Settings dialog
#define DLG_SETTINGS                300
#define SB_START                    301
#define SB_END                      302
#define ST_START                    303
#define ST_END                      304
#define GB_RANGE                    305
#define PB_OK                       306
#define PB_CANCEL                   307
// Menu items
#define ID_MENU                     400
#define MN_FILE                     410
#define MI_OPEN                     411
#define MI_SAVE                     412
#define MI_SAVEAS                   413
#define MI_SETTINGS                 414
#define MI_CLOSE                    415
#define MN_EDIT                     420
#define MI_CUT                      421
#define MI_COPY                     422
#define MI_PASTE                    423
#define MI_CLEAR                    424
#define MI_FONT                     425
#define MI_CODEPAGE                 426
#define MI_MLE                      427
#define MI_DIRECTTYPE               428
#define MN_HELP                     430
#define MI_PRODUCTINFO              431
// AccelTable
#define ID_ACCELTABLE               500
// Icon
#define ID_ICON                     600
// HelpTable & HelpSubTable
#define ID_HELPTABLE                700
#define SUBTABLE_ASCII              710
#define SUBTABLE_ABOUT              720
// Direct Type Dialog
#define DLG_DIRECTTYPE              800
#define LB_PROGRAMS                 801
#define PB_DIRECTTYPE_OK            802
#define PB_DIRECTTYPE_CANCEL        803
#define PB_DIRECTTYPE_HELP          804
#define ST_DIRECTTYPE               805
// Help resources
#define HLP_MAIN                   1000
#define HLP_ABOUT                  3000
#define HLP_ASCII                  1100
#define HLP_TEXT                   1200
#define HLP_FILE                   1300
#define HLP_OPEN                   1310
#define HLP_SAVE                   1320
#define HLP_SAVEAS                 1330
#define HLP_CLOSE                  1340
#define HLP_EDIT                   1400
#define HLP_COPY                   1410
#define HLP_CUT                    1420
#define HLP_PASTE                  1430
#define HLP_CLEAR                  1440
#define HLP_FONT                   1450
#define HLP_CODEPAGE               1460
#define HLP_HELP                   1500
#define HLP_USINGHELP              1510
#define HLP_GENERALHELP            1520
#define HLP_HELPINDEX              1530
#define HLP_PRODUCTINFO            1540
#define HLP_OK                     3100

// User Messages
#define UM_RESIZE WM_USER+100
#define UM_SETMENUSTATE WM_USER+101
#define UM_ADDCPMENU WM_USER+102
#define UM_DIRECTTYPE WM_USER+103

// Constant variables
#define MAX_BUFFER 8192
#define TEMP_BUFFER 11
#define CHAR_BUFFER 2
#define MAX_ROW 8
#define MAX_COLUMN 32
#define DEFAULT_CODEPAGE 850
#define DEFAULT_POINTSIZE 10
#define MAX_CP 100

// Declaration of global variables
extern HAB hab; // Handle to anchor block
extern HWND hwndMain;

// Declaration of global functions
void main(int argc, char *argv[]);
// Message Hook
extern BOOL EXPENTRY MessageHook(HAB hab, PQMSG pqmsg, ULONG fs);
// Window procedure for the main window
extern MRESULT EXPENTRY WinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// Window procedure for the About messagebox
extern MRESULT EXPENTRY AboutProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// Window procedure for the DirectType dialog
extern MRESULT EXPENTRY DirectTypeProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

