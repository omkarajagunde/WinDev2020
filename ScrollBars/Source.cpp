#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ScrollProc(HWND, UINT, WPARAM, LPARAM);

int idFocus;
WNDPROC OldScroll[3];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpSzCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ScrollBarApp");

	wndClass.lpszClassName = szAppName;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszMenuName = NULL;
	wndClass.hInstance = hInstance;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(szAppName,
		TEXT("Scroll Bars -Omkar -Ajagunde"),
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam) {

	static COLORREF crPrim[3] = {RGB(255,0,0), RGB(0,255,0) ,RGB(0,0,255) };
	static HBRUSH hBrush[3], hBrushStatic;
	static HWND hwndScroll[3], hwndLabel[3], hwndValue[3], hwndRect;
	static int color[3], cyChar;
	static RECT rcColor;
	static TCHAR* szColorLabel[] = { TEXT("RED"), TEXT("GREEN") , TEXT("BLUE") };
	HINSTANCE hInstance;
	int i, cxClient, cyClient;
	TCHAR szBuffer[10];

	switch (iMsg) {

	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

		hwndRect = CreateWindow(TEXT("static"), NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT, 0, 0, 0, 0, hwnd, (HMENU)9, hInstance, NULL);

		for (i = 0; i < 3; i++) {
			 
			hwndScroll[i] = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT, 0, 0, 0, 0, hwnd, (HMENU)i, hInstance, NULL);
			SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
			SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

			hwndLabel[i] = CreateWindow(TEXT("static"), szColorLabel[i], WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hwnd, (HMENU)(i + 3), hInstance, NULL);

			hwndValue[i] = CreateWindow(TEXT("static"), TEXT("0"), WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hwnd, (HMENU)(i + 6), hInstance, NULL);

			OldScroll[i] = (WNDPROC)SetWindowLong(hwndScroll[i], GWL_WNDPROC, (LONG)ScrollProc);

			hBrush[i] = CreateSolidBrush(crPrim[i]);
		}

		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		cyChar = HIWORD(GetDialogBaseUnits());
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);

		SetRect(&rcColor, cxClient / 2, 0, cxClient, cyClient);
		MoveWindow(hwndRect, 0, 0, cxClient / 2, cyClient, TRUE);

		for (i = 0; i < 3; i++) {
			MoveWindow(hwndScroll[i], (2 * i + 1) * cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar, TRUE);
			MoveWindow(hwndLabel[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
			MoveWindow(hwndValue[i], (4 * i + 1) * cxClient / 28, cyClient - 3 * cyChar / 2, cxClient / 7, cyChar, TRUE);
		}

		SetFocus(hwnd);
		return 0;

	case WM_SETFOCUS:
		SetFocus(hwndScroll[idFocus]);
		return 0;

	case WM_VSCROLL:
		i = GetWindowLong((HWND)lparam, GWL_ID);

		switch (LOWORD(wparam)) {

		case SB_PAGEDOWN:
			color[i] += 15;

		case SB_LINEDOWN:
			color[i] = min(255, color[i] + 1);
			break;

		case SB_PAGEUP:
			color[i] = max(0, color[i] - 1);
			break;

		case SB_TOP:
			color[i] = 0;
			break;

		case SB_BOTTOM:
			color[i] = 255;
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			color[i] = HIWORD(wparam);
			break;

		default:
			break;
		}

		SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
		wsprintf(szBuffer, TEXT("%i"), color[i]);
		SetWindowText(hwndValue[i], szBuffer);

		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(color[0], color[1], color[2]))));
		InvalidateRect(hwnd, &rcColor, TRUE);
		return 0;

	case WM_CTLCOLORSCROLLBAR:
		i = GetWindowLong((HWND)lparam, GWL_ID);
		return (LRESULT)hBrush[i];

	case WM_CTLCOLORSTATIC:
		i = GetWindowLong((HWND)lparam, GWL_ID);

		if (i >= 3 && i <= 8) {
			SetTextColor((HDC)wparam, crPrim[i % 3]);
			SetBkColor((HDC) wparam, GetSysColor (COLOR_BTNHIGHLIGHT));
			return (LRESULT)hBrushStatic;
		}
		break;

	case WM_SYSCOLORCHANGE:
		DeleteObject(hBrushStatic);
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		return 0;

	case WM_DESTROY:
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)GetStockObject(WHITE_BRUSH)));

		for (i = 0; i < 3; i++)
			DeleteObject(hBrush[i]);

		DeleteObject(hBrushStatic);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}

LRESULT CALLBACK ScrollProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

	int id = GetWindowLong(hwnd, GWL_ID);

	switch (message) {

	case WM_KEYDOWN:

		if (wparam == VK_TAB)
			SetFocus(GetDlgItem(GetParent(hwnd), (id + (GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));

		break;

	case WM_SETFOCUS:
		idFocus = id;
		break;
	}
	return CallWindowProc(OldScroll[id], hwnd, message, wparam, lparam);
}
