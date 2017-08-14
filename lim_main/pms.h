#pragma once
#ifndef PMS_MAIN_HEADER
#define PMS_MAIN_HEADER

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <Windows.h>
#include <math.h>
#include <tchar.h>
#include <vector>
#include "area.h"

#define FILENAME "rainy2.mp4"
#define GNUPLOTLOCATE _T("C:\\Program Files\\gnuplot\\bin\\wgnuplot.exe")

#define ESC 27
#define SPACE 32

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

#define SCALEFIRST 9
#define SCALESECOND 19

#define SCALETHIRD 37


using namespace std;
using namespace cv;

typedef struct HoughLineInfo {
	vector<unsigned int> iAngle;
	vector<double> dRho;
}HoughLineInfo;

Mat houghLine(Mat srcFrame) {

	//Exception handling (Image is Empty)
	if (srcFrame.empty() == true) {
		cout << "srcFrame is Empty" << endl;
	}

	Mat dstFrame;
	srcFrame.copyTo(dstFrame);
	// 1.  Edge detection, e.g. using the Canny edge detector.
	Mat edgeFrame;
	GaussianBlur(srcFrame, srcFrame, Size(9, 9), 1.0);
	Canny(srcFrame, edgeFrame, 50, 100);


	//2. Mapping of edge points to the Hough space and storage in an accumulator

	const unsigned int kAngleSize = 180;
	const unsigned int kDistMax = sqrt((edgeFrame.cols*edgeFrame.cols) + (edgeFrame.rows*edgeFrame.rows)) + 1;
	const unsigned int kDistanceSize = kDistMax * 2;
	const unsigned int kThresHoldForLine = 100;

	unsigned int* pNvote = new unsigned int[kAngleSize*kDistanceSize];
	memset(pNvote, 0, (kAngleSize*kDistanceSize) * sizeof(unsigned int));

	double lukCos[180];
	double lukSin[180];
	unsigned int lukAngleIdx[180];

	for (int i = 0; i < kAngleSize; i++)
	{
		double angle = (double)i*DEG2RAD;

		lukCos[i] = cos(angle);
		lukSin[i] = sin(angle);
		lukAngleIdx[i] = i*kDistanceSize;
	}

	unsigned char* ucMatEdgeData = edgeFrame.data;
	for (int y = 0; y < edgeFrame.rows; y++)
	{
		for (int x = 0; x < edgeFrame.cols; x++)
		{
			if (*ucMatEdgeData++ == 0)continue;

			for (unsigned int j = 0; j < kAngleSize; j++)
			{
				double rho = lukCos[j] * x + lukSin[j] * y;
				rho += (double)kDistMax;

				pNvote[lukAngleIdx[j] + (int)(rho + 0.5)]++;
			}
		}
	}

	//3. Interpretation of the accumulator to yield lines of infinite length. 
	//The interpretation is done by thresholding and possibly other constraints.

	Point ptA;
	Point ptB;

	HoughLineInfo lineVec;

	vector<Vec2f> linesVec;

	for (int i = 0; i < 180; i++)
		for (int j = 0; j < kDistanceSize; j++)
		{
			int nVote = pNvote[lukAngleIdx[i] + j];
			if (nVote >= kThresHoldForLine)
			{
				bool isTrueLine = true;

				for (int dAngle = -1; dAngle <= 1 && isTrueLine; dAngle++)
				{
					if (i + dAngle < 0)continue;
					if (i + dAngle >= kAngleSize)break;

					for (int dRho = -1; dRho <= 1 && isTrueLine; dRho++)
					{
						if (j + dRho < 0)continue;
						if (j + dRho >= kDistanceSize)break;

						if (pNvote[lukAngleIdx[i + dAngle] + (j + dRho)] > nVote)isTrueLine = false;
					}
				}

				if (isTrueLine == false) continue;

				lineVec.iAngle.push_back(i);
				lineVec.dRho.push_back(j - (int)kDistMax);
			}
		}

	int nLineVecSize = lineVec.dRho.size();
	for (int i = 0; i < nLineVecSize; i++)
	{
		int angle = lineVec.iAngle[i];

		double cX = lukCos[angle];
		double cY = lukSin[angle];

		double rho = lineVec.dRho[i];

		double x0 = cX*rho;
		double y0 = cY*rho;

		ptA.x = cvRound(x0 + 1000. * (-cY));
		ptA.y = cvRound(y0 + 1000. * (cX));
		ptB.x = cvRound(x0 - 1000. * (-cY));
		ptB.y = cvRound(y0 - 1000. * (cX));

		line(dstFrame, ptA, ptB, Scalar(0, 255, 0), 1);
	}

	delete[] pNvote;

	return dstFrame;
}

