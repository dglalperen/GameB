#include <stdio.h>
#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)
#include "Main.h"

HANDLE gGameWindow;

BOOL gGameIsRunning;

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

	gGameIsRunning = TRUE;

	while (gGameIsRunning == TRUE) {
		while (PeekMessageA(&Message, gGameWindow, 0, 0, PM_REMOVE)) {
			DispatchMessageA(&Message);
		}

		ProcessPlayerInput();

		// RenderFrameGraphics();

		Sleep(1);
	}

Exit:
	return (0);
}
LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{

	case WM_CLOSE:
		gGameIsRunning = FALSE;

		PostQuitMessage(0);
		break;
	default:
		Result = DefWindowProc(WindowHandle, Message, WParam, LParam);
	}
	return (Result);
}

DWORD CreateMainGameWindow(void) {
	DWORD Result = ERROR_SUCCESS;

	WNDCLASSEXA WindowClass = { 0 };

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

	gGameWindow = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		WindowClass.lpszClassName,
		GAME_NAME,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, GetModuleHandleA(NULL), NULL);

	if (gGameWindow == NULL)
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

void ProcessPlayerInput(void) {
	short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);

	if (EscapeKeyIsDown) {
		SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
	}
}
