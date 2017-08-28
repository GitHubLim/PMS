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

#define FILENAME "front_side_01.avi"
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

#define RESIZE 2

#define SCALEFIRST 9
#define SCALESECOND 19

#define SCALETHIRD 37

#define FULL 0
#define AMBIGUOUS 1
#define EMPTY 2

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

Mat warpingFun(Mat srcFrame, ParkingLotArea area) {

	Point topLeft, bottomLeft, bottomRight, topRight;
	
	topLeft = area.getTopLeft();
	bottomLeft = area.getBottomLeft();
	bottomRight = area.getBottomRight();
	topRight = area.getTopRight();

	//가장 큰 길이 width / height
	double width1 = sqrt(pow(bottomRight.x - bottomLeft.x, 2) + pow(bottomRight.y - bottomLeft.y, 2));
	double width2 = sqrt(pow(topRight.x - topLeft.x, 2) + pow(topRight.y - topLeft.y, 2));
	double height1 = sqrt(pow(topRight.x - bottomRight.x, 2) + pow(topRight.y - bottomRight.y, 2));
	double height2 = sqrt(pow(topLeft.x - bottomLeft.x, 2) + pow(topLeft.y - bottomLeft.y, 2));

	double maxWidth = (width1 > width2) ? width1 : width2;
	double maxHeight = (height1 > height2) ? height1 : height2;

	//warping 전,후 좌표(src, dst)
	Point2f src[4], dst[4];
	src[0] = Point2f(topLeft.x, topLeft.y);					//왼쪽 위
	src[1] = Point2f(topRight.x, topRight.y);				//오른쪽 위
	src[2] = Point2f(bottomRight.x, bottomRight.y);			//오른쪽 아래
	src[3] = Point2f(bottomLeft.x, bottomLeft.y);		    //왼쪽 아래

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

Mat diffFrameFun(Mat srcFrame, Mat background, bool isMorph) {
	Mat foreground, grayFrame;
	
	//1. Source Frame을 Gray Frame으로 변환
	//cvtColor(srcFrame, grayFrame, COLOR_BayerRG2GRAY);
	//GaussianBlur(srcFrame, srcFrame, Size(5, 5), 0.5);

	//2. 2개의 프레임의 차이의 절댓값 => foreground
	absdiff(background, srcFrame, foreground);
	
	//3. Erode, Dilate (미세한 잡음 제거 및 유사 영역 합침)
	if (isMorph) {
		erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(19, 19)));
	}
	//dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));
	//erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));

	//4. foreground의 threshold (150 - 255)
	int thresholdValue = 100;
	int maxBinaryValue = 255;

	cvtColor(foreground, foreground, CV_BGR2GRAY);
	threshold(foreground, foreground, thresholdValue, maxBinaryValue, THRESH_BINARY);
	
	////MOG2 approach
	//Ptr<BackgroundSubtractor> pMOG2;
	//pMOG2 = createBackgroundSubtractorMOG2(500, 250, true); 

	//pMOG2->apply(srcFrame, foreground);

	return foreground;
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

void pushParkingLotPoint(vector<ParkingLotArea>* vecArea, int ID, Point topLeft, Point bottomLeft, Point bottomRight, Point topRight){
	ParkingLotArea area;
	area.setID(ID);																		//ID0;
	area.setParkingLot(topLeft, bottomLeft, bottomRight, topRight);
	vecArea->push_back(area);
}

