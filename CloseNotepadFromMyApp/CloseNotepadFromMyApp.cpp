#include<windows.h>
#define MYTIMER 1005

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWndProc(HWND, LPARAM);
TCHAR buff[255];
TCHAR chText[255];
int iPaintFlag = 0;

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

		HDC hdc;
		RECT rc;
		PAINTSTRUCT ps;
		static HBRUSH hBrush[8];
		

	case WM_CREATE:
		
		hBrush[0] = CreateSolidBrush(RGB(0, 0, 0));
		hBrush[1] = CreateSolidBrush(RGB(255, 255, 255));
		hBrush[2] = CreateSolidBrush(RGB(255, 0, 0));
		hBrush[3] = CreateSolidBrush(RGB(0, 255, 0));
		hBrush[4] = CreateSolidBrush(RGB(0, 0, 255));
		hBrush[5] = CreateSolidBrush(RGB(255, 255, 0));
		hBrush[6] = CreateSolidBrush(RGB(0, 255, 255));
		hBrush[7] = CreateSolidBrush(RGB(255, 0, 255));

		SetTimer(hwnd, MYTIMER, 1000, NULL);
		break;



	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Sre you sure you want to quit!!"), TEXT("Close Window"), MB_ICONWARNING | MB_OKCANCEL);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:

		GetKeyNameText(lParam, chText, 255);
		InvalidateRect(hwnd, NULL, true);
		EnumWindows(&EnumWndProc, lParam);
		break;

	case WM_TIMER:
		
		KillTimer(hwnd, MYTIMER);

		iPaintFlag++;
		InvalidateRect(hwnd, NULL, true);

		SetTimer(hwnd, MYTIMER, 500, NULL);
		if (iPaintFlag > 7)
			iPaintFlag = 0;

		break;

	case WM_PAINT:

		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, hBrush[iPaintFlag]);
		FillRect(hdc, &rc, hBrush[iPaintFlag]);

		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, chText, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);

		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam) {

	GetClassName(hWnd, buff, 255);
	if (lstrcmp(buff, TEXT("Notepad")) == 0) {
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	}
	return true;
}

