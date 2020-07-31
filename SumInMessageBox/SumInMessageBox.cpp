#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


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
	
	TCHAR str[255];

	switch (iMsg) {

	case WM_CREATE:
		
		wsprintf(str, TEXT("Sum of numbers %d and %d is : %d"), 5, 5, 5+5);
		MessageBox(hwnd, str, TEXT("Attention!..."), MB_OKCANCEL);
		break;
		
	case WM_PAINT:



		break;

	case WM_DESTROY:
		
		
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("MutliThreading in Action ..."), TEXT("Message!"), MB_OKCANCEL);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

