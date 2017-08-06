#include <iostream>
#include <opencv2/opencv.hpp>
#include "pms.h"

using namespace std;
using namespace cv;

//DO NOT REVISE THIS MAIN CODE 
int main(void) {

	VideoCapture video(FILENAME);

	//Video is not open
	if (!video.isOpened())
		cout << "Could not open video file." << endl;


	//VIDEO 320 X 240 
	video.set(CAP_PROP_FRAME_WIDTH, 320);
	video.set(CAP_PROP_FRAME_HEIGHT, 240);

	Mat frame, houghFrame, kmeansFrame, ppFrame;
	int flag = 0;
	//HSV Trackbar
	//namedWindow("찾을 색범위 설정", CV_WINDOW_AUTOSIZE);

	//MAIN LOOP
	while (true) {
		video >> frame;

		resize(frame, frame, Size(800, 600), 0, 0, CV_INTER_LINEAR);

		//수동으로 위치확인
		//frame = drawCircle(frame);

		//전처리
		frame = preprocessing(frame);

		//FPS 
		string fps = to_string((int)video.get(CV_CAP_PROP_FPS));
		putText(frame, "FPS : " + fps, Point(10, 40), 2, 1.2, Scalar(0, 255, 0));

		//frame = carHaarCascadeFun(frame);

		//위치에 맞는 HoughLine (수정)

		Mat frameROIFirst;															        //왼쪽 주차장부분 영상
		frameROIFirst = frame(Rect(1, 1, 252, 599));										//Rect(초기 x값, 초기 y값, 초기 x값 + 추가 거리, 초기 y값 + 추가거리);
		Mat ROIhoughLineFirst;																//왼쪽 주차장부분 houghLine한 영상
		ROIhoughLineFirst = houghLine(frameROIFirst);
		addWeighted(frameROIFirst, 0, ROIhoughLineFirst, 1, 0., frameROIFirst);

		Mat frameROISecond;																	//오른쪽 주차장부분 영상
		frameROISecond = frame(cv::Rect(350, 300, 450, 110));
		Mat ROIhoughLineSecond;																//오른쪽 주차장부분 houghLine한 영상
		ROIhoughLineSecond = houghLine(frameROISecond);
		addWeighted(frameROISecond, 0, ROIhoughLineSecond, 1, 0., frameROISecond);

		imshow("PMS", frame);
		//Mat framehoughLine = houghLine(frame);
		

		/*Mat image = imread("test2.PNG", 1);
		frame = houghLine(image)*/

		//imshow("PMS", framehoughLine);
		//imshow("이진화 영상", img_binary);

		//mouse call back 
		setMouseCallback("PMS", mouseClickFun, NULL);

		//Pushing ESC key is terminate
		if (waitKey(DELAYMILL) == ESC)
			break;
	}
	return 0;
}