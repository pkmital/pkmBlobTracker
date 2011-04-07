#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	//for this video these radial settings do a nice un fish eye angle effect
	float radialX = -0.176;
	float radialY =  0.024;
	
#ifdef _USE_LIVE_VIDEO
	vidGrabber[0].setVerbose(true);
	vidGrabber[0].initGrabber(W,H);
	//vidGrabber[1].setVerbose(true);
	//vidGrabber[1].initGrabber(W,H);
#else
	vidPlayer.loadMovie(ofToDataPath("test1.mov"));
	vidPlayer.setLoopState(OF_LOOP_NORMAL);
	vidPlayer.play();
#endif
	/*
	gui.setup("warp", 0, 0, 1024, 768);
	gui.addPanel("warper", 3);
	gui.setWhichPanel(0);
	
	gui.addSlider("radial x/unwarp ", "RAD_X", radialX, -0.2, 0.2, false);
	gui.addSlider("radial y", "RAD_Y", radialY, -0.2, 0.2, false);

	gui.addSlider("tang x", "TAN_X", 0.0, -0.3, 0.3, false);
	gui.addSlider("tang y", "TAN_Y", 0.0, -0.3, 0.3, false);
	
	gui.addSlider("focal x", "FOC_X", W/2,  -W, W, false);
	gui.addSlider("focal y", "FOC_Y", H/2,  -H, H, false);

	gui.addSlider("center x", "CEN_X", W/2,  0, W, false);
	gui.addSlider("center y", "CEN_Y", H/2,  0, H, false);	
	
	gui.setWhichColumn(1);
	gui.addDrawableRect("original", &grayImage[0], 320, 240);
	gui.addDrawableRect("un-warped", &warpedImage[0], 320, 240);
	gui.setWhichColumn(2);
	gui.addDrawableRect("original", &grayImage[1], 320, 240);
	gui.addDrawableRect("un-warped", &warpedImage[1], 320, 240);
	 */
		
    colorImg[0].allocate(W,H);
	grayImage[0].allocate(W,H);
	warpedImage[0].allocate(W,H);
	/*
    colorImg[1].allocate(W,H);
	grayImage[1].allocate(W,H);
	warpedImage[1].allocate(W,H);
	*/
	
	// Background model variables
	grayBg.allocate(W,H);
	grayDiff.allocate(W,H);
	
	pGMM=cvCreatePixelBackgroundGMM(W,H);
	pGMMTiming = 30.0;
	pGMM->fAlphaT = 1. / pGMMTiming;
	pGMM->fTb = 5*4;
	pGMM->fSigma = 30;
	pGMM->bShadowDetection = true;
	bLearnBackground = true;
	threshold = 20;
	block_size = 9;
	
	low_threshold = 16;	frame_num = 0;
	
	bLearnBackground = true;
	threshold = 15;
	option1 = true;	// learn a background once
	
	inImage = new unsigned char[W*H];
	outImage = new unsigned char[W*H];
	
    blobTracker.setListener( this );
	orientation = 0.0f;
	
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetBackgroundAuto(true);
	ofBackground(0,0,0);
	
	bBlob = false;
	
	sender.setup("10.0.2.6", 12345);
	
	ofResetElapsedTimeCounter();
	
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(0,0,0);
	
	gui.update();
