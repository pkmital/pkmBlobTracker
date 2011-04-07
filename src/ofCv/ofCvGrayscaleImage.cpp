
#include "ofCvGrayscaleImage.h"
#include "ofCvColorImage.h"
#include "ofCvFloatImage.h"



ofCvGrayscaleImage::ofCvGrayscaleImage( const ofCvGrayscaleImage& mom ) {
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

void ofCvGrayscaleImage::allocate( int w, int h ) {
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
	pixels = new unsigned char[w*h];
	width = w;
	height = h;
    
    if( bUseTexture ) {
        tex.allocate( width, height, GL_LUMINANCE );    
    }
}




// Set Pixel Data - Arrays
//
//
void ofCvGrayscaleImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*w), w );
	}
}

void ofCvGrayscaleImage::operator =	( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}


void ofCvGrayscaleImage::operator =	( ofCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        cvCopy( mom.getCvImage(), cvImage, 0 );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

void ofCvGrayscaleImage::operator =	( ofCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

void ofCvGrayscaleImage::operator =	( ofCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvConvert( mom.getCvImage(), cvImage );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}


void ofCvGrayscaleImage::operator -= ( ofCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvSub( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in -=, images are different sizes" << endl;
	}
}

void ofCvGrayscaleImage::operator += ( ofCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in +=, images are different sizes" << endl;
	}
}

void ofCvGrayscaleImage::operator *= ( ofCvGrayscaleImage& mom ) {
    float scalef = 1.0f / 255.0f;
	if( mom.width == width && mom.height == height ) {
		cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
		swapTemp();
	} else {
        cout << "error in *=, images are different sizes" << endl;
	}
}

void ofCvGrayscaleImage::operator &= ( ofCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in &=, images are different sizes" << endl;
	}
}

void ofCvGrayscaleImage::absDiff( ofCvGrayscaleImage& mom ) {
    if( mom.width == width && mom.height == height ) {
        cvAbsDiff( cvImage, mom.getCvImage(), cvImageTemp );
        swapTemp();
    } else {
        cout << "error in absDiff, images are different sizes" << endl;
    }
}

void ofCvGrayscaleImage::absDiff( ofCvGrayscaleImage& mom, 
                                  ofCvGrayscaleImage& dad ) {
    if( mom.width == dad.width == width && mom.height == dad.height == height ) {
        cvAbsDiff( mom.getCvImage(), dad.getCvImage(), cvImage );
    } else {
        cout << "error in absDiff, images are different sizes" << endl;
    }

}






// Get Pixel Data
//
//
unsigned char* ofCvGrayscaleImage::getPixels() {
	for( int i = 0; i < height; i++ ) {
		memcpy( pixels+(i*width), 
                cvImage->imageData+(i*cvImage->widthStep), width );
	}
	return pixels;
}





// Draw Image
//
//
void ofCvGrayscaleImage::draw( float x, float y ) {
    if( bUseTexture ) {
        // note, this is a bit ineficient, as we have to
        // copy the data out of the cvImage into the pixel array
        // and then upload to texture.  We should add
        // to the texture class an override for pixelstorei
        // that allows stepped-width image upload:    
        tex.loadData(getPixels(), width, height, GL_LUMINANCE);
        tex.draw(x,y,width, height);
        
    } else {
        IplImage* o;
        o = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+height, 0.0 );
        glDrawPixels( o->width, o->height , 
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+height), 0.0 );  
    }
}


void ofCvGrayscaleImage::draw( float x, float y, float w, float h ) { 
    if( bUseTexture ) {
        tex.loadData(getPixels(), width, height, GL_LUMINANCE); 
        tex.draw(x,y, w,h);
        
    } else {
        IplImage* o;
        o = cvCreateImage( cvSize(w, h), IPL_DEPTH_8U, 1 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+h, 0.0 );
        glDrawPixels( o->width, o->height , 
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+h), 0.0 );
    }
} 



void  ofCvGrayscaleImage::draw( ofCvBlob blob, int color ) { 
       if( blob.contour.size() > 0 ) { 
           CvPoint* pts = new CvPoint[blob.contour.size()]; 
           for( int i=0; i<blob.contour.size() ; i++ ) { 
               pts[i].x = blob.contour[i].x; 
               pts[i].y = blob.contour[i].y; 
           }
           int nPts = blob.contour.size();
           cvFillPoly( cvImage, &pts, &nPts, 1, 
                       CV_RGB(color,color,color) ); 
           delete pts;
       } 
} 






// Image Filter Operations
//
//
void ofCvGrayscaleImage::contrastStretch() {
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
	double scale=1.0f;
	double shift=0;
	if( (maxVal-minVal) != 0 ) {
		scale=255.0/(maxVal-minVal);
    	shift=-minVal*scale;
	}
	cvConvertScale( cvImage, cvImageTemp, scale, shift );
	swapTemp();
}


void ofCvGrayscaleImage::threshold( int value ) {
	//http://lush.sourceforge.net/lush-manual/01a8321b.html
	cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY );
	swapTemp();
}




// Image Transformation Operations
//
//
void ofCvGrayscaleImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by 
    // reusing the temporal image storage
    
    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );  
}



