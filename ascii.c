// ASCII Table 2.0
// Copyright 1998 by D.J. van Enckevort
// This program and sourcecode is released as freeware
// You can use this code for your own programs
// If you use it, please keep a notice of this copyright

#define INCL_WIN
#define INCL_WINHOOKS
#define INCL_PM
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ascii.h"
#include "tool.h"

// declaration of the hab
HAB hab;
HWND hwndMain = NULLHANDLE;

// Program main function
// Arguments:
// argc number of commandline arguments;
// argv commandline arguments
// Note argv[0] is the name of the program
// no return code
void main(int argc, char *argv[])
{
HMQ hmq;
QMSG qmsg;
char Path[CCHMAXPATH], *Program = NULL, FullName[CCHMAXPATH];
HINI Profile;
HWND hwndMenu;
HACCEL hAccel;
HPOINTER Icon;

// Do the usual initialization
hab=WinInitialize(0);
hmq=WinCreateMsgQueue(hab, 0);

WinSetHook(hab, HMQ_CURRENT, HK_INPUT, (PFN)MessageHook, NULLHANDLE);
// Open ASCII.INI
strcpy(Path, argv[0]);
strlwr(Path);
Program=strstr(Path, "ascii.exe");
Program[0]=0;
strcpy(FullName, Path);
strcat(FullName, "ascii.ini");
Profile=PrfOpenProfile(hab, FullName);

// Load the main window from the executable resources
hwndMain=WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP, WinProc, (HMODULE)0, DLG_ASCII, &Profile);
// Load menu
hwndMenu = WinLoadMenu(hwndMain, (HMODULE)0, ID_MENU);
// Load icon & assign it to the main window
Icon = WinLoadPointer( HWND_DESKTOP, NULLHANDLE, ID_ICON);
WinSendMsg(hwndMain, WM_SETICON, MPFROMLONG(Icon), (MPARAM)NULL);
// Load Accellerator table and make it active
hAccel = WinLoadAccelTable(hab, (HMODULE)0, ID_ACCELTABLE);
WinSetAccelTable(hab, hAccel, hwndMain);
// update window
WinSendMsg(hwndMain, WM_UPDATEFRAME, MPFROMLONG(ID_MENU), (MPARAM)NULL);

// Start the message queue
while (WinGetMsg(hab, &qmsg, 0L, 0, 0))
  WinDispatchMsg(hab, &qmsg);

// Finish the program and exit
PrfCloseProfile(Profile);
WinDestroyWindow(hwndMain);
// Release hook
WinReleaseHook(hab, HMQ_CURRENT, HK_INPUT, (PFN)MessageHook, NULLHANDLE);
WinDestroyMsgQueue(hmq);
WinTerminate(hab);
}

BOOL EXPENTRY MessageHook(HAB hab, PQMSG pqmsg, ULONG fs)
{
SHORT x, y, dx, dy, col, row;
MRESULT mr;
char title[25];
SWP swp;
// Do we have a WM_MOUSEMOVE for SET_ASCII?

if (pqmsg->msg==WM_MOUSEMOVE)
   {
   if (WinWindowFromID(hwndMain, SET_ASCII)==pqmsg->hwnd)
      {
      // Mouse is moved
      // Position of the mouse
      x=SHORT1FROMMP(pqmsg->mp1);
      y=SHORT2FROMMP(pqmsg->mp1);
      // Size of the control
      WinQueryWindowPos(pqmsg->hwnd, &swp);
      // Calculate division factors
      dx=swp.cx/32;
      dy=swp.cy/8;
      // calculate column and row
      col=(x)/dx;
      row=7-(y)/dy;
      // is it within range?
      if (((row>=0) && (row<=7)) && ((col>=0) && (col<=31)))
         {
         // display the alt-code in the titlebar
         sprintf(title, "ASCII Table - alt-%i", row*32+col);
         WinSetWindowText(hwndMain, title);
         }
      else
         {
         // Set Window title to normal
         WinSetWindowText(hwndMain, "ASCII Table");
         }
      }
   else
      {
      // Set Window title to normal
      WinSetWindowText(hwndMain, "ASCII Table");
      }
   }
if (fs==PM_REMOVE)
   {
   return FALSE;
   }
return TRUE;
}

