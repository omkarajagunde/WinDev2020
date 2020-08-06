#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE hMutex = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	if (hMutex == NULL) 
		CreateMutex(NULL, FALSE, TEXT("OAMUTEX"));
	else
		exit(0);

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


	static HBRUSH hBrush[8];
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;
	TCHAR str[] = TEXT("HELLO WORLD ! @ ASTROMDEICOMP ...");
	static int iPaintFlag;

	switch (iMsg) {

	case WM_CREATE:


		break;

	case WM_PAINT:

		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		SetBkColor(hdc, RGB(255, 255, 255));

		if (iPaintFlag == 1)
			SetTextColor(hdc, RGB(255, 0, 0));

		if (iPaintFlag == 2)
			SetTextColor(hdc, RGB(0, 255, 0));

		if (iPaintFlag == 3)
			SetTextColor(hdc, RGB(0, 0, 255));

		DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		EndPaint(hwnd, &ps);

		break;

	case WM_CHAR:

		switch (wParam) {

		case 'r':
		case 'R':

			iPaintFlag = 1;
			InvalidateRect(hwnd, NULL, true);

			break;


		case 'g':
		case 'G':

			iPaintFlag = 2;
			InvalidateRect(hwnd, NULL, true);

			break;
		
		case 'b':
		case 'B':

			iPaintFlag = 3;
			InvalidateRect(hwnd, NULL, true);

			break;

		}

		break;

	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;

	
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
