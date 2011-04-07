
#include "ofCvGrayscaleImage.h"
#include "ofCvColorImage.h"
#include "ofCvFloatImage.h"




ofCvFloatImage::ofCvFloatImage( const ofCvFloatImage& mom ) {
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

void ofCvFloatImage::allocate( int w, int h ) {
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 1 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 1 );
	pixels = new unsigned char[w*h];
	width = w;
	height = h;
    
    if( bUseTexture ) {
        tex.allocate(width, height, GL_LUMINANCE);
    }
}




// Set Pixel Data - Arrays
//
//
void ofCvFloatImage::operator =	( ofCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        cvConvert( mom.getCvImage(), cvImage );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

void ofCvFloatImage::operator =	( ofCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

void ofCvFloatImage::operator =	( ofCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCopy( mom.getCvImage(), cvImage, 0 );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}


void ofCvFloatImage::operator -= ( ofCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvSub( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in -=, images are different sizes" << endl;
	}
}

void ofCvFloatImage::operator += ( ofCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in +=, images are different sizes" << endl;
	}
}

void ofCvFloatImage::operator *= ( ofCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvMul( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in *=, images are different sizes" << endl;
	}
}


void ofCvFloatImage::addWeighted( ofCvGrayscaleImage& mom, float f ) {
	if( mom.width == width && mom.height == height ) {
         IplImage* cvTemp = cvCreateImage( cvSize(width,height), IPL_DEPTH_32F, 1 );
         cvConvertScale( mom.getCvImage(), cvTemp, 1, 0 );
         cvAddWeighted( cvTemp, f, cvImage, 1.0f-f,0, cvImageTemp );
         swapTemp();
         cvReleaseImage( &cvTemp );
    } else {
        cout << "error in addWeighted, images are different sizes" << endl;
    }
}







// Draw Image
//
//
void ofCvFloatImage::draw( float x, float y ) {

    // note, this is not using a texture nor is this efficient
    // rewriting this would be good but not totally trivial
    // main issues are probably:
    // textures cannot read from float image data
    // textures cannot read width aligned pixel data
    
    IplImage* o;
    o = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
    cvConvert( cvImage, o );
    cvFlip( o, o, 0 );
    glRasterPos3f( x, y+height, 0.0 );
    glDrawPixels( o->width, o->height , 
                  GL_LUMINANCE, GL_UNSIGNED_BYTE, o->imageData );
    cvReleaseImage( &o );
    glRasterPos3f( -x, -(y+height), 0.0 );      
}



void ofCvFloatImage::draw( float x, float y, float w, float h ) {

    // note, this is not using a texture nor is this efficient
    // rewriting this would be good but not totally trivial
    // main issues are probably:
    // textures cannot read from float image data
    // textures cannot read width aligned pixel data
    
    IplImage*  o8;
    IplImage*  o;
    o8 = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
    o = cvCreateImage( cvSize(w, h), IPL_DEPTH_8U, 1 );
    cvConvert( cvImage, o8 );
    cvResize( o8, o, CV_INTER_NN );    
    cvFlip( o, o, 0 );
    glRasterPos3f( x, y+height, 0.0 );
    glDrawPixels( o->width, o->height , 
                  GL_LUMINANCE, GL_UNSIGNED_BYTE, o->imageData );
    cvReleaseImage( &o8 );
    cvReleaseImage( &o );
    glRasterPos3f( -x, -(y+height), 0.0 );
}





// Image Transformation Operations
//
//
void ofCvFloatImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by 
    // reusing the temporal image storage
    
    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 1 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );  
}

