#include <windows.h>

// Menu symbols.
#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

const char g_szClassName[] = "myWindowClass";

// Window procedure declaration.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	case WM_CREATE:
	{
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
		AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");

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
	}
		break;
	case WM_LBUTTONDOWN:
		// Show the path of our programm.
	{
		char szFileName[MAX_PATH];
		// Get handle to current programm.
		HINSTANCE hInstance = GetModuleHandle(NULL);

		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hwnd, szFileName, "This program is: ", MB_OK | MB_ICONINFORMATION);
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