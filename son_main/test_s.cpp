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
	//namedWindow("ã�� ������ ����", CV_WINDOW_AUTOSIZE);

	//MAIN LOOP
	while (true) {
		video >> frame;

		resize(frame, frame, Size(800, 600), 0, 0, CV_INTER_LINEAR);

		//�������� ��ġȮ��
		//frame = drawCircle(frame);

		//��ó��
		frame = preprocessing(frame);

		//FPS 
		string fps = to_string((int)video.get(CV_CAP_PROP_FPS));
		putText(frame, "FPS : " + fps, Point(10, 40), 2, 1.2, Scalar(0, 255, 0));

		//frame = carHaarCascadeFun(frame);

		//��ġ�� �´� HoughLine (����)

		Mat frameROIFirst;															        //���� ������κ� ����
		frameROIFirst = frame(Rect(1, 1, 252, 599));										//Rect(�ʱ� x��, �ʱ� y��, �ʱ� x�� + �߰� �Ÿ�, �ʱ� y�� + �߰��Ÿ�);
		Mat ROIhoughLineFirst;																//���� ������κ� houghLine�� ����
		ROIhoughLineFirst = houghLine(frameROIFirst);
		addWeighted(frameROIFirst, 0, ROIhoughLineFirst, 1, 0., frameROIFirst);

		Mat frameROISecond;																	//������ ������κ� ����
		frameROISecond = frame(cv::Rect(350, 300, 450, 110));
		Mat ROIhoughLineSecond;																//������ ������κ� houghLine�� ����
		ROIhoughLineSecond = houghLine(frameROISecond);
		addWeighted(frameROISecond, 0, ROIhoughLineSecond, 1, 0., frameROISecond);

		imshow("PMS", frame);
		//Mat framehoughLine = houghLine(frame);
		

		/*Mat image = imread("test2.PNG", 1);
		frame = houghLine(image)*/

		//imshow("PMS", framehoughLine);
		//imshow("����ȭ ����", img_binary);

		//mouse call back 
		setMouseCallback("PMS", mouseClickFun, NULL);

		//Pushing ESC key is terminate
		if (waitKey(DELAYMILL) == ESC)
			break;
	}
	return 0;
}