/* dtype.c: DirectType Module */
/* This module includes everything that has to do with DirectType. DirectType
   is a way to insert characters directly into another application. It is meant
   for people who want to type long texts with a lot of characters that are
   difficult to insert.
*/

#define INCL_WIN
#define INCL_WINHOOKS
#define INCL_PM
#define INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ascii.h"
#include "dtype.h"

/* At the moment I don't know exactly how I will do this, I hope it will be
   sufficient to just post a message to the application.
   At least I need a dialogbox, and therefore a Window Procedure, and I need
   a procedure to send the WM_CHAR message to the receiving application.
*/


// Window procedure for the DirectType dialogbox
// Is called by the system
MRESULT EXPENTRY DirectTypeProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
// Declaration of variables
USHORT ControlID;
APIRET rc;
ULONG cbItems, ulBufSize, i;
static PSWBLOCK pSwBlock;
SWENTRY p;
switch (msg)
   {
   case WM_INITDLG:
      // Initialization
      // Query all the programs and fill the listbox with their names
      // Query the number of items in the switchlist
      cbItems=WinQuerySwitchList(hab, NULL, 0);
      // Calculate needed memory
      ulBufSize=(cbItems*sizeof(SWENTRY))+sizeof(HSWITCH);
      // and allocate it
      rc=DosAllocMem(pSwBlock, ulBufSize, PAG_COMMIT | PAG_READ | PAG_WRITE);
      if (rc!=0)
         {
         WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Not enough memory", "Error", MB_OK | MB_ERROR);
         WinSendMsg(hwnd, WM_CLOSE, 0L, 0L);
         }
      cbItems=WinQuerySwitchList(hab, pSwBlock, ulBufSize);
      // Populate listbox
      p=pSwBlock->aswentry;
      for (i=0; i<pSwBlock->cswentry; i++, p++)
         {
         WinSendMsg(WinWindowFromID(hwnd, LB_PROGRAMS), LIT_END, p->szSwTitle);
         }
      break;
   case WM_COMMAND:
      // Commands from controls
      // Resource ID of control
      ControlID=SHORT1FROMMP(mp1);
      switch (ControlID)
         {
         case PB_OK:
            // Pushbutton pressed
            // Close the About dialogbox
            // Send the WM_CLOSE message
            WinSendMsg(hwnd, WM_CLOSE, 0L, 0L);
            break;
         }
      break;
   case WM_CLOSE:
      // Close the dialogbox
      // Dismiss the dialogbox
      WinDismissDlg(hwnd, TRUE);
      // Note we now do not send a WM_QUIT message, if we did so, we would close all of the application
      break;
   default:
      // Let the default window procedure handle messages we didn't handle
      return WinDefDlgProc(hwnd, msg, mp1, mp2);
   }
// All messages are handled
return (MRESULT)FALSE;
}


