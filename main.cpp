#include <iostream>
#include <opencv2/opencv.hpp>
#include "pms.h"

using namespace std;
using namespace cv;

//DO NOT REVISE THIS MAIN CODE 
int main(void){

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
	namedWindow("ã�� ������ ����", CV_WINDOW_AUTOSIZE);

	//Ʈ���ٿ��� ���Ǵ� ���� �ʱ�ȭ 
	int LowH = 170;
	int HighH = 179;

	int LowS = 50;
	int HighS = 255;

	int LowV = 0;
	int HighV = 255;

	//Ʈ���� ���� 
	cvCreateTrackbar("LowH", "ã�� ������ ����", &LowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "ã�� ������ ����", &HighH, 179);

	cvCreateTrackbar("LowS", "ã�� ������ ����", &LowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "ã�� ������ ����", &HighS, 255);

	cvCreateTrackbar("LowV", "ã�� ������ ����", &LowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "ã�� ������ ����", &HighV, 255);

	//MAIN LOOP
	while(true){ 
		video >> frame;

		//FPS 
		string fps = to_string((int)video.get(CV_CAP_PROP_FPS));
		putText(frame, "FPS : " + fps, Point(10, 40), 2, 1.2, Scalar(0, 255, 0));

		//houghFrame = houghLine(frame);
		//kmeansFrame = kmeansClustering(frame);
		//ppFrame = preprocessing(frame);
		imshow("PMS", frame);

		//Pushing esc key is terminate
		if (waitKey(DELAYMILL) == ESC)
			break; 
	}
	return 0;
}

