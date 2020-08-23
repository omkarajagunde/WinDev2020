#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWndProc(HWND, LPARAM);
TCHAR buff[255];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("MyApp");

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	wndclass.lpszClassName = szName;
	wndclass.lpszMenuName = NULL;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szName, TEXT("Omkar -Ajagunde"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,

		NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	switch (imsg) {

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Sre you sure you want to quit!!"), TEXT("Close Window"), MB_ICONWARNING | MB_OKCANCEL);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		EnumWindows(&EnumWndProc, 0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam) {

	GetClassName(hWnd, buff, 255);
	if (lstrcmp(buff, TEXT("Notepad")) == 0) {
		MessageBox(hWnd, TEXT("GOT NOTEPAD"), TEXT("SUCCESS"), MB_OK);
		DestroyWindow(hWnd);
	}
	return true;
}