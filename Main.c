#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

LRESULT CALLBACK MainWindowProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam);

DWORD CreateMainGameWindow();

INT WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{

	UNREFERENCED_PARAMETER(PrevInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);

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

LRESULT CALLBACK MainWindowProc(
	HWND WindowHandle,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam)
{
	switch (Message)
	{
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
	return 0;
}

DWORD CreateMainGameWindow() {
	DWORD Result = ERROR_SUCCESS;

	WNDCLASSEXA WindowClass = { 0 };
	HWND WindowHandle = { 0 };

	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = MainWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL); // its the same as passing Instance
	WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = "GAME_B_WINDOWCLASS";

	if (RegisterClassExA(&WindowClass) == 0)
	{
		Result = GetLastError();

		MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		goto Exit;
	}

	WindowHandle = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		WindowClass.lpszClassName,
		"Game B",
		WS_OVERLAPPEDWINDOW,
		CWMO_DEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (WindowHandle == NULL)
	{
		Result = GetLastError();
		MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

Exit:

	return Result;
}
