// Function to resize and move controls
void ResizeXY(HWND hwnd, SWP oldframe, SWP newframe, ULONG ID);
// Function to resize and move controls
void ResizeX(HWND hwnd, SWP oldframe, SWP newframe, ULONG ID);
// Function to move controls
void Move(HWND hwnd, SWP oldframe, SWP newframe, ULONG ID);
// Add a Codepage menu
void AddCPMenu(HWND hwndFrame);
// Draw Value Set cell
void DrawValueSet(POWNERITEM OwnerItem, PFATTRS fAttrs);
// Load Font Information from profile
void PrfLoadFont(HINI *pProfile, PFONTDLG pfd);
// Save Font information to profile
void PrfSaveFont(HINI *pProfile, PFONTDLG pfd);
