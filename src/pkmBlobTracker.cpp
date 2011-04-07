/*
 *  pkmBlobTracker.cpp
 *  opencvExample
 *
 *  Created by Mr. Magoo on 4/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "pkmBlobTracker.h"

float pkmBlobTracker::getOrientation(ofCvTrackedBlob &blob)
{
	int numPts = blob.contour.size();
	char status[numPts];
	float err[numPts];
	CvPoint2D32f featuresA[numPts];
	CvPoint2D32f featuresB[numPts];
	
	for(int i = 0; i < numPts; i++)
	{
		featuresA[i].x = MAX(MIN(blob.contour[i].x, 317),3);
		featuresA[i].y = MAX(MIN(blob.contour[i].y, 237),3);
	}
	
	cvCalcOpticalFlowPyrLK(prevGrayImage,grayImage,
						   NULL, NULL,
						   &(featuresA[0]),&(featuresB[0]),
						   numPts,cvSize(12,12),levels,status,err,crit,internal_flags);
	
	float ori = 0.0f;
	int pts = 0;
	for (int i = 0; i < numPts; i++) {

		CvPoint2D32f vecA,vecB,vecAB;
		vecA.x = featuresA[i].x;
		vecA.y = featuresA[i].y;
		
		vecB.x = featuresB[i].x;
		vecB.y = featuresB[i].y;
		
		//atan2f((float)vecA.x, (float)vecA.y) - atan2f((float)vecB.x, (float)vecB.y);
		//float normA = sqrtf(vecA.x*vecA.x + vecA.y*vecA.y);
		//float normB = sqrtf(vecB.x*vecB.x + vecB.y*vecB.y);
		//acosf( (vecA.x*vecB.x + vecA.y*vecB.y) / (normA * normB) );
		float o =  fmod(atan2f((float)(vecB.y - vecA.y), (float)(vecB.x - vecA.x)), (float)(2.0f*PI));
				
		if(!isnan(o))
		{
			printf("%f\n", (float)(o*180.0f/PI));
			ori += o;
			pts++;
		}
		
	}
	ori /= pts;
	if (bInit) {
		orientation = fmod((float)(0.6*orientation + 0.4*(orientation + (ori-orientation))), (float)(2.0f*PI));
	}
	else {
		orientation = ori;
		bInit = true;
	}
	center_x = blob.center.x;
	center_y = blob.center.y;
	width = blob.box.width;
	height = blob.box.height;
	
	return orientation;
}

void pkmBlobTracker::draw(int x, int y)
{
	ofSetColor(200, 20, 20);
	ofNoFill();
	ofPushMatrix();
	ofCircle(x + center_x, y + center_y, height/2.0f);
	ofTranslate(x+center_x, y+center_y, 0);
	ofRotate(orientation * 180.0f / PI, 0, 0, 1);
	ofLine(0, 0, height/2.0f, 0);
	ofPopMatrix();
}