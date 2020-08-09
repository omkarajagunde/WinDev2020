#include<windows.h>

#define MYTIMER 200

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("SineWaveProgram");
	HWND hwnd;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(
	
		szAppName,
		TEXT("Taurus Sign"),
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

	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {


	static RECT rc;
	static POINT pts[12];
	HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	PAINTSTRUCT ps;
	static POINT stars[500];

	switch (iMsg) {

	case WM_CREATE:
	case WM_SIZE:
		GetClientRect(hwnd, &rc);
		pts[0].x = rc.right / 3 - 220;
		pts[0].y = rc.bottom / 3 - 80;

		pts[1].x = rc.right / 2 - 80;
		pts[1].y = rc.bottom / 3 + 20;

		pts[2].x = rc.right / 2 + 20;
		pts[2].y = rc.bottom / 2 - 20;

		pts[3].x = rc.right / 2 + 40;
		pts[3].y = rc.bottom / 2 + 10;

		pts[4].x = rc.right / 2 + rc.right / 4;
		pts[4].y = rc.bottom / 2 + 100;

		pts[5].x = rc.right / 2 + rc.right / 4 + 150;
		pts[5].y = rc.bottom / 2 + 100;

		pts[6].x = rc.right / 2 + rc.right / 4;
		pts[6].y = rc.bottom / 2 + 200;

		pts[7].x = rc.right / 2 + rc.right / 4 + 150;
		pts[7].y = rc.bottom / 2 + 100;

		pts[8].x = rc.right / 2 + rc.right / 4;
		pts[8].y = rc.bottom / 2 + 100;

		pts[9].x = rc.right / 2 + 40;
		pts[9].y = rc.bottom / 2 + 10;

		pts[10].x = rc.right / 2 - 70;
		pts[10].y = rc.bottom / 2 + 20;

		pts[11].x = rc.right / 3 - 180;
		pts[11].y = rc.right / 3 - 100;
		for (int i = 0; i < 500; i++) {
			stars[i].x = rand() % rc.right;
			stars[i].y = rand() % rc.bottom;

		}
		SetTimer(hwnd, MYTIMER, 500, NULL);
		break;

	case WM_TIMER:

		KillTimer(hwnd, MYTIMER);

		for (int i = 0; i < 500; i++) {
			stars[i].x = rand() % rc.right;
			stars[i].y = rand() % rc.bottom;

		}
		SetTimer(hwnd, MYTIMER, 500, NULL);
		InvalidateRect(hwnd, NULL, true);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		

		for (int i = 0; i < 500; i++)
			SetPixel(hdc, stars[i].x, stars[i].y, RGB(255, 255, 255));

		hPen = CreatePen(PS_DOT, 2, RGB(5, 250, 18));
		SelectObject(hdc, hPen);
		SetTextColor(hdc, RGB(5, 250, 18));
		SetBkColor(hdc, RGB(0, 0, 0));
		Polyline(hdc, pts, 12);

		DrawText(hdc, TEXT("Taurus Constillation"),-1, &rc, DT_VCENTER | DT_CENTER);
		
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hPen);

		for (int i = 0; i<12; i++)
		Ellipse(hdc, pts[i].x - 5, pts[i].y - 5, pts[i].x + 5, pts[i].y + 5);

		Ellipse(hdc, pts[10].x - 10, pts[10].y - 10, pts[10].x + 10, pts[10].y + 10);

		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}