void setParkingLotPoint(vector<ParkingLotArea>* vecArea) {
	ParkingLotArea area;
	int ID = 0;
	pushParkingLotPoint(vecArea, ID++, Point(126, 187), Point(97, 238), Point(136, 243), Point(160, 190));	//ID00;
	pushParkingLotPoint(vecArea, ID++, Point(160, 190), Point(136, 243), Point(176, 246), Point(194, 190));	//ID01;
	pushParkingLotPoint(vecArea, ID++, Point(194, 190), Point(176, 246), Point(220, 248), Point(236, 191));	//ID02;
	pushParkingLotPoint(vecArea, ID++, Point(236, 191), Point(220, 248), Point(269, 252), Point(279, 194));	//ID03;
	pushParkingLotPoint(vecArea, ID++, Point(279, 194), Point(269, 252), Point(321, 251), Point(323, 195));	//ID04;
	pushParkingLotPoint(vecArea, ID++, Point(323, 195), Point(321, 251), Point(368, 253), Point(364, 195));	//ID05;
	pushParkingLotPoint(vecArea, ID++, Point(364, 195), Point(368, 253), Point(417, 253), Point(409, 195));	//ID06;
	pushParkingLotPoint(vecArea, ID++, Point(409, 195), Point(417, 253), Point(464, 253), Point(450, 195));	//ID07;
	pushParkingLotPoint(vecArea, ID++, Point(450, 195), Point(464, 253), Point(507, 252), Point(488, 195));	//ID08;
	pushParkingLotPoint(vecArea, ID++, Point(488, 195), Point(507, 252), Point(547, 248), Point(530, 194));	//ID09;
	pushParkingLotPoint(vecArea, ID++, Point(530, 194), Point(547, 248), Point(583, 246), Point(562, 193));	//ID10;
	pushParkingLotPoint(vecArea, ID++, Point(562, 193), Point(583, 246), Point(611, 245), Point(596, 192));	//ID11;
}

void decideParkingLotPoint(Mat mask, ParkingLotArea* area, float maxExtent, float minExtent) {

	//Detecting object 
	int level = 0;

	// Matrix 가로 세로
	int matCols = mask.cols;
	int matRows = mask.rows;

	//영역넓이
	int extent = 0;

	for (int y = 0; y < matRows; y++) {
		for (int x = 0; x < matCols; x++) {
			int binary = mask.at<uchar>(y, x);

			//idx에 값이 존재할경우 영역 넓이 +1
			if (binary) extent++;
		}
	}

	cout << " AREA(MAX) :" << area->getArea()*maxExtent
		<< "\tAREA(MIN) :" << area->getArea()*minExtent
		<< "\tEXTENT : " << extent << endl;

	//주차장 영역의  넘을 경우 => FULL
	if (extent >= area->getArea()*maxExtent)
		level = FULL;
	else if (extent >= area->getArea()*minExtent)
		level = AMBIGUOUS;
	else
		level = EMPTY;

	area->setDecideParkingLot(level);
}
void drawParkingLotFun(Mat srcFrame, vector<ParkingLotArea>* vecArea) {
	Scalar lineColor(0, 0, 255);
	vector<ParkingLotArea>::iterator iter;

	//vector에 있는 값을 전부 추출
	for (iter = vecArea->begin(); iter != vecArea->end(); ++iter) {
		ParkingLotArea area;
		area = *iter;

		line(srcFrame, area.getTopLeft(), area.getBottomLeft(), lineColor, 1);
		line(srcFrame, area.getBottomLeft(), area.getBottomRight(), lineColor, 1);
		line(srcFrame, area.getBottomRight(), area.getTopRight(), lineColor, 1);
		line(srcFrame, area.getTopRight(), area.getTopLeft(), lineColor, 1);

		//주차장 주차 가능 확인
		circle(srcFrame, area.getParkingLotPoint(), 4, area.getDecideParkingLot(), 4);
	}
}

void showFPS(Mat srcFrame, int64 freq, int64 start, int64 finish) {
	double fps = freq / double(finish - start + 1);
	char fpsStr[20];
	sprintf_s(fpsStr, 20, "FPS: %.1lf", fps);
	putText(srcFrame, fpsStr, Point(400, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0, 255, 0), 2);
}
void mouseClickFun(int event, int x, int y, int flags, void* userdata) {

	if (event == EVENT_LBUTTONDOWN) 
		cout << "좌표 = (" << x << ", " << y << ")" << endl;
}

#endif