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

void PrfLoadFont(HINI *pProfile, PFONTDLG pfd)
{
ULONG Size;
int PointSize;
char CurrentFont[FACESIZE];
// Read the font dialog data
// Set default pointsize
PointSize=DEFAULT_POINTSIZE;
// Set all values in the FONTDLG structure fd to zero
memset(pfd, 0, sizeof(FONTDLG));
// Fill the structure with the necessary data
// set default fAttrs values
memset(&pfd->fAttrs, 0, sizeof(FATTRS));
pfd->fAttrs.usRecordLength=sizeof(FATTRS);
pfd->fAttrs.usCodePage=DEFAULT_CODEPAGE;
// read fAttrs values
PrfQueryProfileData(*pProfile, "FontAttr", "PointSize", &PointSize, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "fsSelection", &pfd->fAttrs.fsSelection, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "lMatch", &pfd->fAttrs.lMatch, &Size);
PrfQueryProfileString(*pProfile, "FontAttr", "Facename", "Helv", pfd->fAttrs.szFacename, FACESIZE);
PrfQueryProfileData(*pProfile, "FontAttr", "isRegistry", &pfd->fAttrs.idRegistry, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "usCodePage", &pfd->fAttrs.usCodePage, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "lMaxBaselineExt", &pfd->fAttrs.lMaxBaselineExt, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "fAveCharWidth", &pfd->fAttrs.lAveCharWidth, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "fsType", &pfd->fAttrs.fsType, &Size);
PrfQueryProfileData(*pProfile, "FontAttr", "fsFontUse", &pfd->fAttrs.fsFontUse, &Size);
// size of the structure
pfd->cbSize=sizeof(FONTDLG);
DosAllocMem((PPVOID)&pfd->pszFamilyname, FACESIZE, PAG_COMMIT | PAG_READ | PAG_WRITE);
pfd->pszFamilyname[0]=0;
// Preferred point size
pfd->fxPointSize=MAKEFIXED(PointSize,0);
// Size of buffer for fontfamily name
pfd->usFamilyBufLen=FACESIZE;
// Default foreground colour
pfd->clrFore=SYSCLR_WINDOWTEXT;
// Default background colour
pfd->clrBack=SYSCLR_WINDOW;
// Dialog flags
// Center dialog, and use font attributes
pfd->fl=FNTS_CENTER | FNTS_INITFROMFATTRS;
}

void PrfSaveFont(PFONTDLG pfd)
{
}

// Function to resize and move controls
// Parameters:
// hwnd Window handler of the main window
// oldframe & newframe SWP structures which contain the old & new sizes of the main window
//   used to calculate the new size and position of the client window
// ID Resource ID of the client window
void ResizeX(HWND hwnd, SWP oldframe, SWP newframe, ULONG ID)
{
// structure to store the current position and size
SWP swp;
// Flags to resize and move
ULONG flags=SWP_ACTIVATE | SWP_SHOW | SWP_SIZE | SWP_MOVE;
// Query current position and size
WinQueryWindowPos(WinWindowFromID(hwnd, ID), &swp);
// Calculate the new position and size
swp.y=swp.y+newframe.cy-oldframe.cy;
swp.cx=swp.cx+newframe.cx-oldframe.cx;
// Move and resize the control
WinSetWindowPos(WinWindowFromID(hwnd, ID), NULL, swp.x, swp.y, swp.cx, swp.cy, flags);
}
// Function to move controls
// Parameters:
// hwnd Window handler of the main window
// oldframe & newframe SWP structures which contain the old & new sizes of the main window
// ID Resource ID of the client window
void Move(HWND hwnd, SWP oldframe, SWP newframe, ULONG ID)
{
// structure to store the current position and size
SWP swp;
// Flags to move
ULONG flags=SWP_ACTIVATE | SWP_SHOW | SWP_MOVE;
// Query current position and size
WinQueryWindowPos(WinWindowFromID(hwnd, ID), &swp);
// Calculate the new position
swp.y=swp.y+newframe.cy-oldframe.cy;
// Move the control
WinSetWindowPos(WinWindowFromID(hwnd, ID), NULL, swp.x, swp.y, swp.cx, swp.cy, flags);
}


// AddCPMenu()
// Adds a menu with all available codepages
void AddCPMenu(HWND hwndFrame)
{
ULONG CodePages[MAX_CP], CPs;
MENUITEM mi;
HWND hwndMenu, hwndSubMenu, hwndPullDown;
int i;
char CodePageString[10];
hwndMenu=WinWindowFromID(hwndFrame, FID_MENU);
WinSendMsg(hwndMenu, MM_QUERYITEM, MPFROM2SHORT(MI_CODEPAGE, TRUE), MPFROMP(&mi));
hwndPullDown=mi.hwndSubMenu;
CPs=WinQueryCpList(hab, MAX_CP, CodePages);
for (i=0; i<CPs; i++)
   {
   mi.iPosition=MIT_END;
   mi.afStyle=MIS_TEXT;
   mi.afAttribute=0;
   mi.id=1000+CodePages[i];
   mi.hwndSubMenu=NULL;
   mi.hItem=0;
   sprintf(CodePageString, "%lu", CodePages[i]);
   if (((i%15)==0) && (i!=0))
      {
      mi.afStyle=MIS_TEXT | MIS_BREAKSEPARATOR;
      }
   WinSendMsg(hwndPullDown, MM_INSERTITEM, MPFROMP(&mi), MPFROMP(CodePageString));
   }
}

// Draw the ValueSet cell
void DrawValueSet(POWNERITEM OwnerItem, PFATTRS fAttrs)
{
POINTL Point;
char ch[CHAR_BUFFER];
USHORT row, col;
FONTMETRICS FontMetrics;
GpiCreateLogFont(OwnerItem->hps, NULL, 1L, fAttrs);
//GpiSetCp(OwnerItem->hps, fAttrs->usCodePage);
GpiSetCharSet(OwnerItem->hps, 1L);
row=SHORT1FROMMR(OwnerItem->hItem);
col=SHORT2FROMMR(OwnerItem->hItem);
row--;
col--;
ch[0]=row*MAX_COLUMN+col;
ch[1]=0;
Point.x=OwnerItem->rclItem.xLeft;
Point.y=OwnerItem->rclItem.yBottom;
GpiErase(OwnerItem->hps);
GpiQueryFontMetrics(OwnerItem->hps, sizeof(FONTMETRICS), &FontMetrics);
Point.y+=FontMetrics.lMaxDescender+((OwnerItem->rclItem.yTop-OwnerItem->rclItem.yBottom-FontMetrics.lMaxBaselineExt)/2);
Point.x+=(OwnerItem->rclItem.xRight - OwnerItem->rclItem.xLeft - FontMetrics.lMaxCharInc) / 2;
GpiSetAttrs(OwnerItem->hps, PRIM_CHAR, CBB_COLOR | CBB_BACK_COLOR, CBB_COLOR | CBB_BACK_COLOR, NULL);
GpiCharStringAt(OwnerItem->hps, &Point, 1, ch);
}
