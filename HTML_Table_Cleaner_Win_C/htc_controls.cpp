#pragma once
#include "htc_controls.h"

// Function to read a text file.
BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName) {
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(
		pszFileName, // The name of the file or device to be created or opened.
		GENERIC_READ, // Requested access to the file.
		FILE_SHARE_READ, // Requested sharing mode.
		NULL, // A pointer to a SECURITY_ATTRIBUTES structure.
		OPEN_EXISTING, // An action to take on a file or device that exists or does not exist.
		0, // The file attributes and flags.
		NULL // Handle to a template file with the GENERIC_READ access right.
	);

	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSize;

		// Check the file size so we know how much memory we need to allocate. 
		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF) {
			LPSTR pszFileText;

			// Allocates the  specific number of bytes from the heap.
			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (pszFileText != NULL)
			{
				// Receives the number of bytes read when using a synchronous hFile parameter.
				DWORD dwRead;

				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Add null terminator.
												 // Changes the text of the specified window.
					if (SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE;
				}
				// Frees specified global memory object and invalidates its memory.
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

// Save text from an edit box.
BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName) {
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(
		pszFileName, // The name of the file to be create.
		GENERIC_WRITE, // The requested access to the file.
		0, // The sharing mode. If it's zero and CF func succeeds, the file cannot be shared until the handle is closed.
		NULL, // A pointer to security attribute.
		CREATE_ALWAYS, // If specified file exists and is writable, the function overwrites the file.
		FILE_ATTRIBUTE_NORMAL, // The most common default value for files - attributes and flags.
		NULL // A handle to a template file.
	);

	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwTextLength;

		// Retrieves the length, in characters, of the window's title bar text (if it has any).
		// If the window is a control, retrieves the length of the text with control.
		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if (dwTextLength > 0) {
			LPSTR pszText;
			DWORD dwBufferSize = dwTextLength + 1;

			// Allocate the specified number of bytes from the heap.
			pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
			if (pszText != NULL) {
				// Copies the text of the window's title bar (if it has one) or the text of the control.
				if (GetWindowText(hEdit, pszText, dwBufferSize)) {
					DWORD dwWritten;

					if (WriteFile(
						hFile, // Handle to the file.
						pszText, // Pointer to the buffer containing the data to be written.
						dwTextLength, // The number of bytes to be written.
						&dwWritten, // A pointer to the variable that receives the number of bytes written.
						NULL // A pointer to an OVERLAPPED structure.
					)) {
						// Change return variable if WriteFile() was succeed.
						bSuccess = TRUE;
					}
				}
			}
		}
	}
	return bSuccess;
}

// Open file dialogue.
void DoFileOpen(HWND hwnd) {
	// Try some time later - starting with Windows Vista,
	// the Common Item Dialog supersedes the older Common File Dialog.
	// OPENFILENAME containes information that the GetOpenFileName and GetSaveFileName
	// use to initialize dialog box.
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	// Fills a block of memory with zeroes.
	// This way we don't need to explicitely set each member that we don't use.
	ZeroMemory(&ofn, sizeof(ofn));

	// Fill ofn structure.
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH; // The size in characters of the buffer pointed to by lpstrFile.
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt"; // The default extension.

	if (GetOpenFileName(&ofn)) {
		// Retrieves a handle to a control in the specified dialog box.
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		LoadTextFileToEdit(hEdit, szFileName);
	}
}

// Save file dialog.
void DoFileSave(HWND hwnd) {
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "txt";

	if (GetSaveFileName(&ofn)) {
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SaveTextFileFromEdit(hEdit, szFileName);
	}
}

