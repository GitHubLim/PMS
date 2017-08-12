#include <iostream>
#include <opencv2/opencv.hpp>
#include "pms.h"

using namespace std;
using namespace cv;

//DO NOT REVISE THIS MAIN CODE 
int main(void) {

	VideoCapture video(FILENAME);

	//Error in opening the video input
	if (!video.isOpened()) {
		cerr << "Unable to open video file: " << FILENAME << endl;
		exit(EXIT_FAILURE);
	}

	//VIDEO 320 X 240 
	video.set(CAP_PROP_FRAME_WIDTH, 320);
	video.set(CAP_PROP_FRAME_HEIGHT, 240);

	Mat frame, background, mask;

	//Background extraction
	video >> background;
	mask = preMasking(background);
	background = preprocessing(background, mask);
	
	int speed = 0;

	//MAIN LOOP
	while (true) {
		video >> frame;

		//Speed of Video
		if (++speed % 10)
			continue;

		//End of video
		if (frame.empty()) {
			cout << "END OF VIDEO" << endl;
			break;
		}
	
		//------------- Warping ---------------
		Mat warpFrame, backWarpFrame;
		warpFrame = warpingFun(frame, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));
		backWarpFrame = warpingFun(background, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));

		//차영상(warping)
		Mat foreWarpFrame = diffFrame(backWarpFrame, warpFrame);

		//Labeling(warping)
		makeLabeling(warpFrame, foreWarpFrame);

		drawWarpParkingLotFun(warpFrame);

		//주차공간 확인
		decideParkingLot(warpFrame, foreWarpFrame);
	
		Mat warpParkingLot, backWarpParkingLot;
		warpParkingLot = warpingFun(warpFrame, Point(245, 0), Point(259, 62), Point(296, 62), Point(296, 0));
		
		//--------------------------------------

		//------------- Warping 2---------------
		Mat warpFrame2, backWarpFrame2;
		warpFrame2 = warpingFun(frame, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));
		backWarpFrame2 = warpingFun(background, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));

		//차영상(warping)
		Mat foreWarpFrame2 = diffFrame(backWarpFrame2, warpFrame2);

		//Labeling(warping)
		makeLabeling(warpFrame2, foreWarpFrame2);
		//--------------------------------------

		//------------ Main frame --------------
		//Preprocessing
		frame = preprocessing(frame, mask);

		//Difference between frame
		Mat foreground = diffFrame(background, frame);

		//Parking line
		drawParkingLotFun(frame);

		//Labeling 
		makeLabeling(frame, foreground);

		//FPS 측정 시간 끝
		string fps = to_string((int)video.get(CV_CAP_PROP_FPS));
		putText(frame, "FPS : " + fps, Point(10, 40), 2, 1.2, Scalar(0, 255, 0));
		//--------------------------------------

		imshow("PMS", frame);
		imshow("FOREGROUND", foreground);
		imshow("BACKGROUND", background);

		imshow("WARPING", warpFrame);
		imshow("BACKWARPFRAME", backWarpFrame);
		imshow("FOREWARPINGFRAME", foreWarpFrame);

		imshow("WARPING2", warpFrame2);
		imshow("BACKWARPFRAME2", backWarpFrame2);
		imshow("FOREWARPINGFRAME2", foreWarpFrame2);

		/*imshow("PARKING LOT", warpParkingLot);*/
		//Mouse callback Function
		setMouseCallback("PMS", mouseClickFun, NULL);
		setMouseCallback("WARPING", mouseClickFun, NULL);

		//Pushing ESC key is terminate
		if (waitKey(DELAYMILL) == ESC)
			break;
	}
	return 0;
}