/*
* ofCvContourFinder.h
*
* Finds white blobs in binary images and identifies 
* centroid, bounding box, area, length and polygonal contour
* The result is placed in a vector of ofCvBlob objects. 
*
*/

#ifndef OF_CV_CONTOUR_FINDER
#define OF_CV_CONTOUR_FINDER


#include "ofMain.h"
#include "ofCvMain.h"
#include "ofxOpenCv.h"



class ofCvContourFinder {

  public:
		
    vector<ofCvBlob> blobs;
    
    
    ofCvContourFinder();
    ~ofCvContourFinder();
    void findContours( ofxCvGrayscaleImage& input,
                       int minArea, int maxArea,
                       int nConsidered, bool bFindHoles );
    void draw( float x, float y );
    
    
  protected:
  
    ofxCvGrayscaleImage  inputCopy;
    CvMemStorage*  contour_storage;
    CvMemStorage*  storage;
    CvMoments*  myMoments;

    int nCvSeqsFound;
    CvSeq*  cvSeqBlobs[MAX_NUM_CONTOURS_TO_FIND];
  
  
    void reset();  
    
};



#endif
