#include <windows.h>
#include "resource.h"

// Menu symbols.
#define ID_FILE_EXIT 9001
#define ID_HELP_ABOUT 9002
// Text editor symbol.
#define IDC_MAIN_EDIT 101

const char g_szClassName[] = "myWindowClass";

// Window procedure declaration.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Dialog box procedure declaration.
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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