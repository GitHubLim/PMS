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
	vector<ParkingLotArea> vecArea; //vector ����

	//Background extraction and preprocessing
	video >> background;
	resize(background, background, cv::Size(background.cols / RESIZE, background.rows / RESIZE), 0, 0, CV_INTER_NN);
	//mask = preMasking(background);
	//background = preprocessing(background, mask);

	int speed = 0;

	//FPS ����
	int64 freq, start, finish;
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	//MAIN LOOP
	while (true) {

		//����ó�� ����
		QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		//--------------MAIN-------------------
		//Frame ����
		video >> frame;		//FRAME �б�
		//waitKey(30);		//FPS ����(25- 27)

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

		Mat foreground = diffFrameFun(frame, background, false);	//Forground ����
		setParkingLotPoint(&vecArea);								//���� ���� ����(����)
		//--------------------------------------

		//----------------SUB-------------------
		Mat warpFrame, backWarpFrame, foreWarpFrame;

		//���� ���� ����
		warpFrame = warpingFun(frame, vecArea[0]);				
		backWarpFrame = warpingFun(background, vecArea[0]);
		foreWarpFrame = diffFrameFun(backWarpFrame, warpFrame, true);

		//���� ���� �Ǵ� 
		decideParkingLotPoint(foreWarpFrame, &vecArea[0], 0.8, 0.2);

		//���� ���� �׸���
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