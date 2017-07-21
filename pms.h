#pragma once
#ifndef PMS_HEADER
#define PMS_HEADER

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>

#define FILENAME "rainy1.mp4"
#define ESC 27
#define DELAYMILL 20
#define PI 3.14159265
#define DEG2RAD PI/180
#define K 2

#define LOWH 0
#define HIGHH 70
#define LOWS 21 
#define HIGHS 255
#define LOWV 31
#define HIGHV 255

using namespace std;
using namespace cv;


//DO NOT USE FUNCTION
Mat houghLine(Mat srcFrame) {
	//Exception handling (Image is Empty)
	if (srcFrame.empty() == true) {
		cout << "srcFrame is Empty" << endl;
	}

	Mat dstFrame, grayFrame;
	dstFrame = srcFrame.clone();
	cvtColor(srcFrame, grayFrame, COLOR_BGR2GRAY);

	//1. GaussianBular and Canny Edge Detection
	Mat edgesrcFrame;
	GaussianBlur(grayFrame, grayFrame, Size(9, 9), 1.0);
	Canny(grayFrame, edgesrcFrame, 150, 350);


	//2. Mapping of edge points to the Hough space and storage in an accumulator
	/*vector<Vec4i> lines;
	HoughLinesP()*/
	return edgesrcFrame;
}

Mat kmeansClustering(Mat srcFrame) {
	
	//�迭�� 8bit uchar type���� ����Ǿ� ����.
	Mat colorTable(K, 1, CV_8UC3);
	Vec3b color;


	//destination �̹��� �����
	Mat dstFrame(srcFrame.size(), srcFrame.type());

	//SIZE ����
	cout << "SIZE : " << srcFrame.size() << endl;

	int attempts = 1;
	int flags = KMEANS_RANDOM_CENTERS;
	TermCriteria criteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 1.0);

	Mat samples = srcFrame.reshape(3, srcFrame.rows*srcFrame.cols);
	cout << samples.size() << endl;

	samples.convertTo(samples, CV_32FC3);

	Mat labels;
	srcFrame.convertTo(srcFrame, CV_32FC3);

	kmeans(samples, K, labels, criteria, attempts, flags);

	//output size
	cout << labels.size() << endl;

	// �ð�����
	clock_t begin, end;

	begin = clock();

	for (int y = 0, n = 0; y < srcFrame.rows; y++) {
		for (int x = 0; x < srcFrame.cols; x++, n++) {
			int idx = labels.at<int>(n);
			//idx�� ������ (idx = �����迭)
			color = colorTable.at<Vec3b>(idx);
			dstFrame.at<Vec3b>(y, x) = color;
		}
	}

	cout << srcFrame.size() << endl;
	end = clock();

	cout << "K-means ����ð� : " << (float)(end - begin) / CLOCKS_PER_SEC << endl;

	return dstFrame;
}

