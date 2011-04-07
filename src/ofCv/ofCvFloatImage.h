
#ifndef OF_CV_FLOAT_IMAGE_H
#define OF_CV_FLOAT_IMAGE_H


#include "ofCvImage.h"


class ofCvFloatImage : public ofCvImage {


  public:

    ofCvFloatImage() {};
    ofCvFloatImage( const ofCvFloatImage& mom );
    void allocate( int w, int h );


    // Set Pixel Data - Arrays
    //
    //    
    void operator = ( ofCvGrayscaleImage& mom );
    void operator = ( ofCvColorImage& mom );
    void operator = ( ofCvFloatImage& mom );     
    void operator -= ( ofCvFloatImage& mom );
    void operator += ( ofCvFloatImage& mom );
    void operator *= ( ofCvFloatImage& mom );
    void addWeighted( ofCvGrayscaleImage& mom, float f );    
    
    
    // Draw Image
    //
    //
    void draw( float x, float y );
    void draw( float x, float y, float w, float h );
    void draw( ofCvBlob blob, int color );


    // Image Transformation Operations
    //
    //
    void resize( int w, int h );
            
};



#endif


