/* about.c: The About module */

// Window procedure for the about dialogbox
// Is called by the system
MRESULT EXPENTRY AboutProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
// Declaration of variables
USHORT ControlID;
switch (msg)
   {
   case WM_INITDLG:
      // Initialization
      // Nothing
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
      // Close the About dialogbox
      // User pressed the Ok button
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