#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

#include "GetPointsFromFile.h"

// Globals
char buff[10];
FILE* fptr = NULL;
long ContourCount = 0;
long PointsCount = 0;
// To store that, how many points will be there in contours
int* PointsArray = NULL;
// Actual array of points where *DrawPoints is Contour index and **DrawPoints is array of actual points in that Contour
int** DrawPoints = NULL;

POINT **Points = NULL;

// Entry Point
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {

	// Code
	switch (dwReason) {

	case DLL_PROCESS_ATTACH:

		break;

	case DLL_THREAD_ATTACH:

		break;

	case DLL_THREAD_DETACH:

		break;

	case DLL_PROCESS_DETACH:

		break;
	}

	return TRUE;
}

// Function to export
extern "C" int GetPoints(const char* fileName, POINT*** ptsArr) {


	if ((fptr = fopen(fileName, "r")) == NULL)
		return -1;

	if (fgets(buff, 20, fptr) == NULL)
		return -2;

	fseek(fptr, 0, SEEK_SET);
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
		}else
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

	// Allocating POINT array
	Points = (POINT**)malloc(ContourCount * sizeof(POINT*));
	for (int i = 0; i < ContourCount; i++) 
		Points[i] = (POINT*)malloc(PointsArray[i] * sizeof(POINT));
	
	// Filling POINT array
	for (int i = 0; i < ContourCount; i++) {
		printf("\n\nContour no. : %d\n\n", i);
		for (int j = 0; j < PointsArray[i] - 1; j += 2) {
			printf("X : %4d | Y : %4d\n", DrawPoints[i][j], DrawPoints[i][j + 1]);
			Points[i][j].x = DrawPoints[i][j];
			Points[i][j].y = DrawPoints[i][j + 1];

		}
	}

	// Cleaning and freeing old arrays ...
	fclose(fptr);
	free(PointsArray);
	for (int i = 0; i < ContourCount; i++) {
		free(DrawPoints[i]);
	}
	free(DrawPoints);

	fptr = NULL;
	DrawPoints = NULL;
	PointsArray = NULL;

	(*ptsArr) = Points;
	return 1;

}


// Function to export
extern "C" int FreePointsArray( POINT*** ptsArr) {

	for (int i = 0; i < ContourCount; i++) {
		free(Points[i]);
	}
	free(Points);
	Points = NULL;

	free(*ptsArr);
	return 1;
}