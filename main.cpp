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

	VideoCapture front_video(FRONTFILENAME);
	VideoCapture back_video(BACKFILENAME);

	//Error in opening the video input
	if (!front_video.isOpened() || !back_video.isOpened()) {
		cerr << "Unable to open video file: "<< endl;
		exit(EXIT_FAILURE);
	}

	/*FILE *fp = fopen("C:\\Users\\택기\\Desktop\\1.txt", "wt");
	if (fp) {
		fprintf(fp, "%d\t%d\t%.2f\n", min, max, PvalueProbablity);
		fclose(fp);
	}*/
	//GNU plot
	CpGnuplotU plot(GNUPLOTLOCATE);

	/*plot.cmd(_T("set grid"));
	plot.cmd(_T("set parametric"));
	plot.cmd(_T("set dgrid3d gauss"));*/
	plot.cmd(_T("set title \"각도에 따른 침범하는 영역 비율\""));   // 파일 불러오기
	plot.cmd(_T("set xlabel \"각도(degree) \""));   
	plot.cmd(_T("set ylabel \"영역 비율(percent)\""));   // y축 최대값 * 100
	plot.cmd(_T("plot [x=0:90] [y=0:100] 'C:\\Users\\택기\\Desktop\\1.txt' with lines"));
	
	//plot.cmd(_T("splot 'D:\\test\\1.txt' with lines"));   // 파일 불러오기


	//Background extraction and preprocessing

	// -------------- FRONTSIDE -----------------
	Mat front_frame, front_background, front_mask;
	vector<ParkingLotArea> vec_area_front; //vector 영역

	front_video >> front_background;
	resize(front_background, front_background, 
		Size(front_background.cols / RESIZE, front_background.rows / RESIZE), 0, 0, CV_INTER_NN);

	setParkingLotPoint(&vec_area_front, front_background, FRONTSIDE);

	Ptr<BackgroundSubtractor> p_mog2_front;
	p_mog2_front = createBackgroundSubtractorMOG2(500, 250, true);
	// -----------------------------------------

	// -------------- BACKSIDE -----------------
	Mat back_frame, back_background, back_mask;
	vector<ParkingLotArea> vec_area_back; //vector 영역

	back_video>> back_background;
	resize(back_background, back_background,
		Size(back_background.cols / RESIZE, back_background.rows / RESIZE), 0, 0, CV_INTER_NN);

	setParkingLotPoint(&vec_area_back, back_background, BACKSIDE);

	Ptr<BackgroundSubtractor> p_mog2_back;
	p_mog2_back = createBackgroundSubtractorMOG2(500, 250, true);
	// -----------------------------------------

	//mask = preMasking(background);
	//background = preprocessing(background, mask);

	//주차 공간 추출(수동)

	////// -------------------TEST-----------------------
	//VideoCapture test_video(TESTFILENAME);
	//// -------------- BACKSIDE -----------------
	//Mat back_frame, back_background, back_mask;
	//vector<ParkingLotArea> vec_area_back; //vector 영역

	//test_video >> back_background;
	////resize(back_background, back_background,
	////	Size(back_background.cols / RESIZE, back_background.rows / RESIZE), 0, 0, CV_INTER_NN);

	////setParkingLotPoint(&vec_area_back, back_background, BACKSIDE);
	//int ID = 0;	//ID 0부터 시작 
	////pushParkingLotPoint(&vec_area_back, 0, back_background, Point(396, 209), Point(402, 275), Point(457, 274), Point(442, 210), TEST);
	//pushParkingLotPoint(&vec_area_back, 0, back_background, Point(395, 235), Point(399, 303), Point(456, 302), Point(443, 231), TEST);
	////pushParkingLotPoint(&vec_area_back, 0, back_background, Point(386, 173), Point(392, 241), Point(449, 238), Point(434, 175), TEST);
	//
	//Ptr<BackgroundSubtractor> p_mog2_back;
	//p_mog2_back = createBackgroundSubtractorMOG2(500, 250, true);
	//// -----------------------------------------
	//// -----------------------------------------------
	int speed = 0;

	//FPS 측정
	int64 freq, start, finish;
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	//MAIN LOOP
	while (true) {
		//영상처리 시작
		QueryPerformanceCounter((_LARGE_INTEGER*)&start);
		//--------- TEST -------------
		//test_video >> back_frame;
		//////resize(back_frame, back_frame,
		////	Size(back_frame.cols / RESIZE, back_frame.rows / RESIZE), 0, 0, CV_INTER_NN);	//BACK
		//													
		//Mat back_foreground = diffFrameFun2(back_frame, p_mog2_back);
		//back_foreground = maskingFun(back_frame, back_foreground, true);

		////decideParkingLotPoint(back_frame, back_foreground, &vec_area_back);				// 주차공간 결정(BACK)
		//vec_area_back[0].setLevel(1);
		//drawParkingLotPoint(back_frame, &vec_area_back);								// 주차공간 그리기(BACK)

		//imshow("TEST", back_frame);
		//imshow("BACK", vec_area_back[0].getBackground());
		//setMouseCallback("TEST", mouseClickFun, NULL);
		//--------------MAIN-------------------
		front_video >> front_frame;							//Frame 추출
		back_video >> back_frame;

		if (front_frame.empty() || back_frame.empty()) {
			cout << "END OF VIDEO" << endl;		//End of video
			break;
		}

		////------------- SPEED ------------------
		//Speed of Video
		if (++speed % 10)
			continue;

		//waitKey(35);		//FPS 조정(25- 27)
		////--------------------------------------


		//------------- RESIZE ------------------
		resize(front_frame, front_frame, 
			Size(front_frame.cols / RESIZE, front_frame.rows / RESIZE), 0, 0, CV_INTER_NN);	//FRONT
		resize(back_frame, back_frame,
			Size(back_frame.cols / RESIZE, back_frame.rows / RESIZE), 0, 0, CV_INTER_NN);	//BACK
		//--------------------------------------
		
		
		//---------------FORGROUND--------------
		//Mat foreground = diffFrameFun(frame, background, true);	
		Mat front_foreground = diffFrameFun2(front_frame, p_mog2_front);	
		front_foreground = maskingFun(front_frame, front_foreground, true);

		Mat back_foreground = diffFrameFun2(back_frame, p_mog2_back);
		back_foreground = maskingFun(back_frame, back_foreground, true);
		//--------------------------------------

		//---------------DECIDE------------------
		decideParkingLotPoint(front_frame, front_background, &vec_area_front);			// 주차공간 결정(FRONT)
		decideParkingLotPoint(back_frame, back_foreground, &vec_area_back);				// 주차공간 결정(BACK)

		//detectHaarcascadesCar(front_foreground);
		//detectHaarcascadesCar(back_foreground);
		//---------------------------------------

		//-------------REVISEPROIOR--------------
		prioritizeParkingLotLevel(&vec_area_front, &vec_area_back);
		//---------------------------------------


		//----------------DRAW-------------------
		drawParkingLotPoint(front_frame, &vec_area_front);								// 주차공간 그리기(FRONT)
		drawParkingLotPoint(back_frame, &vec_area_back);								// 주차공간 그리기(BACK)
		//---------------------------------------

		//Processing time (FPS)
		QueryPerformanceCounter((_LARGE_INTEGER*)&finish);
		//showFPSFun(front_frame, freq, start, finish);

		imshow("FORNT_PMS", front_frame);
		imshow("BACK_PMS", back_frame);
		imshow("FRONT_FOREGROUND", front_foreground);

		//Mouse Callback Function
		setMouseCallback("FORNT_PMS", mouseClickFun, NULL);
		setMouseCallback("BACK_PMS", mouseClickFun, NULL);

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
