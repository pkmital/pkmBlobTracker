#ifndef OF_CV_GRAYSCALE_IMAGE_H
#define OF_CV_GRAYSCALE_IMAGE_H

#include "ofCvImage.h"


class ofCvGrayscaleImage : public ofCvImage {


  public:

    ofCvGrayscaleImage() {};
    ofCvGrayscaleImage( const ofCvGrayscaleImage& mom );
    void allocate( int w, int h );


    // Set Pixel Data - Arrays
    //
    //    
    void setFromPixels( unsigned char* _pixels, int w, int h );
    void operator = ( unsigned char* _pixels );
    void operator = ( ofCvGrayscaleImage& mom );
    void operator = ( ofCvColorImage& mom );
    void operator = ( ofCvFloatImage& mom );     
    void operator -= ( ofCvGrayscaleImage& mom );
    void operator += ( ofCvGrayscaleImage& mom );
    void operator *= ( ofCvGrayscaleImage& mom );    
    void operator &= ( ofCvGrayscaleImage& mom );
    void absDiff( ofCvGrayscaleImage& mom );
    void absDiff( ofCvGrayscaleImage& mom, ofCvGrayscaleImage& dad );
    

    // Get Pixel Data
    //
    //
    unsigned char*  getPixels();
    
        
    // Draw Image
    //
    //
    void draw( float x, float y );
    void draw( float x, float y, float w, float h );
    void draw( ofCvBlob blob, int color );
    
    
    // Image Filter Operations
    //
    //
    void contrastStretch();
    void threshold( int value );


    // Image Transformation Operations
    //
    //
    void resize( int w, int h );
           
};



#endif
