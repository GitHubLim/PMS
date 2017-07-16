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

using namespace std;
using namespace cv;


//DO NOT USE FUNCTION
Mat houghLine(Mat srcFrame) {
	//Exception handling (Image is Empty)
	if (srcFrame.empty() == true) {
		cout << "Frame is Empty" << endl;
	}

	Mat dstFrame, grayFrame;
	dstFrame = srcFrame.clone();
	cvtColor(srcFrame, grayFrame, COLOR_BGR2GRAY);

	//1. GaussianBular and Canny Edge Detection
	Mat edgeFrame;
	GaussianBlur(grayFrame, grayFrame, Size(9, 9), 1.0);
	Canny(grayFrame, edgeFrame, 150, 350);


	//2. Mapping of edge points to the Hough space and storage in an accumulator
	/*vector<Vec4i> lines;
	HoughLinesP()*/
	return edgeFrame;
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

double calculateFPS() {
	// method 2
	double fps = 0;
	int64 t = getTickCount(); //���� ���� �ð�

    // ���� �ʿ��� ����
	t = getTickCount() - t; //���� �Ϸ� �ð�
	fps = 1000 / t / getTickFrequency(); // fps ���

	return fps;
}

Mat preMasking(Mat srcFrame) {
	return srcFrame;
}

Mat preprocessing(Mat srcFrame){
	Mat dstFrame, mask;

	//1. masking 
	mask = preMasking(srcFrame);
	srcFrame.copyTo(dstFrame, mask);

	return dstFrame;
}
#endif