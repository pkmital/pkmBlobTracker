#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOsc.h"

#include "ofxOpenCv.h"

//#define _USE_LIVE_VIDEO		

const int MAX_BLOBS = 1;
const int NUM_CAMERAS = 1;
const int W = 320;
const int H = 240;
const int WINDOW_WIDTH = W*3 + 40*2;
const int WINDOW_HEIGHT = H*1.5;

#include "ofxControlPanel.h"
#include "ofVideoGrabber.h"

#include "CvPixelBackgroundGMM.h"
#include "ofCvBlobTracker.h"
#include "pkmBlobTracker.h"

class testApp : public ofBaseApp, public ofCvBlobListener{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
#ifdef _USE_LIVE_VIDEO
		ofVideoGrabber			vidGrabber[NUM_CAMERAS];
#else
		ofVideoPlayer			vidPlayer;
#endif	
		ofxCvColorImage			colorImg[NUM_CAMERAS];
        ofxCvGrayscaleImage 	grayImage[NUM_CAMERAS];
		ofxCvGrayscaleImage 	warpedImage[NUM_CAMERAS];

	
		ofxCvGrayscaleImage		grayBg;
		ofxCvGrayscaleImage		grayDiff;
		CvPixelBackgroundGMM	*pGMM;
		int						threshold;
		int						low_threshold, high_threshold, block_size;
		double					pGMMTiming;
		unsigned long			frame_num;
		bool					option1;
	
		ofCvContourFinder		contourFinder;		
		ofCvBlobTracker			blobTracker;	
		pkmBlobTracker			orientationTracker;
		float					blob_x, blob_y;
		
		bool					bLearnBackground;
		unsigned char			*inImage;
		unsigned char			*outImage;
	
		// blob callbacks 
		
		void blobOn( int x, int y, int id, int order );
		void blobMoved( int x, int y, int id, int order );    
		void blobOff( int x, int y, int id, int order );   
		bool bBlob;
	
		float					orientation;
	
		ofxOscSender			sender;
		
		ofxControlPanel			gui;

};

#endif
