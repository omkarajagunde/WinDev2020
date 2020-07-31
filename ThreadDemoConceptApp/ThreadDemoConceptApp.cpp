#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI MyThreadOne(LPVOID);
DWORD WINAPI MyThreadTwo(LPVOID);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyOmkarGame");
	HWND hwnd;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpszClassName = szAppName;
	wndClass.hInstance = hInstance;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(
		szAppName,
		TEXT("Omkar Ajagunde ..."),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	static HANDLE hThreadOne = NULL;
	static HANDLE hThreadTwo = NULL;

	switch (iMsg) {

	case WM_CREATE:
		hThreadOne = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MyThreadOne, (LPVOID)hwnd, 0, NULL);
		hThreadTwo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MyThreadTwo, (LPVOID)hwnd, 0, NULL);
		break;

	case WM_DESTROY:
		
		if (hThreadOne) {
			CloseHandle(hThreadOne);
			hThreadOne = NULL;
		}

		if (hThreadTwo) {
			CloseHandle(hThreadTwo);
			hThreadTwo = NULL;
		}

		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("MutliThreading in Action ..."), TEXT("Message!"), MB_OKCANCEL);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

DWORD WINAPI MyThreadOne(LPVOID param) {

	// local Variables
	HDC hdc;
	long num;
	TCHAR str[255];

	// code
	hdc = GetDC((HWND) param);
	for (num = 0; num < ULONG_MAX; num++) {
		wsprintf(str, TEXT("Thread-1: Counting : %ld"), num);
		TextOut(hdc, 100, 100, str, wcslen(str));
	}

	ReleaseDC((HWND) param, hdc);
	return 0;
}


DWORD WINAPI MyThreadTwo(LPVOID param) {

	// local Variables
	HDC hdc;
	long int num;
	TCHAR str[255];

	// code
	hdc = GetDC((HWND)param);
	for (num = ULONG_MAX; num > 0; num--) {
		wsprintf(str, TEXT("Thread-2: Counting : %ld"), num);
		TextOut(hdc, 200, 200, str, wcslen(str));
	}

	ReleaseDC((HWND)param, hdc);
	return 0;
}
