#include<windows.h>
#include<shlwapi.h>
#include<time.h>
#include<stdio.h>
#include "MyHeader.h"
#define MYCOLORTIMER 502
#define MYBARSTIMER 503
#define MYBUBBLESORT 504

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK BarTimerProc(HWND, UINT, UINT_PTR, DWORD);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK BubbleSortTimerProc(HWND, UINT, UINT_PTR, DWORD);

void allocateNumMem(int**, int);
void swap(int*, int*);
void randomize(int**, int);
void initBars(int** ,int);
void bubbleSort(int**, int);

int barsNum;
int* shuffledNums;
FILE* fp = NULL;
int r = 0, g = 128, b = 255;
RECT rcCopy;
RECT barPts;
HWND ghwnd;
HBRUSH hBrush;
TCHAR currentElemText[10];
HINSTANCE ghInstance;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	fp = fopen("LogFile.txt", "w+");
	
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SortVisulaApp");
	HMENU hMenu;

	sscanf(lpszCmdLine, "%d", &barsNum);
	fprintf(fp, "%d\n", barsNum);
	ghInstance = hInstance;
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MYMENU));

	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = szAppName;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(
		szAppName,
		TEXT("Sorting -Visualisation -App"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		hMenu,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, iCmdShow); //SW_MAXIMIZE
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rc;
	
	ghwnd = hwnd;
	
	

	switch (iMsg) {
		
	case WM_CREATE:
		if (fp == NULL)
			MessageBox(hwnd, TEXT("Pinter is Null to LogFile"), TEXT("Falied to open file"), MB_OK);
		else
			MessageBox(hwnd, TEXT("SUCCESS"), TEXT("SUCCESS"), MB_OK);

		fprintf(fp, "%s", "Omkar ajagunde is Started writing...\n");

		allocateNumMem(&shuffledNums, barsNum);
		for (int i = 0; i < barsNum; i++)
			fprintf(fp, "%d\n", shuffledNums[i]);

		fprintf(fp, "%s", "Omkar ajagunde is finished writing...\n");

		
		SetTimer(hwnd, MYCOLORTIMER, 10, NULL);
		SetTimer(hwnd, MYBARSTIMER, 10, BarTimerProc);
		

		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		rcCopy = rc;
		rcCopy.bottom -= 10;
		rcCopy.right -= 10;
		rcCopy.top += 10;
		rcCopy.left += 10;
		hdc = BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(RGB(r, g, b));
		

		SelectObject(hdc, hBrush);
		Rectangle(hdc, barPts.left, barPts.top, barPts.right, barPts.bottom);
		//FillRect(hdc, &barPts, hBrush);
		SetBkColor(hdc,RGB(r,g,b));
		DrawText(hdc,currentElemText,-1,&barPts,DT_TOP & DT_CENTER);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		KillTimer(hwnd, MYCOLORTIMER);
		if (r < 256) {
			r += 10;
			g = 128;
			b = 255;
		}
		else if (g < 256) {
			r = 255;
			g += 10;
			b = 128;
		}
		else if (b < 256) {
			r = 128;
			g = 255;
			b += 10;
		}

		
		
		if (b >= 256)	b = 0;
		else if (g >= 256)	g = 0;
		else if (r >= 256)	r = 0;

			
		
		SetTimer(hwnd, MYCOLORTIMER, 10, NULL);
		break;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case IDM_BUBBLESORT:
			MessageBox(hwnd, TEXT("Bubble Sort happening!"), TEXT("BubbleSort"), MB_OKCANCEL | MB_SERVICE_NOTIFICATION);
			InvalidateRect(hwnd, NULL, true);
			bubbleSort(&shuffledNums, barsNum);
			//SetTimer(hwnd, MYBUBBLESORT, 10, BubbleSortTimerProc);
			break;

		case IDM_SELECTIONSORT:
			MessageBox(hwnd, TEXT("Selection Sort happening!"), TEXT("SelectionSort"), MB_OKCANCEL | MB_SERVICE_NOTIFICATION);
			break;

		case IDM_APPABOUT:
			DialogBox(ghInstance, TEXT("about"), hwnd, AboutDlgProc);
			break;
		}

		break;
	

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye!! Its sad you are going !"), TEXT("Bye have a good day!"), MB_OKCANCEL | MB_SERVICE_NOTIFICATION);
		free(shuffledNums);
		fclose(fp);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void allocateNumMem(int** num, int totalNum) {

	// Create array of integers in heap memory
	*num = (int*)malloc(totalNum * sizeof(int));
	// Zero out the elements of array created in memory @ above step
	fprintf(fp, "%s", "malloc done...\n");
	for (int i = 0; i < totalNum; i++) {
		(*num)[i] = i;
	}
	fprintf(fp, "%s", "shuffling array...\n");
	randomize(&(*num), totalNum);
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void randomize(int **arr, int n) {
	srand(time(NULL));
	int i;
	for (i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(&(*arr)[i], &(*arr)[j]);
	}
	fprintf(fp, "%s", "in randomise...\n");
}

void CALLBACK BarTimerProc(HWND hwnd, UINT iMsg, UINT_PTR eventMsg, DWORD ms) {
	initBars(&shuffledNums, barsNum);
}

void CALLBACK BubbleSortTimerProc(HWND hwnd, UINT iMsg, UINT_PTR eventMsg, DWORD ms) {
	bubbleSort(&shuffledNums, barsNum);
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	switch (iMsg) {

	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:

		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, false);
			return true;
		}
		break;
	}
	return false;
}

