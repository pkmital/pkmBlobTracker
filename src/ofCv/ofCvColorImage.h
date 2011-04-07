
#ifndef OF_CV_COLOR_IMAGE_H
#define OF_CV_COLOR_IMAGE_H

#include "ofCvImage.h"


class ofCvColorImage : public ofCvImage {


  public:

    ofCvColorImage() {};
    ofCvColorImage( const ofCvColorImage& mom );
    void allocate( int w, int h );


    // Set Pixel Data - Arrays
    //
    //    
    void setFromPixels( unsigned char * _pixels, int w, int h );
    void operator = ( unsigned char* _pixels );    
    void operator = ( ofCvGrayscaleImage& mom );
    void operator = ( ofCvColorImage& mom );
    void operator = ( ofCvFloatImage& mom );    
    void operator -= ( ofCvColorImage& mom );
    void operator += ( ofCvColorImage& mom );
    void operator *= ( ofCvColorImage& mom );    
    

    // Get Pixel Data
    //
    //
    unsigned char*  getPixels();
    
    
    // Draw Image
    //
    //
    void draw( float x, float y );
    void draw( float x, float y, float w, float h );
    
    
    // Image Filter Operations
    //
    //


    // Image Transformation Operations
    //
    //
    void resize( int w, int h );
};



#endif
