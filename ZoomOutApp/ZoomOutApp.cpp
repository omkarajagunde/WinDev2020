#include<windows.h>
#define COLORTIMER 500
#define CHANGERECT 600

HBRUSH hBrush[8];
int hColorChangeFlag;
RECT rc;
RECT rcCopy;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK ChangeRectProc(HWND, UINT, UINT_PTR, DWORD);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmsLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ZoomOutRectangleApp");

	wndClass.lpszClassName = szAppName;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.style = CS_HREDRAW | CS_HREDRAW;
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.lpszMenuName = NULL;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(

		szAppName,
		TEXT("ZoomOutReactangleAnimation -OmkarAjagunde ..."),
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


	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR wndPts[256];


	switch (iMsg) {

	case WM_CREATE:
		GetClientRect(hwnd, &rc);
		wsprintf(wndPts, TEXT("top-left (x, y): (%d, %d)\nlower-right (x, y): (%d, %d) "), rc.left, rc.top, rc.right, rc.bottom);
		MessageBox(hwnd, wndPts, TEXT("Client Area Co-ordinates"), 0);
		hBrush[0] = CreateSolidBrush(RGB(0, 0, 0));
		hBrush[1] = CreateSolidBrush(RGB(255, 255, 255));
		hBrush[2] = CreateSolidBrush(RGB(255, 0, 0));
		hBrush[3] = CreateSolidBrush(RGB(0, 255, 0));
		hBrush[4] = CreateSolidBrush(RGB(0, 0, 255));
		hBrush[5] = CreateSolidBrush(RGB(255, 255, 0));
		hBrush[6] = CreateSolidBrush(RGB(0, 255, 255));
		hBrush[7] = CreateSolidBrush(RGB(255, 0, 255));

		SetTimer(hwnd, COLORTIMER, 2000, NULL);
		SetTimer(hwnd, CHANGERECT, 10, ChangeRectProc);

		break;

	case WM_PAINT:
		//rcCopy = rc;
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hBrush[hColorChangeFlag]);
		//FillRect(hdc, &rcCopy, hBrush[hColorChangeFlag]);
		Ellipse(hdc, rcCopy.left, rcCopy.top, rcCopy.right, rcCopy.bottom);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:

		KillTimer(hwnd, COLORTIMER);
		hColorChangeFlag++;
		InvalidateRect(hwnd, &rc, true);
		SetTimer(hwnd, COLORTIMER, 2000, NULL);
		if (hColorChangeFlag > 7)
			hColorChangeFlag = 0;

		break;

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye !, Saad you are going !"), TEXT("Message:"), MB_OKCANCEL | MB_ICONWARNING);
		PostQuitMessage(0);
		break;

	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void CALLBACK ChangeRectProc(HWND hwnd, UINT timerMsg, UINT_PTR timerEvent, DWORD ms) {

	KillTimer(hwnd, CHANGERECT);
	rcCopy.left += 5;
	rcCopy.top += 5;
	rcCopy.right -= 5;
	rcCopy.bottom -= 5;
	SetTimer(hwnd, CHANGERECT, 10, ChangeRectProc);
	InvalidateRect(hwnd, NULL, true);

	if (rcCopy.top > rcCopy.bottom)
		rcCopy = rc;
}