// Window procedure for the main window
// Is called by the system
MRESULT EXPENTRY WinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
// Several variables
// Used for replies
USHORT Reply;
// Used in WM_COMMAND and WM_CONTROL (NotifyCode)
USHORT ControlID, NotifyCode;
// Used in WM_CONTROL to store text from value set
// Used in WM_INITDLG
USHORT row, col;
// Used in WM_COMMAND
// Used for the Font Dialog
static FONTDLG fd;
// hwndFD = font dialog
// hwndFileDlg = file dialog
// hwndDT = direct type static omdat deze langer bewaard moet blijven
// menu = menu
HWND hwndFD, hwndFileDlg, menu;
static HWND hwndDT;
// Used for the File Dialog
FILEDLG FileDlg;
// used for file open / file save / file save as
FILE *f;
size_t Read, Written;
HPS hps;
// Used for Copy & Clear
IPT Len, StartingPoint;
// CurrentFont used for fontselection & fontset
// ch used in WM_INITDLG
char ch[CHAR_BUFFER], temp[TEMP_BUFFER], buffer[MAX_BUFFER];
static char CurrentFont[FACESIZE];
// Pointsize of the font
static int PointSize;
static USHORT usCodePage;
// static to remember the value of this variable between different calls
// Used in UM_RESIZE to size the controls
static SWP oldframe, initialframe;
SWP newframe;
// Pointer the the handle to the .INI file
static HINI *pProfile;
ULONG Size;
MRESULT mr;
static char FileName[CCHMAXPATH];
POWNERITEM OwnerItem;
static char PtSizeList[80]="1 2 4 6 8 9 10 11 12 14 16 18 20 24 30 36 40 42 48 72 80 96";
// DirectType is set if the user has activated DirectType
// (Three states: Off/Waiting for User Input/On) (DT_OFF/DT_WAIT/DT_ON)
static char DirectType;
switch (msg)
   {
   case WM_INITDLG:
      // Initialize window
      // No filename
      FileName[0]=0;
      usCodePage=850;
      // Create Import/Export buffer
      memset(buffer, 0, MAX_BUFFER);
      // Set MLE Text format
      WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_FORMAT, MPFROMSHORT(MLFIE_NOTRANS), MPFROMLONG(0L));
      // Set Import/Export buffer
      WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_SETIMPORTEXPORT, MPFROMP(buffer), MPFROMLONG(8192));
      // Register this window as a viewer for the clipboard so we will be
      // notified when there is data on the clipboard.
      WinSetClipbrdViewer(hab, hwnd);
      // Set all valueset cells to VIA_OWNERDRAW
      for (row=1; row<=MAX_ROW; row++)
         {
         for (col=1; col<=MAX_COLUMN; col++)
            {
            // Set the contents of the cell
            WinSendMsg(WinWindowFromID(hwnd, SET_ASCII), VM_SETITEMATTR, MPFROM2SHORT(row, col), MPFROM2SHORT(VIA_TEXT | VIA_OWNERDRAW, TRUE));
            }
         }
      // Get the handle to the .INI file
      pProfile=(HINI*)PVOIDFROMMP(mp2);
      // Get the initial size of the window
      WinQueryWindowPos(hwnd, &initialframe);
      // also in oldframe for sizing
      oldframe.cx=initialframe.cx;
      oldframe.cy=initialframe.cy;
      // Read the size
      newframe.cx=PrfQueryProfileInt(*pProfile, "Size", "x", initialframe.cx);
      newframe.cy=PrfQueryProfileInt(*pProfile, "Size", "y", initialframe.cy);
      // Read the position
      newframe.x=PrfQueryProfileInt(*pProfile, "Position", "x", initialframe.x);
      newframe.y=PrfQueryProfileInt(*pProfile, "Position", "y", initialframe.y);
      WinSetWindowPos(hwnd, NULL, newframe.x, newframe.y, newframe.cx, newframe.cy,
                         SWP_ACTIVATE | SWP_SHOW | SWP_SIZE | SWP_MOVE);
      PrfLoadFont(pProfile, &fd);
      // Set the font through the presentation parameters
      PointSize=fd.fxPointSize/65536;
      fd.pszPtSizeList=PtSizeList;
      sprintf(CurrentFont, "%i.%s", PointSize, fd.fAttrs.szFacename);
      // in the value set
      WinSetPresParam(WinWindowFromID(hwnd, SET_ASCII), PP_FONTNAMESIZE, sizeof(CurrentFont), CurrentFont);
      // in the mle (multi line entryfield)
      WinSetPresParam(WinWindowFromID(hwnd, MLE_TEXT), PP_FONTNAMESIZE, sizeof(CurrentFont), CurrentFont);
      // set the text of this static text control to the name and pointsize of the font
      WinSetWindowText(WinWindowFromID(hwnd, ST_FONT), CurrentFont);
      WinPostMsg(hwnd, UM_SETMENUSTATE, MPFROMSHORT(MI_SAVE), MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
      WinPostMsg(hwnd, UM_SETMENUSTATE, MPFROMSHORT(MI_PASTE), MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
      WinPostMsg(hwnd, UM_ADDCPMENU, 0L, 0L);
      break;
   case WM_COMMAND:
      // Commands raising from the controls
      // mp1 contains the resource ID of the control
      ControlID=SHORT1FROMMP(mp1);
      switch (ControlID)
         {
         case MI_CLEAR:
            // Clear menu
            // Get the number of bytes in MLE
            Len=LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_QUERYTEXTLENGTH, 0L, 0L));
            // Delete the contents of the MLE
            WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_DELETE, 0L, MPFROMLONG(Len));
            break;
         case MI_COPY:
            // copy menu
            WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_COPY, 0L, 0L);
            break;
         case MI_CUT:
            // cut menu
            // cut & copy selected text to the clipboard
            WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_CUT, 0L, 0L);
            break;
         case MI_PASTE:
            WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_PASTE, 0L, 0L);
            break;
         case MI_MLE:
            break;
         case MI_DIRECTTYPE:
            menu=HWNDFROMMP(mp2);
            if (DirectType==DT_OFF)
               { // Ask the user for the program for DirectType
               DirectType=DT_WAIT;
               WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP, DirectTypeProc, (HMODULE)0, DLG_ABOUT, NULL);
               WinCheckMenuItem(menu, ControlID, FALSE);
               }
            if (DirectType==DT_ON)
               { // Switch off DirectType
               DirectType=DT_OFF;
               hwndDT=NULL_HANDLE;
               WinCheckMenuItem(menu, ControlID, FALSE);
               }
            if (DirectType==DT_WAIT):
               { // The window is still waiting for user input, switch to the window
               // Probably have to use WinFocusChange, but need to know HWND
               }
            break;
         case MI_FONT:
            // Font menu
            // Open a presentation space (required for the font dialog
            hps=WinGetPS(hwnd);
            // presentation space
            fd.hpsScreen=hps;
            // Open the fonddialog
            hwndFD=WinFontDlg(HWND_DESKTOP, hwnd, &fd);
            if ((hwndFD) && (fd.lReturn==DID_OK))
               {
               // User selected a font and pressed Ok
               // Create a complete fontname
               PointSize=fd.fxPointSize/65536;
               sprintf(CurrentFont, "%i.%s", PointSize, fd.fAttrs.szFacename);
               fd.fAttrs.usCodePage=usCodePage;
               // and set the presentation parameters
               // for the value set
               WinSetPresParam(WinWindowFromID(hwnd, SET_ASCII), PP_FONTNAMESIZE, sizeof(CurrentFont), CurrentFont);
               // for the MLE
               WinSetPresParam(WinWindowFromID(hwnd, MLE_TEXT), PP_FONTNAMESIZE, sizeof(CurrentFont), CurrentFont);
               // Update the text of the static text control
               WinSetWindowText(WinWindowFromID(hwnd, ST_FONT), CurrentFont);
               }
            // release presentation space
            WinReleasePS(hps);
            break;
         case MI_OPEN:
            memset(&FileDlg, 0, sizeof(FILEDLG));
            FileDlg.cbSize=sizeof(FILEDLG);
            FileDlg.fl=FDS_OPEN_DIALOG | FDS_CENTER;
            hwndFileDlg=WinFileDlg(HWND_DESKTOP, hwnd, &FileDlg);
            if ((hwndFileDlg!=NULLHANDLE) && (FileDlg.lReturn==DID_OK))
               {
               if ((f=fopen(FileDlg.szFullFile, "r"))!=NULL)
                  {
                  WinPostMsg(hwnd, UM_SETMENUSTATE, MPFROMSHORT(MI_SAVE), MPFROM2SHORT(MIA_DISABLED, ~MIA_DISABLED));
                  // Get the number of bytes in MLE
                  Len=LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_QUERYTEXTLENGTH, 0L, 0L));
                  // Delete the contents of the MLE
                  WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_DELETE, 0L, MPFROMLONG(Len));
                  strcpy(FileName, FileDlg.szFullFile);
                  StartingPoint=0;
                  while (!feof(f))
                     {
                     Read=fread(buffer, 1, MAX_BUFFER, f);
                     WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_SETIMPORTEXPORT, MPFROMP(buffer), MPFROMLONG(MAX_BUFFER));
                     WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_IMPORT, MPFROMP(&StartingPoint), MPFROMLONG(Read));
                     }
                  fclose(f);
                  }
               else
                  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Couldn't open file, file might be in use.", "Error", 0, MB_OK | MB_ERROR);
               }
            break;
         case MI_SAVE:
            if ((f=fopen(FileName, "w"))!=NULL)
               {
               StartingPoint=0;
               // Get the number of bytes in MLE
               Len=LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_QUERYTEXTLENGTH, 0L, 0L));
               while (Len>0)
                  {
                  WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_SETIMPORTEXPORT, MPFROMP(buffer), MPFROMLONG(MAX_BUFFER));
                  Read=LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_EXPORT, MPFROMP(&StartingPoint), MPFROMLONG(&Len)));
                  Written=fwrite(buffer, 1, Read, f);
                  }
               fclose(f);
               }
            else
               WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Couldn't open file, file might be in use.", "Error", 0, MB_OK | MB_ERROR);
            break;
         case MI_SAVEAS:
            do
               {
               Reply=MBID_YES;
               memset(&FileDlg, 0, sizeof(FILEDLG));
               FileDlg.cbSize=sizeof(FILEDLG);
               FileDlg.fl=FDS_SAVEAS_DIALOG | FDS_CENTER;
               hwndFileDlg=WinFileDlg(HWND_DESKTOP, hwnd, &FileDlg);
               if ((hwndFileDlg!=NULLHANDLE) && (FileDlg.lReturn==DID_OK))
                  {
                  if ((f=fopen(FileDlg.szFullFile, "r"))!=NULL)
                     {
                     Reply=WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "File already exists, overwrite?", "Warning", 0, MB_YESNO | MB_QUERY);
                     fclose(f);
                     }
                  else
                     {
                     Reply=MBID_YES;
                     }
                  }
               else
                  if (FileDlg.lReturn==0)
                     WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Failed to open file dialog, cannot save file!", "Error", 0, MB_OK | MB_ERROR);
