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

//DO NOT REVISE THIS MAIN CODE 
int main(void) {

	VideoCapture video(FILENAME);

	//Error in opening the video input
	if (!video.isOpened()) {
		cerr << "Unable to open video file: " << FILENAME << endl;
		exit(EXIT_FAILURE);
	}

	//GNU plot
	//CpGnuplotU plot(GNUPLOTLOCATE);
	//plot.cmd(_T("splot [x=-3:3] [y=-3:3] sin(x) * cos(y)"));

	Mat frame, background, mask;
	vector<ParkingLotArea> vecArea; //vector 영역

	//Background extraction and preprocessing
	video >> background;
	resize(background, background, cv::Size(background.cols / RESIZE, background.rows / RESIZE), 0, 0, CV_INTER_NN);
	//mask = preMasking(background);
	//background = preprocessing(background, mask);

	int speed = 0;

	//FPS 측정
	int64 freq, start, finish;
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	//MAIN LOOP
	while (true) {

		//영상처리 시작
		QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		//--------------MAIN-------------------
		//Frame 추출
		video >> frame;		//FRAME 읽기
		//waitKey(30);		//FPS 조정(25- 27)

		//Resize Frame
		resize(frame, frame, Size(frame.cols/ RESIZE, frame.rows/ RESIZE), 0, 0, CV_INTER_NN);

		//Speed of Video
		/*if (++speed % 10)
			continue;*/

		//End of video
		if (frame.empty()) {
			cout << "END OF VIDEO" << endl;
			break;
		}

		Mat foreground = diffFrameFun(frame, background, false);	//Forground 추출
		setParkingLotPoint(&vecArea);								//주차 공간 추출(수동)
		//--------------------------------------

		//----------------SUB-------------------
		Mat warpFrame, backWarpFrame, foreWarpFrame;

		//주차 공간 추출
		warpFrame = warpingFun(frame, vecArea[0]);				
		backWarpFrame = warpingFun(background, vecArea[0]);
		foreWarpFrame = diffFrameFun(backWarpFrame, warpFrame, true);

		//주차 공간 판단 
		decideParkingLotPoint(foreWarpFrame, &vecArea[0], 0.8, 0.2);

		//주차 공간 그리기
		drawParkingLotFun(frame, &vecArea);
		//--------------------------------------

		//Processing time (fps)
		QueryPerformanceCounter((_LARGE_INTEGER*)&finish);
		showFPS(frame, freq, start, finish);

		imshow("PMS", frame);
		imshow("FOREGROUND", foreground);


		//Mouse callback Function
		setMouseCallback("PMS", mouseClickFun, NULL);
		setMouseCallback("WARPING", mouseClickFun, NULL);
		setMouseCallback("WARPING2", mouseClickFun, NULL);

		char key = waitKey(10);

		if (key == ESC) break;						// ESC Key
		else if (key == SPACE) {					// SPACE Key
			while ((key = waitKey(10)) != SPACE && key != ESC);
			if (key == 27) break;
		}

	}
	return 0;
}