Mat kmeansClustering(Mat srcFrame) {
	
	//배열에 8bit uchar type으로 저장되어 있음.
	Mat colorTable(K, 1, CV_8UC3);
	Vec3b color;


	//destination 이미지 만들기
	Mat dstFrame(srcFrame.size(), srcFrame.type());

	//SIZE 추출
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

	// 시간측정
	clock_t begin, end;

	begin = clock();

	for (int y = 0, n = 0; y < srcFrame.rows; y++) {
		for (int x = 0; x < srcFrame.cols; x++, n++) {
			int idx = labels.at<int>(n);
			//idx로 추출함 (idx = 정수배열)
			color = colorTable.at<Vec3b>(idx);
			dstFrame.at<Vec3b>(y, x) = color;
		}
	}

	cout << srcFrame.size() << endl;
	end = clock();

	cout << "K-means 수행시간 : " << (float)(end - begin) / CLOCKS_PER_SEC << endl;

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
	erode(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(SCALEFIRST, SCALEFIRST)));
	dilate(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(SCALESECOND, SCALESECOND)));

	dilate(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(SCALEFIRST, SCALEFIRST)));
	erode(binaryFrame, binaryFrame, getStructuringElement(MORPH_ELLIPSE, Size(SCALESECOND, SCALESECOND)));

	//4. not bitwise
	bitwise_not(binaryFrame, binaryFrame);

	return binaryFrame;
}

Mat preprocessing(Mat srcFrame, Mat mask){
	Mat dstFrame;

	//1. masking 
	srcFrame.copyTo(dstFrame, mask);

	return dstFrame;
}

