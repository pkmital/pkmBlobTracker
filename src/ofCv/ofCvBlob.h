/*
* ofCvBlob.h
* openFrameworks
*
* A blob is a homogenous patch represented by a polygonal contour. 
* Typically a blob tracker uses the contour to figure out the blob's
* persistence and "upgrades" it with ids and other temporal 
* information. 
*
*/


#ifndef OF_CV_BLOB_H
#define OF_CV_BLOB_H

#include <vector>

class ofCvBlob {
  public:

    float area;
    float length;
    ofRectangle box;
    ofPoint center;
    
    bool hole;
    vector<ofPoint> contour;
   

    ofCvBlob() {
        area = 0.0f;
        length = 0.0f;
        hole = false;        
    }
    
};


#endif


