

#include "ofCvGrayscaleImage.h"
#include "ofCvColorImage.h"
#include "ofCvFloatImage.h"




ofCvColorImage::ofCvColorImage( const ofCvColorImage& mom ) {
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

void ofCvColorImage::allocate( int w, int h ) {
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
	pixels = new unsigned char[w*h*3];
	width = w;
	height = h;
    
    if( bUseTexture ) {
        tex.allocate( width, height, GL_RGB );
    }
}





// Set Pixel Data - Arrays
//
//
void ofCvColorImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*width*3), width*3 );
	}
}

void ofCvColorImage::operator =	( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

void ofCvColorImage::operator =	( ofCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_GRAY2RGB );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

void ofCvColorImage::operator =	( ofCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCopy( mom.getCvImage(), cvImage, 0 );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

void ofCvColorImage::operator =	( ofCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		//cvCopy(mom.getCvImage(), cvImage, 0);
		//cvConvertScale( mom.getCvImage(), cvImage, 1, 0 );
		cvConvert( mom.getCvImage(), cvImage ); // same as above but optimized
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}


void ofCvColorImage::operator -= ( ofCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvSub( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in -=, images are different sizes" << endl;
	}
}

void ofCvColorImage::operator += ( ofCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in +=, images are different sizes" << endl;
	}
}

void ofCvColorImage::operator *= ( ofCvColorImage& mom ) {
    float scalef = 1.0f / 255.0f;
	if( mom.width == width && mom.height == height ) {
		cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
		swapTemp();
	} else {
        cout << "error in *=, images are different sizes" << endl;
	}
}





// Get Pixel Data
//
//
unsigned char* ofCvColorImage::getPixels() {
	// copy each line of pixels:
	for( int i=0; i<height; i++ ) {
		memcpy( pixels+(i*width*3), 
                cvImage->imageData+(i*cvImage->widthStep), width*3 );
	}
	return pixels;
}




// Draw Image
//
//
void ofCvColorImage::draw( float x, float y ) {

	// note, this is a bit ineficient, as we have to
	// copy the data out of the cvImage into the pixel array
	// and then upload to texture.  We should add
	// to the texture class an override for pixelstorei
	// that allows stepped-width image upload:

    if( bUseTexture ) {
        tex.loadData( getPixels(), width, height, GL_RGB );
        tex.draw( x, y, width, height );
 
    } else {
        IplImage* o;
        o = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 3 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+height, 0.0 );
        glDrawPixels( o->width, o->height , 
                     GL_BGR, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+height), 0.0 );     
    }
}


void ofCvColorImage::draw( float x, float y, float w, float h ) { 
    if( bUseTexture ) {
        tex.loadData( getPixels(), width, height, GL_RGB ); 
        tex.draw( x, y, w, h );
        
    } else {
        IplImage* o;
        o = cvCreateImage( cvSize(w, h), IPL_DEPTH_8U, 3 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+h, 0.0 );
        glDrawPixels( o->width, o->height , 
                     GL_BGR, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+h), 0.0 );     
    }
} 





// Image Filter Operations
//
//




// Image Transformation Operations
//
//
void ofCvColorImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by 
    // reusing the temporal image storage
    
    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp ); 
}

