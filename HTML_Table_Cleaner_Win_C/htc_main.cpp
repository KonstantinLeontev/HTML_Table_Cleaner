// The WinAPI code based on the winprog.org tutorials
// and examples from MSDN.
//////////////////////////////////////////////////////

#include "htc_controls.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	// Carries information used to load common control classes.
	INITCOMMONCONTROLSEX initCtrls{};
	BOOL result;

	// size of the structure.
	initCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	// Set of bit flags that indicate which common control classes will be loaded from DLL.
	// ICC_BAR_CLASSES load toolbar, status bar, trackbar, and tooltip control classes.
	initCtrls.dwICC = ICC_BAR_CLASSES;

	// Ensures that common control DLL is loaded, and registers specific common controls
	// classes from DLL
	result = InitCommonControlsEx(&initCtrls);
	if (!result) {
		MessageBox(NULL, "InitCommonControlsEx() failed.", "Error", MB_OK | MB_ICONERROR);
	}

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
		// Create main window menu.
		if (!CreateMainWindowMenu(hwnd)) {
			MessageBox(hwnd, "Can't create main window menu. CreateMainWindowMenu() failed!", "Error", MB_OK | MB_ICONERROR);
		}

		// Create the main toolbar.
		HWND hwndToolBar = CreateMainToolbar(hwnd);
		if (!hwndToolBar) {
			MessageBox(NULL, "CreateMainToolbar() failed!", "Error", MB_OK | MB_ICONERROR);
		}

		// Create main window edit text box.
		HWND hwndEditBox = CreateMainEditBox(hwnd);
		if (!hwndEditBox) {
			MessageBox(hwnd, "Can't create main window edit box. CreateMainWindowEditBox() failed!", "Error", MB_OK | MB_ICONERROR);
		}

		// Create the status bar.
		HWND hwndStatus = CreateStatusBar(hwnd);
	}
		break;

	// Sent to a window after it's size has changed.
	case WM_SIZE: {
		HWND hwndToolbar{};
		RECT rcToolbar{}; // Structure that defines the coordinate of the upper-left and lower-right corner.
		int iToolHeight{};

		HWND hwndStatus{};
		RECT rcStatus{};
		int iStatusHeight{};

		HWND hwndEdit{};
		RECT rcClient{};
		int iEditHeight{};

		BOOL result{};

		// Compute the size of the toolbar.
		hwndToolbar = GetDlgItem(hwnd, IDC_MAIN_TOOL);
		if (hwndToolbar) {
			SendMessage(hwndToolbar, TB_AUTOSIZE, 0, 0);

			GetWindowRect(hwndToolbar, &rcToolbar);
			iToolHeight = rcToolbar.bottom - rcToolbar.top;
		}
		
		// Compute the size of the status bar.
		hwndStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
		if (hwndStatus) {
			SendMessage(hwndStatus, WM_SIZE, 0, 0);

			GetWindowRect(hwndStatus, &rcStatus);
			iStatusHeight = rcStatus.bottom - rcStatus.top;
		}

		// Calculate the size of the edit box.
		// Retrieves a handle to a control in the specified dialog box.
		hwndEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		if (hwndEdit && hwndStatus) {
			// Retrieves the coordinate of a  main window's client area.
			result = GetClientRect(hwnd, &rcClient);
			if (!result) {
				MessageBox(hwnd, "Couldn't get the client area size.", "Error", MB_OK | MB_ICONERROR);
			}

			// Calculate the height of edit area.
			iEditHeight = rcClient.bottom - iToolHeight - iStatusHeight;

			result = SetWindowPos(
				hwndEdit, // Handle to the window.
				NULL, // Handle to the window to precede the positioned window in the Z order.
				0, // Position of the left side.
				iToolHeight, // Position of the top side - it's just below the toolbar.
				rcClient.right - 200, // Width in pixels.
				iEditHeight, // Height in pixels.
				SWP_NOZORDER // The window sizing and position flag. This one retains the current Z order.
			);
			if (!result) {
				MessageBox(hwnd, "Could not set editbox's position.\nSetWindowPos() was failed.", "Error", MB_OK | MB_ICONERROR);
			}
		}	
	}
		break;

	// Sent when the user selects a command item from a menu.
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDM_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case IDM_FILE_NEW:
			SetDlgItemText(hwnd, IDC_MAIN_EDIT, "");
			break;
		case IDM_FILE_OPEN:
			DoFileOpen(hwnd);
			break;
		case IDM_FILE_SAVEAS:
			DoFileSave(hwnd);
			break;
		case IDM_HELP_ABOUT:
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