#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
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
	resize(background, background, Size(background.cols / RESIZE, background.rows / RESIZE), 0, 0, CV_INTER_NN);
	//mask = preMasking(background);
	//background = preprocessing(background, mask);

	//주차 공간 추출(수동)
	setParkingLotPoint(&vecArea, background);

	int speed = 0;

	//FPS 측정
	int64 freq, start, finish;
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	Ptr<BackgroundSubtractor> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2(500, 250, true);

	//MAIN LOOP
	while (true) {

		//영상처리 시작
		QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		//--------------MAIN-------------------

		video >> frame;							//Frame 추출

		if (frame.empty()) {
			cout << "END OF VIDEO" << endl;		//End of video
			break;
		}

		waitKey(30);		//FPS 조정(25- 27)			
		resize(frame, frame, Size(frame.cols / RESIZE, frame.rows / RESIZE), 0, 0, CV_INTER_NN);	//Resize Frame

		//Speed of Video
		if (++speed % 10)
			continue;

		//Mat foreground = diffFrameFun(frame, background, true);	//Forground 추출
		Mat foreground = diffFrameFun2(frame, pMOG2);				//Forground 추출
		foreground = maskingFun(frame, foreground, true);
		//--------------------------------------

		//----------------SUB-------------------

		//makeLabeling(frame, foreground);
		detectHaarcascadesCar(foreground);
		decideParkingLotPoint(frame, background, &vecArea);			// 주차공간 결정
		drawParkingLotPoint(frame, &vecArea);						// 주차공간 그리기

		//--------------------------------------

		//Processing time (FPS)
		QueryPerformanceCounter((_LARGE_INTEGER*)&finish);
		showFPSFun(frame, freq, start, finish);

		imshow("PMS", frame);
		imshow("FOREGROUND", foreground);

		//Mouse Callback Function
		setMouseCallback("PMS", mouseClickFun, NULL);

		//Key Event controller
		char key = waitKey(10);

		if (key == ESC) break;				// ESC Key
		else if (key == SPACE) {			// SPACE Key
			while ((key = waitKey(10)) != SPACE && key != ESC);
			if (key == 27) break;
		}
	}

	return 0;
}
