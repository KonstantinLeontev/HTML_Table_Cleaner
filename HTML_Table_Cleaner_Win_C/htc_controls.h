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

#define IDC_MAIN_EDIT 101 // Text editor symbol.
#define IDC_MAIN_TOOL 102 // Main toolbar.
#define IDC_MAIN_STATUS 103 // Main status bar.


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

// Create main window menu.
BOOL CreateMainWindowMenu(HWND hwndParent);

// Create main window edit text box.
HWND CreateMainEditBox(HWND hwndParent);

// Create the main toolbar.
HWND CreateMainToolbar(HWND hwndParent);

// Create the status bar.
HWND CreateStatusBar(HWND hwndParent);