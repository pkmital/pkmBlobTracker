/*
 *  pkmBlobTracker.h
*/


#pragma once

#include "ofCvBlobTracker.h"
#include "ofMain.h"

class pkmBlobTracker
{
public:
	pkmBlobTracker()
	{
		center_x = center_y = width = height = 0;
		orientation = 0;
		initOrientation = 0;
		bInit = false;
		initializeLKFlow(320, 240);
	}
	
	~pkmBlobTracker()
	{
		cvReleaseImage(&prevGrayImage);
		cvReleaseImage(&grayImage);
		//cvReleaseImage(prevGrayImagePyr);
		//cvReleaseImage(grayImagePyr);
	}
	void initializeLKFlow(int width, int height)
	{
		prevGrayImage = cvCreateImage(cvSize(width, height), 8, 1);
		grayImage = cvCreateImage(cvSize(width, height), 8, 1);
		//prevGrayImagePyr = cvCreateImage(cvSize(width, height), 8, 1);
		//grayImagePyr = cvCreateImage(cvSize(width, height), 8, 1);
		
		gridSize = 4;													// Size of the grid for feature points A
		levels = 2;														// Number of levels in the pyramid
		winSize = cvSize(2,2);											// Size of the search window
		int type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
		eps = 0.01;
		iter = 10;
		crit = cvTermCriteria(type,iter,eps);
		internal_flags = 0;
	}
	void updateImage(const IplImage *img)
	{
		swap(prevGrayImage, grayImage);
		
		cvCopyImage(img, grayImage);
	}
	
	void reInit()
	{
		bInit = false;
	}
	
	float getOrientation(ofCvTrackedBlob &blob);
	void draw(int x, int y);
	
	IplImage				*prevGrayImage, *grayImage, 
							*prevGrayImagePyr, *grayImagePyr;
	CvTermCriteria			crit;
	int						levels;
	int						gridSize;
	int						type;
	double					eps;
	int						iter;
	int						internal_flags;
	CvSize					winSize;
	
	float					orientation, initOrientation;
	float					center_x, center_y, width, height;
	bool					bInit;
};