void initBars(int** arr, int barsCount) {

	int* barsArr = *arr;
	int barHeight;
	int barWidth = rcCopy.right / barsCount;
	static int barX = 20;
	int barY;

	static int i = 0;


	//barPts.top = rcCopy.bottom - 5;


	if(i < barsCount) {

		KillTimer(ghwnd, MYBARSTIMER);

		int currentElem = barsArr[i];
		sprintf(currentElemText, "%d", currentElem);
		barPts.top = rcCopy.bottom - (barsArr[i] * rcCopy.bottom) / 100;
		barPts.bottom = rcCopy.bottom;
		barPts.left = barX;
		barPts.right = barX + barWidth + 2;
		fprintf(fp, "%ld %ld %ld %ld\n", barPts.left, barPts.top, barPts.right, barPts.bottom);
		barX = barX + barWidth;
		InvalidateRect(ghwnd, &barPts, true);
		SetTimer(ghwnd, MYBARSTIMER, 10, BarTimerProc);

		i++;
	}

}

void bubbleSort(int** arr, int n) {
	
	int* barsArr = *arr;
	/*
	int barHeight;
	int barWidth = rcCopy.right / n;
	static int barX = 20;
	int barY;

	static int i = 0,j = 0;
	for (i = 0; i < n - 1; i++) {

		//KillTimer(ghwnd, MYBUBBLESORT);

		// Last i elements are already in place    
		for (j = 0; j < n - i - 1; j++) {

			if (barsArr[j] > barsArr[j + 1]) {
				swap(&barsArr[j], &barsArr[j + 1]);
				fprintf(fp, "\t%d\n", barsArr[i]);

				int currentElem = barsArr[i];
				sprintf(currentElemText, "%d", currentElem);
				barPts.top = rcCopy.bottom - (barsArr[i] * rcCopy.bottom) / 100;
				barPts.bottom = rcCopy.bottom;
				barPts.left = barX;
				barPts.right = barX + barWidth + 2;
				//fprintf(fp, "%ld %ld %ld %ld\n", barPts.left, barPts.top, barPts.right, barPts.bottom);
				barX = barX + barWidth;
				InvalidateRect(ghwnd, &barPts, true);

			}
		}
		//SetTimer(ghwnd, MYBUBBLESORT, 10, BubbleSortTimerProc);
	}*/


}