#pragma once
#include <windows.h>
#include "resource.h"
#include <CommCtrl.h>

// Menu symbols.
#define IDM_FILE_NEW 9000
#define IDM_FILE_OPEN 9001
#define IDM_FILE_SAVEAS 9002
#define IDM_FILE_EXIT 9003
#define IDM_HELP_ABOUT 9004
// Text editor symbol.
#define IDC_MAIN_EDIT 101

const char g_szClassName[] = "myWindowClass";

// Handle to the image list for toolbar icons declaration.
extern HIMAGELIST g_hImageList;

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

// Create main window menu.
BOOL CreateMainWindowMenu(HWND hwndParent);

// Create main window edit text box.
BOOL CreateMainEditBox(HWND hwndParent);

// Create the main toolbar.
HWND CreateMainToolbar(HWND hwndParent);