#ifdef _USE_LIVE_VIDEO
	vidGrabber[0].grabFrame();
	if (vidGrabber[0].isFrameNew())
	{
		inImage = vidGrabber[0].getPixels();
#else
	vidPlayer.update();
	if (vidPlayer.isFrameNew()) 
	{
		inImage = vidPlayer.getPixels();
#endif
		colorImg[0].setFromPixels(inImage, W,H);
	    grayImage[0] = colorImg[0];
		orientationTracker.updateImage(grayImage[0].getCvImage());
		if (bLearnBackground){
			cvUpdatePixelBackgroundGMM(pGMM, inImage, outImage);
			grayDiff.setFromPixels(outImage, W, H);
		}
		else {
			cvPixelBackgroundGMMSubtraction(pGMM, inImage, outImage);
			grayDiff.setFromPixels(outImage, W, H);

		}

		grayBg = grayDiff;
		
		// subtraction and threshold
		grayDiff.blur( block_size );
		grayDiff.threshold( threshold );
		//cvAdaptiveThreshold(grayDiff.getCvImage(), grayDiff.getCvImage(), 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, block_size, threshold);
		//grayDiff.flagImageChanged();
		
		// blob tracking
		contourFinder.findContours(grayDiff, 150, W*H/3, MAX_BLOBS, false);
		blobTracker.trackBlobs( contourFinder.blobs );
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	gui.draw();
	
	ofSetColor( 0xffffff );
	
	colorImg[0].draw(20,20);
	
	grayDiff.draw(20+W+20+W+20,20);	
	
	// debug text output
	ofSetColor(0,255,131);
	char buf[256];
	if (bLearnBackground) {
		sprintf(buf, "learning background...\n['+'] or ['-'] to change threshold: %d\n", threshold);
	}
	else {
		sprintf(buf, "['space'] to learn background\n['+'] or ['-'] to change threshold: %d\n", threshold);
	}
	ofDrawBitmapString( buf, 20,290 );
	
	grayBg.draw(20+W+20, 20);
	ofSetColor(0,255,131);
	sprintf(buf, "['9'] or ['0'] to change gmm timing: %f\n['['] or [']'] to change block size: %d", pGMMTiming, block_size);
	ofDrawBitmapString( buf, 20,316 );
	
	
	// blob drawing
	blobTracker.draw(20+W+20+W+20, 20);
	
	if (bBlob) {
		orientationTracker.draw(20+W+20+W+20, 20);
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	switch (key){
#ifdef _USE_LIVE_VIDEO
		case 's':
			vidGrabber[0].videoSettings();
			break;
#endif
		case 'f':
			ofToggleFullscreen();
			break;
//		case 'd':
//			isdebug = !isdebug;
//			break;
//		case 'm':
//			drawmovie = !drawmovie;
//			break;
//		case 'r':
//			dorandom = !dorandom;
//			break;
			
			
		case ' ':
			bLearnBackground = !bLearnBackground;
			break;
		case '=': case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case '9':
			pGMMTiming -= 1.0;
			pGMMTiming = MAX(5,pGMMTiming);
			pGMM->fAlphaT = 1. / pGMMTiming;
			break;
		case '0':
			pGMMTiming += 1.0;
			pGMMTiming = MIN(320.0,pGMMTiming);
			pGMM->fAlphaT = 1. / pGMMTiming;
			break;
		case '[':
			block_size = MAX(5,block_size-=2);
			break;
		case ']':
			block_size = MIN(53,block_size+=2);
			break;
			
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------
void testApp::blobOn( int x, int y, int id, int order ) {

	printf("blobOn() - id:%d\n", id);

	bBlob = true; 
}

void testApp::blobMoved( int x, int y, int id, int order) {
	printf("blobMoved() - id:%d\n", id);
	
    // full access to blob object ( get a reference)
    ofCvTrackedBlob blob = blobTracker.getById( id );
	
	orientation = orientationTracker.getOrientation(blob) * 180.0f / PI;
	printf("ori: %f\n", orientation);
	ofxOscMessage m;
	m.setAddress("/listener/position");
	blob_x = (x / (float)W - 0.5) * 5.0f;
	blob_y = (y / (float)H - 0.5) * 5.0f;
	printf("%f, %f\n", blob_x, blob_y);
	m.addFloatArg(blob_x);
	m.addFloatArg(blob_y);
	m.addFloatArg(1.0f);
	sender.sendMessage(m);
	
	bBlob = true;
}

void testApp::blobOff( int x, int y, int id, int order ) {
		
	bBlob = false; 
	printf("blobOff() - id:%d\n", id);
}

