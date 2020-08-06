#include<windows.h>
#include<stdlib.h>
#include<process.h>

// window procedure callback
LRESULT CALLBACK MyWndProcedure(HWND, UINT, WPARAM, LPARAM);
void __cdecl ThreadProcOne(void *);
void __cdecl ThreadProcTwo(void *);
void __cdecl ThreadProcThree(void *);
void __cdecl ThreadProcFour(void *);

int iPaintFlag;

BOOL gbDone = FALSE;
RECT gRect;

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
	static unsigned long hThreadOne = NULL;
	static unsigned long hThreadTwo = NULL;
	static unsigned long hThreadThree = NULL;
	static unsigned long hThreadFour = NULL;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &gRect);
		MessageBox(hwnd, TEXT("Wow did you just Started this app :: WM_CREATE"), TEXT("Welcome User!"), MB_OK);
		hThreadOne = _beginthread(ThreadProcOne, 0, (void*)hwnd);
		hThreadTwo = _beginthread(ThreadProcTwo, 0, (void*)hwnd);
		hThreadThree = _beginthread(ThreadProcThree, 0, (void*)hwnd);
		hThreadFour = _beginthread(ThreadProcFour, 0, (void*)hwnd);

		break;

	case WM_PAINT:

		
		break;
	
	case WM_SIZE:
		GetClientRect(hwnd, &gRect);
		break;

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye I'm saaad you are leaving mee!"), TEXT("Bye !!!"), MB_OK);
		PostQuitMessage(0);

	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}


void _cdecl ThreadProcOne(void* param){


	HDC hdc;
	int i;
	TCHAR str[255];

	hdc = GetDC((HWND)param);
	SetBkColor(hdc, RGB(0,0,0));
	SetTextColor(hdc, RGB(0, 255, 0));

	for (i = 0; i< 42947295; i++){

		wsprintf(str, TEXT("%ul"), i);
		TextOut(hdc, 5, 5, str, wcslen(str));

		wsprintf(str, TEXT("%ul"), 42947295 - i);
		TextOut(hdc, 5, 25, str, wcslen(str));
	}

	ReleaseDC((HWND)param, hdc);
	_endthread();
}


void __cdecl ThreadProcTwo(void* param){

	HDC hdc;
	HBRUSH hBrush;
	RECT rc;

	hdc = GetDC((HWND) param);
	while (gbDone == FALSE) {

		rc.left = gRect.right / 2 + (rand() % gRect.right / 2);
		rc.top = rand() % gRect.bottom / 2;
		rc.right = gRect.right / 2 + (rand() % gRect.right / 20);
		rc.bottom = rand() % gRect.bottom / 2;

		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		FillRect(hdc, &rc, hBrush);
		DeleteObject(hBrush);

	}

	ReleaseDC((HWND)param, hdc);
	_endthread();
	
}


void __cdecl ThreadProcThree(void* param) {

	HDC hdc;
	HBRUSH hBrush;
	RECT rc;

	hdc = GetDC((HWND)param);
	while (gbDone == FALSE) {

		rc.left = gRect.right / 2 + (rand() % gRect.right / 2);
		rc.top = gRect.bottom / 2 + rand() % gRect.bottom / 2;
		rc.right = gRect.right / 2 + (rand() % gRect.right / 20);
		rc.bottom = gRect.bottom / 2 + rand() % gRect.bottom / 2;

		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
		DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));

	}

	ReleaseDC((HWND)param, hdc);
	_endthread();

}


void __cdecl ThreadProcFour(void* param) {

	HDC hdc;
	HBRUSH hBrush;
	RECT rc;

	hdc = GetDC((HWND)param);
	while (gbDone == FALSE) {

		rc.left = gRect.left + (rand() % gRect.right / 2);
		rc.top = gRect.bottom / 2 + rand() % gRect.bottom / 2;
		rc.right = gRect.left + (rand() % gRect.right / 20);
		rc.bottom = gRect.bottom / 2 + rand() % gRect.bottom / 2;

		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		SelectObject(hdc, hBrush);
		RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, rand() % 20, rand() % 20);
		DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));

	}

	ReleaseDC((HWND)param, hdc);
	_endthread();

}