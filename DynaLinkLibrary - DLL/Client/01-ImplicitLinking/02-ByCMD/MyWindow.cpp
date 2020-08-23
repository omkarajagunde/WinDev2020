#include<windows.h>
#include "08_MyMath.h"

//#pragma comment(lib, "MyMath.lib")

// window procedure callback
LRESULT CALLBACK MyWndProcedure(HWND, UINT, WPARAM, LPARAM);

int iPaintFlag;

// WinMain() -> __WinMainCRTStartup() -> main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindowApp");
	HWND hwnd;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = MyWndProcedure;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hInstance = hInstance;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	int xCalc = GetSystemMetrics(SM_CXSCREEN);
	int yCalc = GetSystemMetrics(SM_CYSCREEN);
	xCalc -= 800;
	xCalc /= 2;

	yCalc -= 600;
	yCalc /= 2;

	hwnd = CreateWindow(szAppName,
		TEXT("My 2ND Window -WinDev2020"),
		WS_OVERLAPPEDWINDOW,
		xCalc,
		yCalc,
		800,
		600,
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

LRESULT CALLBACK MyWndProcedure(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam)
{
	
	int i = 5;
	int j;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
		
		j = MakeSquare(i);
		wsprintf(str, TEXT("Square of %d is %d"), i, j);
		MessageBox(hwnd, str, TEXT("Square Via Implicit DLL Linking ..."), MB_OK);

		break;
	case WM_PAINT:

		PAINTSTRUCT ps;
		HDC hdc;
		RECT rc;

		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		if (iPaintFlag == 1)
			SetBkColor(hdc, RGB(255, 255, 255));
		if (iPaintFlag == 2)
			SetBkColor(hdc, RGB(0, 255, 255));
		if (iPaintFlag == 3)
			SetBkColor(hdc, RGB(0, 255, 0));
		if (iPaintFlag == 4)
			SetBkColor(hdc, RGB(255, 0, 255));
		if (iPaintFlag == 5)
			SetBkColor(hdc, RGB(0, 0, 255));
		if (iPaintFlag == 6)
			SetBkColor(hdc, RGB(255, 255, 0));

		SetTextColor(hdc, RGB(255, 0, 0));
		DrawText(hdc, TEXT("Hello World !!!"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
		EndPaint(hwnd, &ps);
		break;
	
	case WM_KEYDOWN:
		// A
		if (wparam == 0x41) {
			iPaintFlag = 1;
			InvalidateRect(hwnd, NULL, true);
		}
		// C
		if (wparam == 0x43) {
			iPaintFlag = 2;
			InvalidateRect(hwnd, NULL, true);
		}
		// G
		if (wparam == 0x47) {
			iPaintFlag = 3;
			InvalidateRect(hwnd, NULL, true);
		}
		// M
		if (wparam == 0x4D) {
			iPaintFlag = 4;
			InvalidateRect(hwnd, NULL, true);
		}
		// B
		if (wparam == 0x42) {
			iPaintFlag = 5;
			InvalidateRect(hwnd, &rc, true);
		}
		// Y
		if (wparam == 0x59) {
			iPaintFlag = 6;
			InvalidateRect(hwnd, NULL, true);
		}

		break;
	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye I'm saaad you are leaving mee!"), TEXT("Bye !!!"), MB_OK);
		PostQuitMessage(0);

	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}
