#include <windows.h>
#define MYTIMER 200


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

	int xCalc = GetSystemMetrics(SM_CXSCREEN);
	int yCalc = GetSystemMetrics(SM_CYSCREEN);

	xCalc -= 800;
	yCalc -= 800;

	xCalc /= 2;
	yCalc /= 2;

	hwnd = CreateWindow(
		szAppName,
		TEXT("Omkar Ajagunde ..."),
		WS_OVERLAPPEDWINDOW,
		xCalc,
		yCalc,
		800,
		800,
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


	static HBRUSH hBrush;
	static HPEN hPen;
	HDC hdc;
	static RECT rc;
	PAINTSTRUCT ps;

	

	switch (iMsg) {

	case WM_CREATE:
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(RGB(246, 249, 22));
		hPen = CreatePen(PS_SOLID, 8, RGB(189, 189, 188));
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		// Draw Face
		Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);

		// Left Eye
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, rc.right / 2 - 250, rc.bottom / 2 - 250, rc.right / 2 - 50, rc.bottom / 2);

		hBrush = CreateSolidBrush(RGB(60, 152, 251));
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Ellipse(hdc, rc.right / 2 - 200, rc.bottom / 2 - 200, rc.right / 2 - 70, rc.bottom / 2 - 70);

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Ellipse(hdc, rc.right / 2 - 170, rc.bottom / 2 - 170, rc.right / 2 - 90, rc.bottom / 2 - 90);

		
		// Right Eye
		hPen = CreatePen(PS_SOLID, 8, RGB(189, 189, 188));
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Ellipse(hdc, rc.right / 2 + 50, rc.bottom / 2 - 250, rc.right / 2 + 250, rc.bottom / 2);

		hBrush = CreateSolidBrush(RGB(60, 152, 251));
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Ellipse(hdc, rc.right / 2 + 70, rc.bottom / 2 - 200, rc.right / 2 + 200, rc.bottom / 2 - 70);

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		hPen = (HPEN)GetStockObject(NULL_PEN);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Ellipse(hdc, rc.right / 2 + 90, rc.bottom / 2 - 170, rc.right / 2 + 170, rc.bottom / 2 - 90);

		// Nose



		// Mouth
		hPen = CreatePen(PS_SOLID, 10, RGB(189, 189, 188));
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Arc(hdc, rc.right / 2 - 250, rc.bottom / 2 + 100, rc.right / 2 + 250, rc.bottom / 2 + 250, rc.right / 2 - 250, rc.bottom / 2 + 200, rc.right / 2 + 250, rc.bottom / 2 + 200);

		// Tongue
		hPen = CreatePen(PS_SOLID, 40, RGB(251, 60, 239));
		hBrush = CreateSolidBrush(RGB(255, 128, 0));
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Arc(hdc, rc.right / 2 - 50, rc.bottom / 2 + 250, rc.right / 2 + 50, rc.bottom / 2 + 320, rc.right / 2 - 250, rc.bottom / 2 + 200, rc.right / 2 + 250, rc.bottom / 2 + 200);


		DeleteObject(hBrush);
		DeleteObject(hPen);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
