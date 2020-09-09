#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<process.h>
#include<math.h>


#define MYTIMER 200
#define MYTIMER2 200

// window procedure callback
LRESULT CALLBACK MyWndProcedure(HWND, UINT, WPARAM, LPARAM);
void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
void Draw(HWND);
void FreeResources();
void __cdecl ThreadProcOne(void*);
void __cdecl ThreadProcTwo(void*);

long ContourCount = 0;
long PointsCount = 0;

// To store that, how many points will be there in contours
int* PointsArray = NULL;
// Actual array of points where *DrawPoints is Contour index and **DrawPoints is array of actual points in that Contour
int** DrawPoints = NULL;

char buff[10];
FILE* fptr = NULL;

const TCHAR * fileName;
static int i = 1, j = 0;
HDC hdc;

TCHAR str[255];
RECT rc;

HBRUSH hBrush;
HPEN hPen;


// WinMain() -> __WinMainCRTStartup() -> main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// take file name as input from commandline ...
	fileName = lpszCmdLine;

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

	hwnd = CreateWindow(szAppName,
		TEXT("Ganpati Bappa Morya"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		xCalc,
		yCalc,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ((int)msg.wParam);

}

LRESULT CALLBACK MyWndProcedure(HWND hwnd, UINT iMsg, WPARAM wparam, LPARAM lparam)
{
	
	int ret;
	TCHAR str[255];
	static unsigned long hThreadOne = NULL;
	static unsigned long hThreadTwo = NULL;

	

	switch (iMsg)
	{
	case WM_CREATE:

		hThreadOne = _beginthread(ThreadProcOne, 0, (void*)hwnd);
		
		break;

	case WM_PAINT:

		GetClientRect(hwnd, &rc);

		hThreadTwo = _beginthread(ThreadProcTwo, 0, (void*)hwnd);

		break;

	case WM_TIMER:

		

		break;

	case WM_DESTROY:
		FreeResources();
		PostQuitMessage(0);

	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wparam, lparam));
}

void FreeResources() {

	fclose(fptr);
	free(PointsArray);
	for (int i = 0; i < ContourCount; i++) {
		free(DrawPoints[i]);
	}
	free(DrawPoints);

	fptr = NULL;
	DrawPoints = NULL;
	PointsArray = NULL;
}

void __cdecl ThreadProcOne(void *param) {

	if ((fptr = fopen(fileName, "r")) == NULL) {
		MessageBox((HWND)param, TEXT("Error opening file"), TEXT("Message"), MB_OK | MB_ICONWARNING);
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
}

void __cdecl ThreadProcTwo(void* param) {


	POINT *pts = NULL;

	hdc = GetDC((HWND)param);
	SetViewportOrgEx(hdc, rc.right / 4 + 50, 50, NULL);
	hBrush = CreateSolidBrush(RGB(247, 221, 196));
	SelectObject(hdc, hBrush);
	hPen = CreatePen(PS_INSIDEFRAME, 2, RGB(173, 139, 245));
	SelectObject(hdc, hPen);
	
	

	for (i = ContourCount - 1; i > 0; i--) {

		wsprintf(str, TEXT("Drawing contour number, %d  "), i);
		DrawText(hdc, str, -1, &rc, DT_VCENTER);

		
		for (int j = 0; j < PointsArray[i] - 1; j += 2) {

			Sleep(1);
			Ellipse(hdc, DrawPoints[i][j] - 2, DrawPoints[i][j + 1] - 2, DrawPoints[i][j] + 2, DrawPoints[i][j + 1] + 2);
		}

		

		if (i == 1) {

			pts = (POINT*)malloc(PointsArray[i] * sizeof(POINT));

			for (int k = 0; k <= PointsArray[i] - 1; k++) {

				if (k % 2 == 0)
					pts[k].x = DrawPoints[i][k];
				else
					pts[k - 1].y = DrawPoints[i][k];
			}


			hBrush = CreateSolidBrush(RGB(255, 128, 0));
			SelectObject(hdc, hBrush);
			//MoveToEx(hdc, pts[0].x, pts[0].y,NULL);
			Polyline(hdc, pts, PointsArray[i]);

			FILE* fp = NULL;
			fp = fopen("logfile.txt", "w+");

			for (int k = 0; k < PointsArray[i]; k++) {
				fprintf(fp, "%d\n%d\n", pts[k].x, pts[k].y);
			}

			fclose(fp);
		}
		free(pts);
		pts = NULL;
		
	}

	ReleaseDC((HWND)param, hdc);
	_endthread();
}