Mat preMasking(Mat srcFrame) {
	Mat dstFrame;
	
	//Specific HSV Removal
	Mat hsvFrame, binaryFrame;

	//1. convert to HSV
	cvtColor(srcFrame, hsvFrame, COLOR_BGR2HSV);

	//2. hsv frame to binary frame
	inRange(hsvFrame, Scalar(LOWH, LOWS, LOWV), Scalar(HIGHH, HIGHS, HIGHV), binaryFrame);


	//3. erode and dilate to binary frame
	erode(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	dilate(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//4. not bitwise
	bitwise_not(binaryFrame, binaryFrame);

	return binaryFrame;
}

Mat preprocessing(Mat srcFrame){
	Mat dstFrame, mask;

	//1. masking 
	mask = preMasking(srcFrame);
	srcFrame.copyTo(dstFrame, mask);

	return dstFrame;
}

Mat drawCircle(Mat srcFrame) {
	//����� ������ ��
	circle(srcFrame, Point(345, 300), 3, Scalar(0, 0, 255), 1);
	circle(srcFrame, Point(397, 303), 3, Scalar(0, 0, 255), 1);
	circle(srcFrame, Point(402, 355), 3, Scalar(0, 0, 255), 1);
	circle(srcFrame, Point(343, 347), 3, Scalar(0, 0, 255), 1);

	//���� ù��° ��
	circle(srcFrame, Point(158, 450), 3, Scalar(0, 0, 225), 1);
	//���� �ι�° ��
	circle(srcFrame, Point(171, 420), 3, Scalar(0, 0, 225), 1);
	//���� ����° ��
	circle(srcFrame, Point(189, 382), 3, Scalar(0, 0, 225), 1);
	//���� �׹�° ��
	circle(srcFrame, Point(204, 352), 3, Scalar(0, 0, 225), 1);
	//���� �ټ���° ��
	circle(srcFrame, Point(215, 326), 3, Scalar(0, 0, 225), 1);
	//���� ������° ��
	circle(srcFrame, Point(226, 304), 3, Scalar(0, 0, 225), 1);
	//���� �ϰ���° ��
	circle(srcFrame, Point(237, 283), 3, Scalar(0, 0, 225), 1);
	//���� ������° ��
	circle(srcFrame, Point(245, 266), 3, Scalar(0, 0, 225), 1);
	//���� ��ȩ��° ��
	circle(srcFrame, Point(252, 251), 3, Scalar(0, 0, 225), 1);
	//���� ����° ��
	circle(srcFrame, Point(257, 237), 3, Scalar(0, 0, 225), 1);
	//���� ���ѹ�° ��
	circle(srcFrame, Point(264, 225), 3, Scalar(0, 0, 225), 1);
	//���� ���ι�° ��
	circle(srcFrame, Point(269, 214), 3, Scalar(0, 0, 225), 1);
	//���� ������° ��
	circle(srcFrame, Point(272, 204), 3, Scalar(0, 0, 225), 1);
	//���� ���׹�° ��
	circle(srcFrame, Point(278, 194), 3, Scalar(0, 0, 225), 1);
	//���� ���ټ���° ��
	circle(srcFrame, Point(281, 186), 3, Scalar(0, 0, 225), 1);
	//���� ��������° ��
	circle(srcFrame, Point(285, 177), 3, Scalar(0, 0, 225), 1);
	//���� ���ϰ���° ��
	circle(srcFrame, Point(287, 171), 3, Scalar(0, 0, 225), 1);
	//���� ��������° ��
	circle(srcFrame, Point(291, 165), 3, Scalar(0, 0, 225), 1);

	//����������� ������ ù��° ���� ��
	circle(srcFrame, Point(449, 359), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
	circle(srcFrame, Point(437, 305), 3, Scalar(0, 0, 225), 1); //�� ��

															 //����������� ������ �ι�° ���� ��
	circle(srcFrame, Point(495, 363), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
	circle(srcFrame, Point(477, 307), 3, Scalar(0, 0, 225), 1); //�� ��

															 //����������� ������ ����° ���� ��
	circle(srcFrame, Point(542, 367), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
	circle(srcFrame, Point(518, 309), 3, Scalar(0, 0, 225), 1); //�� ��

															 //����������� ������ �׹�° ���� ��
	circle(srcFrame, Point(590, 370), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
	circle(srcFrame, Point(563, 311), 3, Scalar(0, 0, 225), 1); //�� ��

															 //����������� ������ �ټ���° ���� ��
	circle(srcFrame, Point(636, 374), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
	circle(srcFrame, Point(605, 314), 3, Scalar(0, 0, 225), 1); //�� ��

	return srcFrame;
}

void mouseClickFun(int event, int x, int y, int flags, void* userdata){

	if (event == EVENT_LBUTTONDOWN){
		cout << "��ǥ = (" << x << ", " << y << ")" << endl;
	}
}

#endif