//                  Reply=MBID_NO;
               }
            while (Reply==MBID_NO);
            if ((hwndFileDlg!=NULLHANDLE) && (FileDlg.lReturn==DID_OK))
               {
               if ((f=fopen(FileDlg.szFullFile, "w"))!=NULL)
                  {
                  strcpy(FileName, FileDlg.szFullFile);
                  StartingPoint=0;
                  // Get the number of bytes in MLE
                  Len=LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_QUERYTEXTLENGTH, 0L, 0L));
                  while (Len>0)
                     {
                     WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_SETIMPORTEXPORT, MPFROMP(buffer), MPFROMLONG(MAX_BUFFER));
                     Read=LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_EXPORT, MPFROMP(&StartingPoint), MPFROMLONG(&Len)));
                     Written=fwrite(buffer, 1, Read, f);
                     }
                  fclose(f);
                  }
               else
                  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Couldn't open file, file might be in use.", "Error", 0, MB_OK | MB_ERROR);
               }
            break;
         case MI_CLOSE:
            WinPostMsg(hwnd, WM_CLOSE, 0L, 0L);
            break;
         case MI_PRODUCTINFO:
            // ProductInfo menu
            // Load and display the about dialog, we don't load it modally, because I hate that
            WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP, AboutProc, (HMODULE)0, DLG_ABOUT, NULL);
            break;
         default:
            // The control ID is of one of the codepage menus?
            if (ControlID>1000)
               {
               menu=WinWindowFromID(hwnd, FID_MENU); // Vraag de handle naar het menu op
               WinCheckMenuItem(menu, usCodePage+1000, FALSE);
               WinCheckMenuItem(menu, ControlID, TRUE);
               usCodePage=ControlID-1000;
               fd.fAttrs.usCodePage=usCodePage;
               WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_SETFONT, MPFROMP(&fd.fAttrs), 0L);
               WinSetPresParam(WinWindowFromID(hwnd, SET_ASCII), PP_FONTNAMESIZE, sizeof(CurrentFont), CurrentFont);
               }
            break;
         }
      break;
   case WM_CONTROL:
      // Messages from controls
      // Control resource ID
      ControlID=SHORT1FROMMP(mp1);
      // Notify code
      NotifyCode=SHORT2FROMMP(mp1);
      switch (ControlID)
         {
         case SET_ASCII:
            // Value set
            if ((NotifyCode==VN_SELECT) || (NotifyCode==VN_ENTER))
               {
               // A Cell was selected, or enter was pressed in the selected cell
               mr=WinSendMsg(WinWindowFromID(hwnd, SET_ASCII), VM_QUERYSELECTEDITEM, 0L, 0L);
               row=SHORT1FROMMR(mr)-1;
               col=SHORT2FROMMR(mr)-1;
               ch[0]=row*MAX_COLUMN+col;
               ch[1]=0;
               // Insert the text in the MLE
               WinSendMsg(WinWindowFromID(hwnd, MLE_TEXT), MLM_INSERT, MPFROMP(ch), 0L);
               }
            break;
         }
      break;
   case WM_DRAWITEM:
      ControlID=SHORT1FROMMP(mp1);
      if (ControlID==SET_ASCII)
         {
         OwnerItem=(OWNERITEM*)PVOIDFROMMP(mp2);
         switch (OwnerItem->idItem)
            {
            case VDA_ITEM:
               DrawValueSet(OwnerItem, &fd.fAttrs);
               return (MRESULT)TRUE;
            }
         }
      break;
   case WM_DRAWCLIPBOARD:
      // Open the clipboard
      WinOpenClipbrd(hab);
      // Does the clipboard have text?
      if (WinQueryClipbrdData(hab,CF_TEXT)!=0)
         { // Make the Paste menu available
         WinSendMsg(WinWindowFromID(hwnd, FID_MENU), MM_SETITEMATTR, MPFROM2SHORT(MI_PASTE, TRUE), MPFROM2SHORT(MIA_DISABLED, ~MIA_DISABLED));
         }
      else
         { // Disable the Paste menu
         WinSendMsg(WinWindowFromID(hwnd, FID_MENU), MM_SETITEMATTR, MPFROM2SHORT(MI_PASTE, TRUE), MPFROM2SHORT(MIA_DISABLED, MIA_DISABLED));
         }
      // Close the clipboard
      WinCloseClipbrd(hab);
      break;
   case WM_PAINT:
      // Window need to be repainted
      // Call Default Dialog Window Procedure to repaint the window
      WinDefDlgProc(hwnd, msg, mp1, mp2);
      // Do a resize of the controls (should check first whether it is needed)
      WinQueryWindowPos(hwnd, &newframe);
      if ((newframe.cx!=oldframe.cx) || (newframe.cy!=oldframe.cy))
         {
         WinPostMsg(hwnd, UM_RESIZE, 0L, 0L);
         }
      break;
   case UM_RESIZE:
      // Requested to resize the controls
      // Query new size of the window
      WinQueryWindowPos(hwnd, &newframe);
      // Check whether the new size isn't too small
      if (newframe.cx<initialframe.cx)
         {
         // window is to small, restore it to the initial size
         // Copy the initial sizes to the newframe structure
         // So we use the initial size, but the new position
         newframe.cx=initialframe.cx;
         // Make it so
         WinSetWindowPos(hwnd, NULL, newframe.x, newframe.y, newframe.cx, newframe.cy,
                         SWP_ACTIVATE | SWP_SHOW | SWP_SIZE | SWP_MOVE);
         // And break out, we've finished the resize (and don't want to do more unneeded work)
         break;
         }
      if (newframe.cy<initialframe.cy)
         {
         // window is to small, restore it to the initial size
         // Copy the initial sizes to the newframe structure
         // So we use the initial size, but the new position
         newframe.cy=initialframe.cy;
         // Make it so
         WinSetWindowPos(hwnd, NULL, newframe.x, newframe.y, newframe.cx, newframe.cy,
                         SWP_ACTIVATE | SWP_SHOW | SWP_SIZE | SWP_MOVE);
         // And break out, we've finished the resize (and don't want to do more unneeded work)
         break;
         }
      // Don't move around these, in this order they will be correctly repainted
      // The MLE must be moved, and be resized if the width changes
      ResizeX(hwnd, oldframe, newframe, MLE_TEXT);
      // These controls we don't want to size, they have their proper size already, just have to move them
      // to their new positions
      // Move the static text with the font name
      Move(hwnd, oldframe, newframe, ST_FONT);
      // The value set will be resized in both directions
      ResizeXY(hwnd, oldframe, newframe, SET_ASCII);
      // Query the size of the window again, to store in oldframe
      // (could have copied it from newframe too)
      WinQueryWindowPos(hwnd, &oldframe);
      WinPostMsg(hwnd, WM_PAINT, 0L, 0L);
      break;
   case UM_SETMENUSTATE:
      // Set state of menu
      WinSendMsg(WinWindowFromID(hwnd, FID_MENU), MM_SETITEMATTR, MPFROM2SHORT(SHORT1FROMMP(mp1), TRUE), mp2);
      break;
   case UM_ADDCPMENU:
      AddCPMenu(hwnd);
      menu=WinWindowFromID(hwnd, FID_MENU); // Vraag de handle naar het menu op
      WinCheckMenuItem(menu, usCodePage+1000, TRUE);
      break;
   case UM_DIRECTTYPE:
      break;
   case WM_CLOSE:
      // User has selected to close the program
      // (Could ask for confirmation here)
      // Save the size & position
      WinQueryWindowPos(hwnd, &newframe);
      sprintf(temp, "%li", newframe.cx);
      PrfWriteProfileString(*pProfile, "Size", "x", temp);
      sprintf(temp, "%li", newframe.cy);
      PrfWriteProfileString(*pProfile, "Size", "y", temp);
      sprintf(temp, "%li", newframe.x);
      PrfWriteProfileString(*pProfile, "Position", "x", temp);
      sprintf(temp, "%li", newframe.y);
      PrfWriteProfileString(*pProfile, "Position", "y", temp);
      // Save the font dialog values
      PrfWriteProfileData(*pProfile, "FontAttr", "PointSize", &PointSize, sizeof(INT));
      PrfWriteProfileData(*pProfile, "FontAttr", "fsSelection", &fd.fAttrs.fsSelection, sizeof(USHORT));
      PrfWriteProfileData(*pProfile, "FontAttr", "lMatch", &fd.fAttrs.lMatch, sizeof(LONG));
      PrfWriteProfileString(*pProfile, "FontAttr", "Facename", fd.fAttrs.szFacename);
      PrfWriteProfileData(*pProfile, "FontAttr", "isRegistry", &fd.fAttrs.idRegistry, sizeof(USHORT));
      PrfWriteProfileData(*pProfile, "FontAttr", "usCodePage", &fd.fAttrs.usCodePage, sizeof(USHORT));
      PrfWriteProfileData(*pProfile, "FontAttr", "lMaxBaselineExt", &fd.fAttrs.lMaxBaselineExt, sizeof(LONG));
      PrfWriteProfileData(*pProfile, "FontAttr", "fAveCharWidth", &fd.fAttrs.lAveCharWidth, sizeof(LONG));
      PrfWriteProfileData(*pProfile, "FontAttr", "fsType", &fd.fAttrs.fsType, sizeof(USHORT));
      PrfWriteProfileData(*pProfile, "FontAttr", "fsFontUse", &fd.fAttrs.fsFontUse, sizeof(USHORT));
      // Dismiss the window
      WinDismissDlg(hwnd, TRUE);
      // Let the program know we are ready to quit
      WinPostMsg(hwnd, WM_QUIT, 0L, 0L);
      break;
   default:
      // We didn't handle the message, let the default window procedure handle it
      return WinDefDlgProc(hwnd, msg, mp1, mp2);
   }
// We have handled all messages
return (MRESULT)FALSE;
}


// Function to resize and move controls
// Parameters:
// hwnd Window handler of the main window
// oldframe & newframe SWP structures which contain the old & new sizes of the main window
//   used to calculate the new size and position of the client window
// ID Resource ID of the client window
void ResizeXY(HWND hwnd, SWP oldframe, SWP newframe, ULONG ID)
{
// structure to store the current position and size
SWP swp;
// Flags to resize and move
ULONG flags=SWP_ACTIVATE | SWP_SHOW | SWP_SIZE | SWP_MOVE;
// Query current position and size
WinQueryWindowPos(WinWindowFromID(hwnd, ID), &swp);
// Calculate the new position and size
swp.cx=swp.cx+newframe.cx-oldframe.cx;
swp.cy=swp.cy+newframe.cy-oldframe.cy;
// Move and resize the control
WinSetWindowPos(WinWindowFromID(hwnd, ID), NULL, swp.x, swp.y, swp.cx, swp.cy, flags);
}