Mat drawCircle(Mat srcFrame) {
	//장애인 주차장 점
	circle(srcFrame, Point(345, 300), 3, Scalar(0, 0, 255), 1);
	circle(srcFrame, Point(397, 303), 3, Scalar(0, 0, 255), 1);
	circle(srcFrame, Point(402, 355), 3, Scalar(0, 0, 255), 1);
	circle(srcFrame, Point(343, 347), 3, Scalar(0, 0, 255), 1);

	//왼쪽 첫번째 점
	circle(srcFrame, Point(158, 450), 3, Scalar(0, 0, 225), 1);
	//왼쪽 두번째 점
	circle(srcFrame, Point(171, 420), 3, Scalar(0, 0, 225), 1);
	//왼쪽 세번째 점
	circle(srcFrame, Point(189, 382), 3, Scalar(0, 0, 225), 1);
	//왼쪽 네번째 점
	circle(srcFrame, Point(204, 352), 3, Scalar(0, 0, 225), 1);
	//왼쪽 다섯번째 점
	circle(srcFrame, Point(215, 326), 3, Scalar(0, 0, 225), 1);
	//왼쪽 여섯번째 점
	circle(srcFrame, Point(226, 304), 3, Scalar(0, 0, 225), 1);
	//왼쪽 일곱번째 점
	circle(srcFrame, Point(237, 283), 3, Scalar(0, 0, 225), 1);
	//왼쪽 여덟번째 점
	circle(srcFrame, Point(245, 266), 3, Scalar(0, 0, 225), 1);
	//왼쪽 아홉번째 점
	circle(srcFrame, Point(252, 251), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열번째 점
	circle(srcFrame, Point(257, 237), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열한번째 점
	circle(srcFrame, Point(264, 225), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열두번째 점
	circle(srcFrame, Point(269, 214), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열세번째 점
	circle(srcFrame, Point(272, 204), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열네번째 점
	circle(srcFrame, Point(278, 194), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열다섯번째 점
	circle(srcFrame, Point(281, 186), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열여섯번째 점
	circle(srcFrame, Point(285, 177), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열일곱번째 점
	circle(srcFrame, Point(287, 171), 3, Scalar(0, 0, 225), 1);
	//왼쪽 열여덟번째 점
	circle(srcFrame, Point(291, 165), 3, Scalar(0, 0, 225), 1);

	//장애인주차장 오른쪽 첫번째 주차 점
	circle(srcFrame, Point(449, 359), 3, Scalar(0, 0, 225), 1); // 아래 점
	circle(srcFrame, Point(437, 305), 3, Scalar(0, 0, 225), 1); //위 점

															 //장애인주차장 오른쪽 두번째 주차 점
	circle(srcFrame, Point(495, 363), 3, Scalar(0, 0, 225), 1); // 아래 점
	circle(srcFrame, Point(477, 307), 3, Scalar(0, 0, 225), 1); //위 점

															 //장애인주차장 오른쪽 세번째 주차 점
	circle(srcFrame, Point(542, 367), 3, Scalar(0, 0, 225), 1); // 아래 점
	circle(srcFrame, Point(518, 309), 3, Scalar(0, 0, 225), 1); //위 점

															 //장애인주차장 오른쪽 네번째 주차 점
	circle(srcFrame, Point(590, 370), 3, Scalar(0, 0, 225), 1); // 아래 점
	circle(srcFrame, Point(563, 311), 3, Scalar(0, 0, 225), 1); //위 점

															 //장애인주차장 오른쪽 다섯번째 주차 점
	circle(srcFrame, Point(636, 374), 3, Scalar(0, 0, 225), 1); // 아래 점
	circle(srcFrame, Point(605, 314), 3, Scalar(0, 0, 225), 1); //위 점

	return srcFrame;
}

Mat warpingFun(Mat srcFrame, Point topLeft, Point bottomLeft, Point bottomRight, Point topRight) {

	double w1 = sqrt(pow(bottomRight.x - bottomLeft.x, 2) + pow(bottomRight.y - bottomLeft.y, 2));
	double w2 = sqrt(pow(topRight.x - topLeft.x, 2) + pow(topRight.y - topLeft.y, 2));
	double h1 = sqrt(pow(topRight.x - bottomRight.x, 2) + pow(topRight.y - bottomRight.y, 2));
	double h2 = sqrt(pow(topLeft.x - bottomLeft.x, 2) + pow(topLeft.y - bottomLeft.y, 2));

	double maxWidth = (w1 > w2) ? w1 : w2;
	double maxHeight = (h1 > h2) ? h1 : h2;

	//warping 전,후 좌표(src, dst)
	Point2f src[4], dst[4];
	src[0] = Point2f(topLeft.x, topLeft.y);            //왼쪽 위
	src[1] = Point2f(topRight.x, topRight.y);         //오른쪽 위
	src[2] = Point2f(bottomRight.x, bottomRight.y);      //오른쪽 아래
	src[3] = Point2f(bottomLeft.x, bottomLeft.y);      //왼쪽 아래

	dst[0] = Point2f(0, 0);
	dst[1] = Point2f(maxWidth - 1, 0);
	dst[2] = Point2f(maxWidth - 1, maxHeight - 1);
	dst[3] = Point2f(0, maxHeight - 1);

	//Transformation Matrix 구하기
	Mat transformMat = getPerspectiveTransform(src, dst);

	//Warping
	Mat copySrcFrame, dstFrame;
	//srcFrame.copyTo(copySrcFrame);

	warpPerspective(srcFrame, dstFrame, transformMat, Size(maxWidth, maxHeight));

	return dstFrame;
}

Mat diffFrame(Mat background, Mat srcFrame) {
	Mat foreground, grayFrame;
	
	//1. Source Frame을 Gray Frame으로 변환
	//cvtColor(srcFrame, grayFrame, COLOR_BayerRG2GRAY);
	//GaussianBlur(srcFrame, srcFrame, Size(5, 5), 0.5);

	//2. 2개의 프레임의 차이의 절댓값 => foreground
	absdiff(background, srcFrame, foreground);

	//3. Erode, Dilate (미세한 잡음 제거 및 유사 영역 합침)
	erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(19, 19)));

	//dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));
	//erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));

	//4. foreground의 threshold (150 - 255)
	int thresholdValue = 100;
	int maxBinaryValue = 255;

	cvtColor(foreground, foreground, CV_BGR2GRAY);
	threshold(foreground, foreground, thresholdValue, maxBinaryValue, THRESH_BINARY);

	return foreground;
}

void drawParkingLotFun(Mat srcFrame, ParkingLotArea area) {
	Scalar lineColor(0, 0, 255);

	line(srcFrame, area.getTopLeft(), area.getBottomLeft(), lineColor, 1);
	line(srcFrame, area.getBottomLeft(), area.getBottomRight(), lineColor, 1);
	line(srcFrame, area.getBottomRight(), area.getTopRight(), lineColor, 1);
	line(srcFrame, area.getTopRight(), area.getTopLeft(), lineColor, 1);

	//주차장 주차 가능 확인
	circle(srcFrame, area.getParkingLotPoint(), 4, area.getDecideParkingLot(), 4);
}

void decideParkingLot(Mat srcFrame, ParkingLotArea* area, float maxExtent, float minExtent){

	//Detecting object 
	int level = 0;

	// Matrix 가로 세로
	int matCols = srcFrame.cols;
	int matRows = srcFrame.rows;

	//영역넓이
	int extent = 0;

	for (int y = 0; y < matRows; y++){
		for (int x = 0; x < matCols; x++){
			int binary = srcFrame.at<uchar>(y, x);
			
			//idx에 값이 존재할경우 영역 넓이 +1
			if(binary) extent++;
		}
	}
	cout << " AREA(MAX) :" << area->getArea()*maxExtent 
		 << "\tAREA(MIN) :" << area->getArea()*minExtent 
		 << "\tEXTENT : "<< extent << endl;
	//주차장 영역의  넘을 경우 => FULL
	if (extent >= area->getArea()*maxExtent)
		level = 0;
	else if (extent >= area->getArea()*minExtent)
		level = 1;
	else
		level = 2;
	
	area->setDecideParkingLot(level);
}
void fun(ParkingLotArea* area) {
	area->setDecideParkingLot(true);
}
void makeLabeling(Mat srcFrame, Mat foreground) {
	//1. make a labeling
	Mat labelsFrame, stats, centroids;
	int numOfLables = connectedComponentsWithStats(foreground, labelsFrame,
		stats, centroids, 8, CV_32S);

	//Detecting object 
	for (int i = 1; i < numOfLables; i++) {
		int area = stats.at<int>(i, CC_STAT_AREA);
		int left = stats.at<int>(i, CC_STAT_LEFT);
		int top = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);

		//object 중심 좌표
		double x = centroids.at<double>(i, 0); 
		double y = centroids.at<double>(i, 1);

		Scalar colorParkingLot = Scalar(255, 0, 0);

		circle(srcFrame, Point(x, y), 5, colorParkingLot, 1);

		//cout << area << endl;
		rectangle(srcFrame, Point(left, top), Point(left + width, top + height),
			Scalar(0, 0, 255), 1);

		putText(srcFrame, to_string(i), Point(left + 20, top + 20), FONT_HERSHEY_SIMPLEX,
			1, Scalar(255, 0, 0), 2);
	}
}

//DO NOT USE FUNCTION
Mat detectHaarcascadesCar(Mat srcFrame){

	//Copy source frame
	Mat dstFrame;
	srcFrame.copyTo(dstFrame);
	
	//Loading haarcascade xml file
	string cascadeName = "haarcascade_cars.xml";
	CascadeClassifier detector;

	if (!detector.load(cascadeName)){
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return srcFrame;
	}

	//Parameters
	int grThr = 4;
	double scaleStep = 1.1;
	Size minObjSize(24,24);
	Size maxObjSize(150, 200);

	vector<Rect> found;
	detector.detectMultiScale(srcFrame, found, scaleStep, grThr, 0, minObjSize, maxObjSize);

	// draw results (bounding boxes)
	for (int i = 0; i<(int)found.size(); i++)
		rectangle(dstFrame, found[i], Scalar(0, 255, 0), 2);

	return dstFrame;
}

void mouseClickFun(int event, int x, int y, int flags, void* userdata) {

	if (event == EVENT_LBUTTONDOWN) 
		cout << "좌표 = (" << x << ", " << y << ")" << endl;
}
#endif