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
	vector<ParkingLotArea> vecArea; //vector ����

									//Background extraction and preprocessing
	video >> background;
	resize(background, background, Size(background.cols / RESIZE, background.rows / RESIZE), 0, 0, CV_INTER_NN);
	//mask = preMasking(background);
	//background = preprocessing(background, mask);

	//���� ���� ����(����)
	setParkingLotPoint(&vecArea, background);

	int speed = 0;

	//FPS ����
	int64 freq, start, finish;
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	Ptr<BackgroundSubtractor> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2(500, 250, true);

	//MAIN LOOP
	while (true) {

		//����ó�� ����
		QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		//--------------MAIN-------------------

		video >> frame;							//Frame ����

		if (frame.empty()) {
			cout << "END OF VIDEO" << endl;		//End of video
			break;
		}

		waitKey(30);		//FPS ����(25- 27)			
		resize(frame, frame, Size(frame.cols / RESIZE, frame.rows / RESIZE), 0, 0, CV_INTER_NN);	//Resize Frame

		//Speed of Video
		if (++speed % 10)
			continue;

		//Mat foreground = diffFrameFun(frame, background, true);	//Forground ����
		Mat foreground = diffFrameFun2(frame, pMOG2);				//Forground ����
		foreground = maskingFun(frame, foreground, true);
		//--------------------------------------

		//----------------SUB-------------------

		//makeLabeling(frame, foreground);
		detectHaarcascadesCar(foreground);
		decideParkingLotPoint(frame, background, &vecArea);			// �������� ����
		drawParkingLotPoint(frame, &vecArea);						// �������� �׸���

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
