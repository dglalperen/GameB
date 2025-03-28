#include <stdio.h>
#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)
#include "Main.h"

INT WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{

	UNREFERENCED_PARAMETER(PrevInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);

	if (GameIsAlreadyRunning() == TRUE) {
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		goto Exit;
	}

	if (CreateMainGameWindow() != ERROR_SUCCESS) {
		goto Exit;
	}

	MSG Message = { 0 };
	while (GetMessageA(&Message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Message);

		DispatchMessageA(&Message);
	}
Exit:
	return (0);
}
LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
	switch (Message)
	{
		OutputDebugStringA(Message);
		OutputDebugStringA("\n");

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		// Initialize the window.
		return 0;

	case WM_PAINT:
		// Paint the window's client area.
		return 0;

	case WM_SIZE:
		// Set the size and position of the window.
		return 0;

	case WM_DESTROY:
		// Clean up window-specific data objects.
		return 0;

		//
		// Process other messages.
		//

	default:
		return DefWindowProc(WindowHandle, Message, WParam, LParam);
	}
	return (0);
}

DWORD CreateMainGameWindow(void) {
	DWORD Result = ERROR_SUCCESS;

	WNDCLASSEXA WindowClass = { 0 };
	HWND WindowHandle = { 0 };

	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = MainWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleA(NULL); // its the same as passing Instance
	WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";

	if (RegisterClassExA(&WindowClass) == 0)
	{
		Result = GetLastError();

		MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		goto Exit;
	}

	WindowHandle = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		WindowClass.lpszClassName,
		GAME_NAME,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, GetModuleHandleA(NULL), NULL);

	if (WindowHandle == NULL)
	{
		Result = GetLastError();
		MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

Exit:

	return Result;
}

BOOL GameIsAlreadyRunning(void) {
	HANDLE Mutex = NULL;
	Mutex = CreateMutexA(NULL, FALSE, GAME_NAME);

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return(TRUE);
	}
	else {
		return(FALSE);
	}
};
