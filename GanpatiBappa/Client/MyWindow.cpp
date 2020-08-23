#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define MYTIMER 200

// window procedure callback
LRESULT CALLBACK MyWndProcedure(HWND, UINT, WPARAM, LPARAM);
void Draw(HWND);

long ContourCount = 0;
long PointsCount = 0;

// To store that, how many points will be there in contours
int* PointsArray = NULL;
// Actual array of points where *DrawPoints is Contour index and **DrawPoints is array of actual points in that Contour
int** DrawPoints = NULL;

char buff[10];
FILE* fptr = NULL;




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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
		TEXT("Ganpati Bappa Morya"),
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
	
	static int i = 0;
	int j;
	int ret;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:

		if ((fptr = fopen("points.txt", "r")) == NULL) {
			MessageBox(hwnd, TEXT("Error opening file"), TEXT("Bye !!!"), MB_OK);
			exit(0);
		}


		// To get the number of contours
		while (fgets(buff, 20, fptr) != NULL) {
			if (strcmp(buff, "****\n") == 0) {
				ContourCount++;
			}
		}

		// Allocation of array where the points are to be stored ...
		PointsArray = (int*)malloc(ContourCount * sizeof(int));
		// Allocation of array where the actual points will be stored ... at this stage in code
		// we just know the count of contours...
		DrawPoints = (int**)malloc(ContourCount * sizeof(int*));

		ContourCount = 0;
		PointsCount = 0;
		fseek(fptr, 0, SEEK_SET);

		// Here we will find the count of total points in each Contours,
		// based on this count we will be able to allocate the memory for actual points to be stored ... afterwards
		while (fgets(buff, 20, fptr) != NULL) {
			if (strcmp(buff, "****\n") == 0) {
				//printf("Contour no. %d   |   POINTS size. : %d\n", ContourCount, PointsCount);
				PointsArray[ContourCount] = PointsCount;
				PointsCount = 0;
				ContourCount++;
			}
			else
				PointsCount++;

		}

		// we have points count in each contour in the form of array 
		// so we will iterate over array and actually allocate the memory where points are to be stored ...
		for (int i = 0; i < ContourCount; i++) {
			DrawPoints[i] = (int*)malloc(PointsArray[i] * sizeof(int));
		}

		ContourCount = 0;
		PointsCount = 0;
		fseek(fptr, 0, SEEK_SET);

		// Now we will fill points in **DrawContours 
		while (fgets(buff, 20, fptr) != NULL) {

			if (strcmp(buff, "****\n") == 0) {
				PointsCount = 0;
				ContourCount++;
			}
			else {
				DrawPoints[ContourCount][PointsCount] = atoi(buff);
				PointsCount++;
			}


		}
		
		SetTimer(hwnd, MYTIMER, 300, NULL);

		break;
	case WM_PAINT:
		break;

	case WM_TIMER:

		HDC hdc;
		HBRUSH hBrush;
		HPEN hPen;

		hdc = GetDC(hwnd);
		hBrush = CreateSolidBrush(RGB(255, 128, 0));
		SelectObject(hdc, hBrush);
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
		SelectObject(hdc, hPen);
		KillTimer(hwnd, MYTIMER);

		if (i < ContourCount) {
			for (int j = 0; j < PointsArray[i] - 1; j += 2) {
				MoveToEx(hdc, DrawPoints[i][j], DrawPoints[i][j + 1], NULL);
				LineTo(hdc, DrawPoints[i][j], DrawPoints[i][j + 1]);
			}
			i++;
		}


		ReleaseDC(hwnd, hdc);
		SetTimer(hwnd, MYTIMER, 150, NULL);

		break;

	case WM_DESTROY:
		MessageBox(hwnd, TEXT("Bye I'm saaad you are leaving mee!"), TEXT("Bye !!!"), MB_OK);
		fclose(fptr);
		free(PointsArray);
		for (int i = 0; i < ContourCount; i++) {
			free(DrawPoints[i]);
		}
		free(DrawPoints);

		fptr = NULL;
		DrawPoints = NULL;
		PointsArray = NULL;
		PostQuitMessage(0);

	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}


