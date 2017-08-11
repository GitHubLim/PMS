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

	Mat frame, houghFrame, kmeansFrame, ppFrame, background, mask;
	int flag = 0;

	//Background frame
	video >> background;
	mask = preMasking(background);
	background = preprocessing(background, mask);
	
	//HSV Trackbar
	//namedWindow("찾을 색범위 설정", CV_WINDOW_AUTOSIZE);

	//트랙바에서 사용되는 변수 초기화 
	/*int LowH = LOWH;
	int HighH = HIGHH;
	int LowS = LOWS;
	int HighS = HIGHS;
	int LowV = LOWV;
	int HighV = HIGHV;*/

	////트랙바 생성 
	//cvCreateTrackbar("LowH", "찾을 색범위 설정", &LowH, 179); //Hue (0 - 179)
	//cvCreateTrackbar("HighH", "찾을 색범위 설정", &HighH, 179);

	//cvCreateTrackbar("LowS", "찾을 색범위 설정", &LowS, 255); //Saturation (0 - 255)
	//cvCreateTrackbar("HighS", "찾을 색범위 설정", &HighS, 255);

	//cvCreateTrackbar("LowV", "찾을 색범위 설정", &LowV, 255); //Value (0 - 255)
	//cvCreateTrackbar("HighV", "찾을 색범위 설정", &HighV, 255);
	//FPS 측정시작
	//clock_t startTime = clock(); //측정 시작 시간

	//MAIN LOOP
	int count = 0;
	while (true) {
		video >> frame;

		if (++count % 10)
			continue;
			
		//resize(frame, frame, Size(800, 600), 0, 0, CV_INTER_LINEAR);

		//Rotate
		//Point2f src_center(frame.cols / 2.0F, frame.rows / 2.0F);
		//Mat rot_matrix = getRotationMatrix2D(src_center, 180, 1.0);
		//warpAffine(frame, frame, rot_matrix, frame.size());

		////라벨링 
		//Mat img_labels, stats, centroids;
		//int numOfLables = connectedComponentsWithStats(img_binary, img_labels,
		//	stats, centroids, 8, CV_32S);

		////영역박스 그리기
		//int max = -1, idx = 0;
		//for (int j = 1; j < numOfLables; j++) {
		//	int area = stats.at<int>(j, CC_STAT_AREA);
		//	if (max < area)
		//	{
		//		max = area;
		//		idx = j;
		//	}
		//}

		//int left = stats.at<int>(idx, CC_STAT_LEFT);
		//int top = stats.at<int>(idx, CC_STAT_TOP);
		//int width = stats.at<int>(idx, CC_STAT_WIDTH);
		//int height = stats.at<int>(idx, CC_STAT_HEIGHT);


		/*rectangle(frame, Point(left, top), Point(left + width, top + height),
		Scalar(0, 0, 255), 1);*/

		//수동으로 위치확인
		//frame = drawCircle(frame);

		//전처리
		frame = preprocessing(frame, mask);

		//차영상
		Mat foreground = diffFrame(background, frame);

		//Warping
		//Mat warpFrame = warpingFun(frame);
	
		//FPS 측정 시간 끝 
		string fps = to_string((int)video.get(CV_CAP_PROP_FPS));

		putText(frame, "FPS : " + fps, Point(10, 40), 2, 1.2, Scalar(0, 255, 0));

		//cout << tempTime << endl;
		//frame = carHaarCascadeFun(frame);

		

		//HoughLine
		//frame = houghLine(frame);
		/*Mat image = imread("test2.PNG", 1);
		frame = houghLine(image)*/

		//------------- Warping ---------------
		Mat warpFrame, backWarpFrame;
		warpFrame= warpingFun(frame, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));
		backWarpFrame = warpingFun(background, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));

		//차영상(warping)
		Mat foreWarpFrame = diffFrame(backWarpFrame, warpFrame);

		//Labeling(warping)
		makeLabeling(warpFrame, foreWarpFrame);

		drawWarpParkingLotFun(warpFrame);

		//주차공간 확인
		decideParkingLot(warpFrame, foreWarpFrame);
		//--------------------------------------

		//------------- Warping 2---------------
		Mat warpFrame2, backWarpFrame2;
		warpFrame2 = warpingFun(frame, Point(209,200), Point(63, 399), Point(180, 422), Point(274, 211));
		backWarpFrame2 = warpingFun(background, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));

		//차영상(warping)
		Mat foreWarpFrame2 = diffFrame(backWarpFrame2, warpFrame2);

		//Labeling(warping)
		makeLabeling(warpFrame2, foreWarpFrame2);
		//--------------------------------------

		//Parking line
		drawParkingLotFun(frame);

		//Labeling 
		makeLabeling(frame, foreground);

		imshow("PMS", frame);
		imshow("FOREGROUND", foreground);
		imshow("BACKGROUND", background);

		imshow("WARPING", warpFrame);
		imshow("BACKWARPFRAME", backWarpFrame);
		imshow("FOREWARPINGFRAME", foreWarpFrame);

		imshow("WARPING2", warpFrame2);
		imshow("BACKWARPFRAME2", backWarpFrame2);
		imshow("FOREWARPINGFRAME2", foreWarpFrame2);
		//imshow("이진화 영상", img_binary);

		//mouse call back 
		setMouseCallback("PMS", mouseClickFun, NULL);
		setMouseCallback("WARPING", mouseClickFun, NULL);
		//Pushing ESC key is terminate
		if (waitKey(DELAYMILL) == ESC)
			break;
	}
	return 0;
}