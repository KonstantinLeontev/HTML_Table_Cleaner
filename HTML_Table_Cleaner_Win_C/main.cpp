// The WinAPI code based on the winprog.org tutorials.
//////////////////////////////////////////////////////

#include <windows.h>
#include "resource.h"

// Menu symbols.
#define ID_FILE_NEW 9000
#define ID_FILE_OPEN 9001
#define ID_FILE_SAVEAS 9002
#define ID_FILE_EXIT 9003
#define ID_HELP_ABOUT 9004
// Text editor symbol.
#define IDC_MAIN_EDIT 101

const char g_szClassName[] = "myWindowClass";

// Window procedure declaration.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Dialog box procedure declaration.
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Function to read a text file.
BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);

// Save text from an edit box.
BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);

// Open file dialog.
void DoFileOpen(HWND hwnd);

// Save file dialog.
void DoFileSave(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	// Registering the windows class.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Register failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Creating the window.
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"HTML Table Cleaner",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// The message loop.
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

// Window procedure definition.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	// Sent when an application requested that a window be created by calling CreateWindow().
	case WM_CREATE: {
		// Start the main menu creating.
		HMENU hMenu, hSubMenu;
		HICON hIcon, hIconSm;

		hMenu = CreateMenu();

		hSubMenu = CreatePopupMenu();
		// Append a new item to the specified menu bar, drop-down menu, submenu or shortcut menu.
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_NEW, "&New");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVEAS, "&Save As");
		AppendMenu(
			hSubMenu, // A handle to the menu to be changed.
			MF_STRING, // Controls the appearance and behavior of the menu item. 
			ID_FILE_EXIT, // The identifier of the menu item.
			"&Exit" // The content of the menu item.
		);
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

		SetMenu(hwnd, hMenu);

		// Large icon setup. File from VS2107 image library.
		hIcon = (HICON)LoadImage(NULL, "UpdateLinkedTable_32x.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		if (hIcon) {
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
		else {
			MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);
		}

		// Small icon setup.
		hIconSm = (HICON)LoadImage(NULL, "UpdateLinkedTable_16x.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		if (hIconSm) {
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
		}
		else {
			MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
		}
		// End the main menu creating.

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
			hwnd, // A handle to the parent.
			(HMENU)IDC_MAIN_EDIT, // For a child window specifices the child-window identifiere.
			GetModuleHandle(NULL), // A handle to the instance of the module to be associated with the window.
			NULL // Pointer to a value to be passed through the CREATESTRUCT. NULL if no additional data is needed.
		);
		if (hwndEdit == NULL) {
			MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
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
			MessageBox(hwnd, "Could not obtain the system metrics.", "Error", MB_OK | MB_ICONERROR);
		}	
	}
		break;

	// Sent to a window after it's size has changed.
	case WM_SIZE: {
		HWND hwndEdit{};
		RECT rcClient{}; // Structure that defines the coordinate of the upper-left and lower-right corner.
		BOOL result{};

		// Retrieves the coordinate of a  main window's client area.
		result = GetClientRect(hwnd, &rcClient);
		if (!result) {
			MessageBox(hwnd, "Couldn't get the client area size.", "Error", MB_OK | MB_ICONERROR);
		}

		// Retrieves a handle to a control in the specified dialog box.
		hwndEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);

		result = SetWindowPos(
			hwndEdit, // Handle to the window.
			NULL, // Handle to the window to 
			0, // Position of the left side.
			0, // Position of the top side.
			rcClient.right - 200, // Width in pixels.
			rcClient.bottom, // Height in pixels.
			SWP_NOZORDER // The window sizing and position flag. This one retains the current Z order.
		);
		if (!result) {
			MessageBox(hwnd, "Could not set editbox's position.\nSetWindowPos() was failed.", "Error", MB_OK | MB_ICONERROR);
		}
	}
		break;

	// Sent when the user selects a command item from a menu.
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_FILE_NEW:
			SetDlgItemText(hwnd, IDC_MAIN_EDIT, "");
			break;
		case ID_FILE_OPEN:
			DoFileOpen(hwnd);
			break;
		case ID_FILE_SAVEAS:
			DoFileSave(hwnd);
			break;
		case ID_HELP_ABOUT:
			int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
			if (ret == -1) {
				MessageBox(hwnd, "Dialog failed!", "Error", MB_OK | MB_ICONINFORMATION);
			}
			break;
		}
	}

	// Case for learning purposes for a while.
	case WM_LBUTTONDOWN:{
		// Empty for now.
	}
		break;

	case WM_CLOSE: 
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

// Dialog box procedure definition.
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		// Handle all processing that needs to be done before the dialog appears.
		case WM_INITDIALOG: return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd, IDOK);
					break;
			}
			break;
		default: return FALSE;
	}
	return TRUE;
}

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