// Create main window menu.
BOOL CreateMainWindowMenu(HWND hwndParent) {
	// Start the main menu creating.
	HMENU hMenu, hSubMenu;
	HICON hIcon, hIconSm;

	hMenu = CreateMenu();
	if (!hMenu) {
		MessageBox(NULL, "CreateMenu() failed!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hSubMenu = CreatePopupMenu();
	if (!hSubMenu) {
		MessageBox(NULL, "CreatePopupMenu() failed!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	// Append a new item to the specified menu bar, drop-down menu, submenu or shortcut menu.
	AppendMenu(hSubMenu, MF_STRING, IDM_FILE_NEW, "&New");
	AppendMenu(hSubMenu, MF_STRING, IDM_FILE_OPEN, "&Open");
	AppendMenu(hSubMenu, MF_STRING, IDM_FILE_SAVEAS, "&Save As");
	AppendMenu(
		hSubMenu, // A handle to the menu to be changed.
		MF_STRING, // Controls the appearance and behavior of the menu item. 
		IDM_FILE_EXIT, // The identifier of the menu item.
		"&Exit" // The content of the menu item.
	);
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

	hSubMenu = CreatePopupMenu();
	if (!hSubMenu) {
		MessageBox(NULL, "CreatePopupMenu() failed!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	AppendMenu(hSubMenu, MF_STRING, IDM_HELP_ABOUT, "&About");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

	if (!SetMenu(hwndParent, hMenu)) {
		MessageBox(NULL, "SetMenu() failed!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Large icon setup. File from VS2107 image library.
	hIcon = (HICON)LoadImage(NULL, "UpdateLinkedTable_32x.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	if (hIcon) {
		SendMessage(hwndParent, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
	else {
		MessageBox(hwndParent, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Small icon setup.
	hIconSm = (HICON)LoadImage(NULL, "UpdateLinkedTable_16x.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	if (hIconSm) {
		SendMessage(hwndParent, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
	}
	else {
		MessageBox(hwndParent, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	// End the main menu creating.
	return true;
}

// Create main window edit text box.
HWND CreateMainEditBox(HWND hwndParent) {
	// Start the main editor window.
	HWND hwndEdit;
	LOGFONT lfDefault;
	HFONT hfDefault;

	hwndEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE, // Extended window style - border with a sunken edge.
		"EDIT", // Predefined class name - rectangular child window into wich the user can type text.
		"", // The window name.
			// Window styles.
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		0, // Initial horizontal position.
		0, // Initial vertical position.
		100, // The width of the window in device units. 
		100, // The height of the window in device units.
		hwndParent, // A handle to the parent.
		(HMENU)IDC_MAIN_EDIT, // For a child window specifices the child-window identifiere.
		GetModuleHandle(NULL), // A handle to the instance of the module to be associated with the window.
		NULL // Pointer to a value to be passed through the CREATESTRUCT. NULL if no additional data is needed.
	);
	if (hwndEdit == NULL) {
		MessageBox(hwndParent, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	// Get system font.
	// This structure retrieves the metrics associated with nonclient area or nonminimized windows.
	NONCLIENTMETRICS editBoxMetrics;
	// Set size of this structure.
	editBoxMetrics.cbSize = sizeof(NONCLIENTMETRICS);

	// Retrieves metrics.
	bool result = SystemParametersInfo(
		SPI_GETNONCLIENTMETRICS, // Retrieves the metrics.
		sizeof(NONCLIENTMETRICS), // Size of the metrics structure.
		&editBoxMetrics, // Structure wich stores retrieving metrics.
		0
	);
	if (result) {
		// Get LOGFONT structure of default font for message windows.
		lfDefault = editBoxMetrics.lfMessageFont;
		// Create logic font based on LOGFONT structure.
		hfDefault = CreateFontIndirect(&lfDefault);
		// Pass HFONT handle with message.
		SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	}
	else {
		MessageBox(hwndParent, "Could not obtain the system metrics.", "Error", MB_OK | MB_ICONERROR);
		return NULL;
	}
	return hwndEdit;
}

// Create the main toolbar.
HWND CreateMainToolbar(HWND hwndParent) {
	// Create the toolbar.
	HWND hwndToolbar = CreateWindowEx(
		0, // Extended window style (DWORD).
		TOOLBARCLASSNAME, // Class name provided by the common control library (LPCTSTR).
		NULL, // The window name (LPCTSTR).
		WS_CHILD | WS_VISIBLE, // The style of the window being created (DWORD). Create the toolbar that
									 // can have multiple lines of buttons.
		0, // The initial horizontal position of the window (int).
		0, // The initial vertical position of the window (int).
		0, // The width of the window in device units (int).
		0, // The height of the window in device units (int).
		hwndParent, // A handle to the parent window.
		(HMENU)IDC_MAIN_TOOL, // A handle to a menu.
		GetModuleHandle(NULL), // A handle to the instance of the module.
		NULL // Pointer to a value to be passed to the window through the CREATESTRUCT structure.
			 // NULL if no data is needed.
	);

	if (hwndToolbar == NULL) {
		MessageBox(NULL, "CreateWindowEx() in CreateMainToolbar failed.", "Error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	// Send the TB_BUTTONSTRUCTSIZE message, wich is required for
	// backward compatibility.
	// From MSDN: if the toolbar was created using the CreateWindowEx(), you must send the TB_BUTTONSTRUCTSIZE
	// message to the toolbar before sending TB_ADDBUTTONS.
	SendMessage(hwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	// Structs for buttons and bitmaps.
	TBBUTTON tbButtons[3];
	TBADDBITMAP tbAddBitmap;

	// Adds the standart bitmaps to the toolbar, using the imagelist predefined in the common control library.
	tbAddBitmap.hInst = HINST_COMMCTRL;
	tbAddBitmap.nID = IDB_STD_SMALL_COLOR;
	SendMessage(hwndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbAddBitmap);

	// Adds buttons.
	ZeroMemory(tbButtons, sizeof(tbButtons));
	tbButtons[0].iBitmap = STD_FILENEW;
	tbButtons[0].fsState = TBSTATE_ENABLED;
	tbButtons[0].fsStyle = TBSTYLE_BUTTON;
	tbButtons[0].idCommand = IDM_FILE_NEW;

	tbButtons[1].iBitmap = STD_FILEOPEN;
	tbButtons[1].fsState = TBSTATE_ENABLED;
	tbButtons[1].fsStyle = TBSTYLE_BUTTON;
	tbButtons[1].idCommand = IDM_FILE_OPEN;

	tbButtons[2].iBitmap = STD_FILESAVE;
	tbButtons[2].fsState = TBSTATE_ENABLED;
	tbButtons[2].fsStyle = TBSTYLE_BUTTON;
	tbButtons[2].idCommand = IDM_FILE_SAVEAS;

	SendMessage(
		hwndToolbar, // Handle to the toolbar.
		TB_ADDBUTTONS, // Adds one or more buttons to a toolbar.
		sizeof(tbButtons) / sizeof(TBBUTTON), // It's doing a calculation of the number of buttons in the array tbButtons.
		(LPARAM)&tbButtons
	);

	return hwndToolbar;
}

HWND CreateStatusBar(HWND hwndParent) {
	HWND hwndStatus{};

	hwndStatus = CreateWindowEx(
		0,
		STATUSCLASSNAME, // Create status windows - display status information in a horizontal window.
		NULL,
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, // Last includes a sizing grip at the right end of the status bar.
		0, 0, 0, 0,
		hwndParent,
		(HMENU)IDC_MAIN_STATUS,
		GetModuleHandle(NULL),
		NULL
	);
	if (!hwndStatus) {
		MessageBox(NULL, "Could not create status bar handle!", "Error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	// The width of sections in the status bar. -1 = any remaining space.
	int statWidth[] = { 100, -1 };

	SendMessage(hwndStatus, SB_SETPARTS, sizeof(statWidth) / sizeof(int), (LPARAM)statWidth);
	SendMessage(hwndStatus, SB_SETTEXT, 0, (LPARAM)"Some useful things will be here: ");

	return hwndStatus;
}