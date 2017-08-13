#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <math.h>
#include <tchar.h>
#include "pGnuPlotU.h"
#include "pms.h"
#include "area.h"

using namespace std;
using namespace cv;

ParkingLotArea plArea1, plArea2;

//DO NOT REVISE THIS MAIN CODE 
int main(void) {

	VideoCapture video(FILENAME);

	//Error in opening the video input
	if (!video.isOpened()) {
		cerr << "Unable to open video file: " << FILENAME << endl;
		exit(EXIT_FAILURE);
	}

	//GNU plot
	CpGnuplotU plot(GNUPLOTLOCATE);
	plot.cmd(_T("splot [x=-3:3] [y=-3:3] sin(x) * cos(y)"));

	////VIDEO 320 X 240 
	//video.set(CAP_PROP_FRAME_WIDTH, 320);
	//video.set(CAP_PROP_FRAME_HEIGHT, 240);

	Mat frame, background, mask;

	//Background extraction and preprocessing
	video >> background;
	/*mask = preMasking(background);
	background = preprocessing(background, mask);*/
	
	int speed = 0;

	//FPS 측정
	int64 freq, start, finish;
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	//MAIN LOOP
	while (true) {
		
		//영상처리 시작
		QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		//Extracting frame
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
		//Warping 영역
		plArea1.setParkingLot(Point(247, 12), Point(255, 58), Point(294, 60), Point(280, 10));
	
		warpFrame = warpingFun(frame, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));
		backWarpFrame = warpingFun(background, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));

		//차영상(warping)
		Mat foreWarpFrame = diffFrame(backWarpFrame, warpFrame);

		//Labeling(warping)
		//makeLabeling(warpFrame, foreWarpFrame);

		drawParkingLotFun(warpFrame, plArea1);

		////주차 공간 판단
		//decideParkingLot(foreWarpFrame, &plArea1);

		//--------------------------------------

		//------------- Warping 2---------------
		Mat warpFrame2, backWarpFrame2;
		//Warping 영역
		plArea2.setParkingLot(Point(77, 77), Point(77, 100), Point(112, 96), Point(112, 73));

		warpFrame2 = warpingFun(frame, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));
		backWarpFrame2 = warpingFun(background, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));

		//차영상(warping)
		Mat foreWarpFrame2 = diffFrame(backWarpFrame2, warpFrame2);

		//Labeling(warping)
		//makeLabeling(warpFrame2, foreWarpFrame2);
		
		//주차 영역 그리기 
		//drawParkingLotFun(warpFrame2, plArea2);

		////주차 공간 판단
		//decideParkingLot(foreWarpFrame2, &plArea2);
		//--------------------------------------

		//----- warping !!-----
		Mat warpFrame3, backWarpFrame3;

		//Warping 영역
		warpFrame3 = warpingFun(warpFrame, Point(247, 12), Point(255, 58), Point(294, 60), Point(280, 10));
		backWarpFrame3 = warpingFun(backWarpFrame, Point(247, 12), Point(255, 58), Point(294, 60), Point(280, 10));

		//차영상(warping)
		Mat foreWarpFrame3 = diffFrame(backWarpFrame3, warpFrame3);

		decideParkingLot(foreWarpFrame3, &plArea1, 0.75);

		imshow("WARPING3", warpFrame3);
		imshow("BACKWARPFRAME3", backWarpFrame3);
		imshow("FOREWARPINGFRAME3", foreWarpFrame3);

		Mat warpFrame4, backWarpFrame4;

		//Warping 영역
		warpFrame4 = warpingFun(warpFrame2, Point(77, 77), Point(77, 100), Point(112, 96), Point(112, 73));
		backWarpFrame4 = warpingFun(backWarpFrame2, Point(77, 77), Point(77, 100), Point(112, 96), Point(112, 73));

		//차영상(warping)
		Mat foreWarpFrame4 = diffFrame(backWarpFrame4, warpFrame4);

		decideParkingLot(foreWarpFrame4, &plArea2, 0.3);

		drawParkingLotFun(warpFrame2, plArea2);
		imshow("WARPING4", warpFrame4);
		imshow("BACKWARPFRAME4", backWarpFrame4);
		imshow("FOREWARPINGFRAME4", foreWarpFrame4);

		//---------------------
		//------------ Main frame --------------
		//Preprocessing
		//frame = preprocessing(frame, mask);

		//Difference between frame
		Mat foreground = diffFrame(background, frame);

		//Parking line
		//drawParkingLotFun(frame);

		//Labeling 
		//makeLabeling(frame, foreground);

		//Processing time (fps)
		QueryPerformanceCounter((_LARGE_INTEGER*)&finish);
		double fps = freq / double(finish - start + 1);
		char fpsStr[20];
		sprintf_s(fpsStr, 20, "FPS: %.1lf", fps);
		putText(frame, fpsStr, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0, 255, 0), 2);

		//--------------------------------------

		//Car haarcascade
		//Mat carFrame = detectHaarcascadesCar(frame);

		imshow("PMS", frame);
		imshow("FOREGROUND", foreground);
		imshow("BACKGROUND", background);
		//imshow("CARCASCADE", carFrame);

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
		setMouseCallback("WARPING2", mouseClickFun, NULL);
		char key = waitKey(10);

		if (key == ESC) break;					// ESC Key
		else if (key == SPACE){					// SPACE Key
			while ((key = waitKey(10)) != SPACE && key != ESC);
			if (key == 27) break;
		}
	
	}
	return 0;
}