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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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


	static HBRUSH hBrush[3];
	HDC hdc;
	static RECT rc;
	PAINTSTRUCT ps;
	static int sentenceTracker = 0;
	static int xCalc, yCalc;
	HFONT hFont;
	SIZE size;

	TCHAR str[255];

	static TCHAR* pledge[] = {
	
		TEXT("भारत माझा देश आहे."),
		TEXT("सारे भारतीय माझे बांधव आहेत."),
		TEXT("माझ्या देशावर माझे प्रेम आहे."),
		TEXT("माझ्या देशातल्या समृद्ध आणि"),
		
		TEXT("विविधतेने नटलेल्या परंपरांचा मला अभिमान आहे."),
		TEXT("त्या परंपरांचा पाईक होण्याची पात्रता"),
		TEXT("माझ्या अंगी यावी म्हणून मी सदैव प्रयत्न करेन.मी माझ्या पालकांचा, गुरुजनांचा"),
		TEXT("आणि वडीलधाऱ्या माणसांचा मान ठेवीन"),
		
		TEXT("आणि प्रत्येकाशी सौजन्याने वागेन, माझा देश आणि माझे देशबांधव"),
		TEXT("यांच्याशी निष्ठा राखण्याची, मी प्रतिज्ञा करीत आहे."),
		TEXT("त्यांचे कल्याण आणि, त्यांची समृद्धी ह्यांतच माझे"),
		TEXT("सौख्य सामावले आहे. जय हिंद"),
		
	};

	switch (iMsg) {

	case WM_CREATE:
		GetClientRect(hwnd, &rc);
		SetTimer(hwnd, MYTIMER, 2600, NULL);
		yCalc = (rc.bottom - 410) / 2;
		PlaySound(TEXT("pledge.wav"),NULL, SND_ASYNC);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		hFont = CreateFont(35, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
		SelectObject(hdc, hFont);
		SetBkColor(hdc, RGB(0,0,0));


		if (sentenceTracker <= 3) 
			SetTextColor(hdc, RGB(255, 128, 0));
		else if (sentenceTracker > 3 && sentenceTracker <= 7) 
			SetTextColor(hdc, RGB(255, 255, 255));
		else
			SetTextColor(hdc, RGB(0, 255, 0));


		if (sentenceTracker == 4)
			yCalc += 25;

		if (sentenceTracker == 8)
			yCalc += 25;


		wsprintf(str, TEXT("%s"), pledge[sentenceTracker]);
		GetTextExtentPoint(hdc, str, wcslen(pledge[sentenceTracker]), &size);
		xCalc = (rc.right - size.cx) / 2;
		TextOut(hdc, xCalc, yCalc, str, wcslen(pledge[sentenceTracker++]));
		

		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		KillTimer(hwnd, MYTIMER);

		if (sentenceTracker > 11)
			KillTimer(hwnd, MYTIMER);
		else {
			yCalc += 30;
			InvalidateRect(hwnd, NULL, false);
			SetTimer(hwnd, MYTIMER, 2600, NULL);
		}
		break;

	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;

	
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
