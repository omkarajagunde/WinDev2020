#include<windows.h>
#include<Mmsystem.h>

#define PREV_AUDIO 21
#define PREV 22
#define PLAY_PAUSE 23
#define FWD 24
#define NEXT_AUDIO 25

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE ghInstance = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpSzCmdLine, int iCmdShow) {


	ghInstance = hInstance;

	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyAudioPlayerApp");

	wndClass.lpszClassName = szAppName;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszMenuName = NULL;
	wndClass.hInstance = hInstance;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	int xCalc = GetSystemMetrics(SM_CXSCREEN);
	int yCalc = GetSystemMetrics(SM_CYSCREEN);

	xCalc -= 800;
	xCalc /= 2;

	yCalc -= 500;
	yCalc /= 2;

	hwnd = CreateWindow(szAppName,
		TEXT("MyAudioPlayer -Omkar -Ajagunde"),
		WS_OVERLAPPEDWINDOW,
		xCalc,
		yCalc,
		800,
		500,
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


	static HWND playPauseButton = NULL;
	static HWND fwdButton = NULL;
	static HWND prevButton = NULL;
	static HWND nextAudioButton = NULL;
	static HWND prevAudioButton = NULL;

	// Drawing variables
	static RECT rc;
	static HDC hdc;
	static PAINTSTRUCT ps;
	static HBRUSH hBrush;
	static HPEN hPen;
	static POINT pts[4];

	static int playPauseButton_xCalc, _yCalc;
	static int prevAudioButton_xCalc;
	static int nextAudioButton_xCalc;
	static int fwdButton_xCalc;
	static int prevButton_xCalc;

	static LPDRAWITEMSTRUCT pdis;
	static int flag = 0;

	switch (iMsg) {

	case WM_CREATE:

		GetClientRect(hwnd, &rc);

		// previous audio
		prevAudioButton_xCalc = rc.right / 4;

		// go back 15 secs
		prevButton_xCalc = rc.right / 4 + 100;

		// Play Pause Button
		_yCalc = rc.bottom - 60;
		playPauseButton_xCalc = rc.right / 2 - 25;

		// fast Forward 15 secs
		fwdButton_xCalc = rc.right - rc.right / 4 - 150;

		// next audio
		nextAudioButton_xCalc = fwdButton_xCalc + 100;
		
		prevAudioButton = CreateWindow(
			TEXT("button"),
			TEXT("<--"),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			prevAudioButton_xCalc,
			_yCalc,
			50,
			50,
			hwnd,
			(HMENU)PREV_AUDIO,
			ghInstance,
			NULL
		);

		prevButton = CreateWindow(
			TEXT("button"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			prevButton_xCalc,
			_yCalc,
			50,
			50,
			hwnd,
			(HMENU)PREV,
			ghInstance,
			NULL
		);

		playPauseButton = CreateWindow(
			TEXT("button"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW ,
			playPauseButton_xCalc,
			_yCalc,
			50,
			50,
			hwnd,
			(HMENU) PLAY_PAUSE,
			ghInstance,
			NULL
			);

		fwdButton = CreateWindow(
			TEXT("button"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			fwdButton_xCalc,
			_yCalc,
			50,
			50,
			hwnd,
			(HMENU)FWD,
			ghInstance,
			NULL
		);

		nextAudioButton = CreateWindow(
			TEXT("button"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			nextAudioButton_xCalc,
			_yCalc,
			50,
			50,
			hwnd,
			(HMENU)NEXT_AUDIO,
			ghInstance,
			NULL
		);

		break;

	case WM_COMMAND:

		GetWindowRect(hwnd, &rc);
		pdis = (LPDRAWITEMSTRUCT)lparam;

		switch (wparam) {
		case PREV_AUDIO:
			
			break;

		case PLAY_PAUSE:

			if (flag == 0) {
				SendMessage(playPauseButton, BM_SETSTATE, 1, 0);
				flag = 1;
			}
			else {
				SendMessage(playPauseButton, BM_SETSTATE, 0, 0);
				flag = 0;
			}
			
			break;

		}

		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		SelectObject(hdc, hPen);


		Rectangle(hdc, rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1);

		MoveToEx(hdc, 0, rc.bottom - 100, NULL);
		LineTo(hdc, rc.right, rc.bottom - 100);

		MoveToEx(hdc, 0, rc.bottom - 70, NULL);
		LineTo(hdc, rc.right, rc.bottom - 70);

		MoveToEx(hdc, rc.right/8, rc.bottom - 70, NULL);
		LineTo(hdc, rc.right/8, rc.bottom);

		MoveToEx(hdc, rc.right - (rc.right / 8), rc.bottom - 70, NULL);
		LineTo(hdc, rc.right - (rc.right / 8), rc.bottom);
		
		break;

	case WM_DRAWITEM:

		pdis = (LPDRAWITEMSTRUCT)lparam;

		FillRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
		switch (pdis->CtlID) {
			
		case PREV_AUDIO:

			hBrush = CreateSolidBrush(RGB(142, 89, 253));
			SelectObject(pdis->hDC, hBrush);
			RoundRect(pdis->hDC, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom, 8, 8);


			if (pdis->itemState & ODS_SELECTED) {
				hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				SelectObject(pdis->hDC, hPen);
				PlaySound("MouseClick", NULL, SND_ASYNC);

			}
			else {
				hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
				SelectObject(pdis->hDC, hPen);
			}

			pts[0].x = pdis->rcItem.right - 30;
			pts[0].y = pdis->rcItem.top + 15;

			pts[1].x = pdis->rcItem.left + 10;
			pts[1].y = pdis->rcItem.bottom / 2;

			pts[2].x = pdis->rcItem.right - 30;
			pts[2].y = pdis->rcItem.bottom - 15;

			Polyline(pdis->hDC, pts, 3);

			MoveToEx(pdis->hDC, pdis->rcItem.left + 10, pdis->rcItem.bottom / 2,NULL);
			LineTo(pdis->hDC, pdis->rcItem.left + 40, pdis->rcItem.bottom / 2);

			
			break;

		case PREV:

			hBrush = CreateSolidBrush(RGB(255, 0, 127));
			SelectObject(pdis->hDC, hBrush);
			RoundRect(pdis->hDC, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom, 8, 8);


			if (pdis->itemState & ODS_SELECTED) {
				hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				SelectObject(pdis->hDC, hPen);
				PlaySound("MouseClick", NULL, SND_ASYNC);

			}
			else {
				hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
				SelectObject(pdis->hDC, hPen);
			}
			pts[0].x = pdis->rcItem.right - 10;
			pts[0].y = pdis->rcItem.top + 10;

			pts[1].x = pdis->rcItem.left + 20;
			pts[1].y = pdis->rcItem.bottom / 2;

			pts[2].x = pdis->rcItem.right - 10;
			pts[2].y = pdis->rcItem.bottom - 10;

			Polyline(pdis->hDC, pts, 3);

			pts[0].x -= 20;
			pts[0].y += 5;

			pts[1].x -= 10;

			pts[2].x -= 20;
			pts[2].y -= 5;

			Polyline(pdis->hDC, pts, 3);

			break;

		case PLAY_PAUSE:

			hBrush = CreateSolidBrush(RGB(255, 0, 127));
			SelectObject(pdis->hDC, hBrush);
			RoundRect(pdis->hDC, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom, 8, 8);
			

			if (pdis->itemState & ODS_SELECTED) {

					PlaySound("MouseClick", NULL, SND_ASYNC);
					hBrush = CreateSolidBrush(RGB(255, 255, 255));
					SelectObject(pdis->hDC, hBrush);
					RoundRect(pdis->hDC, pdis->rcItem.left + 10, pdis->rcItem.top + 10, pdis->rcItem.right - 30, pdis->rcItem.bottom - 10, 4, 4);
					RoundRect(pdis->hDC, pdis->rcItem.left + 30, pdis->rcItem.top + 10, pdis->rcItem.right - 10, pdis->rcItem.bottom - 10, 4, 4);
					
			}
			else {

					PlaySound("MouseClick", NULL, SND_ASYNC);
					pts[0].x = pdis->rcItem.left + 10;
					pts[0].y = pdis->rcItem.top + 10;

					pts[1].x = pdis->rcItem.left + 10;
					pts[1].y = pdis->rcItem.bottom - 10;

					pts[2].x = pdis->rcItem.right - 10;
					pts[2].y = pdis->rcItem.bottom / 2;

					pts[3].x = pdis->rcItem.left + 10;
					pts[3].y = pdis->rcItem.top + 10;

					DeleteObject(hBrush);
					hBrush = CreateSolidBrush(RGB(255, 255, 255));
					SelectObject(pdis->hDC, hBrush);

					Polygon(pdis->hDC, pts, 4);
				
			}
			break;

		case FWD:

			hBrush = CreateSolidBrush(RGB(255, 0, 127));
			SelectObject(pdis->hDC, hBrush);
			RoundRect(pdis->hDC, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom, 8, 8);


			if (pdis->itemState & ODS_SELECTED) {
				hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				SelectObject(pdis->hDC, hPen);
				PlaySound("MouseClick", NULL, SND_ASYNC);

			}
			else {
				hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
				SelectObject(pdis->hDC, hPen);
			}

			pts[0].x = pdis->rcItem.left + 10;
			pts[0].y = pdis->rcItem.top + 10;

			pts[1].x = pdis->rcItem.right - 20;
			pts[1].y = pdis->rcItem.bottom / 2;

			pts[2].x = pdis->rcItem.left + 10;
			pts[2].y = pdis->rcItem.bottom - 10;

			Polyline(pdis->hDC, pts, 3);

			pts[0].x += 20;
			pts[0].y += 5;

			pts[1].x += 10;

			pts[2].x += 20;
			pts[2].y -= 5;
			Polyline(pdis->hDC, pts, 3);
			break;

		case NEXT_AUDIO:

			hBrush = CreateSolidBrush(RGB(142, 89, 253));
			SelectObject(pdis->hDC, hBrush);
			RoundRect(pdis->hDC, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom, 8, 8);


			if (pdis->itemState & ODS_SELECTED) {
				hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				SelectObject(pdis->hDC, hPen);
				PlaySound("MouseClick", NULL, SND_ASYNC);

			}
			else {
				hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
				SelectObject(pdis->hDC, hPen);
			}

			pts[0].x = pdis->rcItem.left + 30;
			pts[0].y = pdis->rcItem.top + 15;

			pts[1].x = pdis->rcItem.right - 10;
			pts[1].y = pdis->rcItem.bottom / 2;

			pts[2].x = pdis->rcItem.left + 30;
			pts[2].y = pdis->rcItem.bottom - 15;

			Polyline(pdis->hDC, pts, 3);

			MoveToEx(pdis->hDC, pdis->rcItem.left + 10, pdis->rcItem.bottom / 2, NULL);
			LineTo(pdis->hDC, pdis->rcItem.left + 40, pdis->rcItem.bottom / 2);

			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}
