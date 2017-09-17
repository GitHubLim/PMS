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

#define FRONTFILENAME "front_side_01.avi"
#define BACKFILENAME "back_side_01.avi"
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

#define MAXEXTENT 0.25
#define MINEXTENT 0.16

#define BACKSIDE 10
#define FRONTSIDE 20
#define NOSIDE 30

using namespace std;
using namespace cv;

float calculateExtent(Mat srcFrame, int startX, int endX, int startY, int endY);
float rearrangeExtent(Mat srcFrame);

typedef struct HoughLineInfo {
	vector<unsigned int> iAngle;
	vector<double> dRho;
}HoughLineInfo;

//Mat houghLine(Mat srcFrame) {
//
//	//Exception handling (Image is Empty)
//	if (srcFrame.empty() == true) {
//		cout << "srcFrame is Empty" << endl;
//	}
//
//	Mat dstFrame;
//	srcFrame.copyTo(dstFrame);
//	// 1.  Edge detection, e.g. using the Canny edge detector.
//	Mat edgeFrame;
//	GaussianBlur(srcFrame, srcFrame, Size(9, 9), 1.0);
//	Canny(srcFrame, edgeFrame, 50, 100);
//
//
//	//2. Mapping of edge points to the Hough space and storage in an accumulator
//
//	const unsigned int kAngleSize = 180;
//	const unsigned int kDistMax = sqrt((edgeFrame.cols*edgeFrame.cols) + (edgeFrame.rows*edgeFrame.rows)) + 1;
//	const unsigned int kDistanceSize = kDistMax * 2;
//	const unsigned int kThresHoldForLine = 100;
//
//	unsigned int* pNvote = new unsigned int[kAngleSize*kDistanceSize];
//	memset(pNvote, 0, (kAngleSize*kDistanceSize) * sizeof(unsigned int));
//
//	double lukCos[180];
//	double lukSin[180];
//	unsigned int lukAngleIdx[180];
//
//	for (int i = 0; i < kAngleSize; i++)
//	{
//		double angle = (double)i*DEG2RAD;
//
//		lukCos[i] = cos(angle);
//		lukSin[i] = sin(angle);
//		lukAngleIdx[i] = i*kDistanceSize;
//	}
//
//	unsigned char* ucMatEdgeData = edgeFrame.data;
//	for (int y = 0; y < edgeFrame.rows; y++)
//	{
//		for (int x = 0; x < edgeFrame.cols; x++)
//		{
//			if (*ucMatEdgeData++ == 0)continue;
//
//			for (unsigned int j = 0; j < kAngleSize; j++)
//			{
//				double rho = lukCos[j] * x + lukSin[j] * y;
//				rho += (double)kDistMax;
//
//				pNvote[lukAngleIdx[j] + (int)(rho + 0.5)]++;
//			}
//		}
//	}
//
//	//3. Interpretation of the accumulator to yield lines of infinite length. 
//	//The interpretation is done by thresholding and possibly other constraints.
//
//	Point ptA;
//	Point ptB;
//
//	HoughLineInfo lineVec;
//
//	vector<Vec2f> linesVec;
//
//	for (int i = 0; i < 180; i++)
//		for (int j = 0; j < kDistanceSize; j++)
//		{
//			int nVote = pNvote[lukAngleIdx[i] + j];
//			if (nVote >= kThresHoldForLine)
//			{
//				bool isTrueLine = true;
//
//				for (int dAngle = -1; dAngle <= 1 && isTrueLine; dAngle++)
//				{
//					if (i + dAngle < 0)continue;
//					if (i + dAngle >= kAngleSize)break;
//
//					for (int dRho = -1; dRho <= 1 && isTrueLine; dRho++)
//					{
//						if (j + dRho < 0)continue;
//						if (j + dRho >= kDistanceSize)break;
//
//						if (pNvote[lukAngleIdx[i + dAngle] + (j + dRho)] > nVote)isTrueLine = false;
//					}
//				}
//
//				if (isTrueLine == false) continue;
//
//				lineVec.iAngle.push_back(i);
//				lineVec.dRho.push_back(j - (int)kDistMax);
//			}
//		}
//
//	int nLineVecSize = lineVec.dRho.size();
//	for (int i = 0; i < nLineVecSize; i++)
//	{
//		int angle = lineVec.iAngle[i];
//
//		double cX = lukCos[angle];
//		double cY = lukSin[angle];
//
//		double rho = lineVec.dRho[i];
//
//		double x0 = cX*rho;
//		double y0 = cY*rho;
//
//		ptA.x = cvRound(x0 + 1000. * (-cY));
//		ptA.y = cvRound(y0 + 1000. * (cX));
//		ptB.x = cvRound(x0 - 1000. * (-cY));
//		ptB.y = cvRound(y0 - 1000. * (cX));
//
//		line(dstFrame, ptA, ptB, Scalar(0, 255, 0), 1);
//	}
//
//	delete[] pNvote;
//
//	return dstFrame;
//}

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

Mat maskingFun(Mat srcFrame, Mat mask, bool isExpand){
	Mat dstFrame;

	if (isExpand) {
		threshold(mask, mask, 15, 255, THRESH_BINARY);
		dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(11, 11)));
	}
	//1. masking 
	srcFrame.copyTo(dstFrame, mask);

	return dstFrame;
}

Mat warpingFun(Mat srcFrame, ParkingLotArea area, int side) {
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

	switch (side) {
	case FRONTSIDE:
		src[0] = Point(126, 187);			//왼쪽 위
		src[1] = Point(160, 190);			//오른쪽 위
		src[2] = Point(136, 243);			//오른쪽 아래
		src[3] = Point(97, 238);		    //왼쪽 아래
		break;

	case BACKSIDE:
		src[0] = Point(266, 230);			//왼쪽 위
		src[1] = Point(219, 271);			//오른쪽 위
		src[2] = Point(325, 275);			//오른쪽 아래
		src[3] = Point(323, 237);		    //왼쪽 아래
		break;

	case NOSIDE:
		src[0] = Point(topLeft.x, topLeft.y);				//왼쪽 위
		src[1] = Point(topRight.x, topRight.y);				//오른쪽 위
		src[2] = Point(bottomRight.x, bottomRight.y);		//오른쪽 아래
		src[3] = Point(bottomLeft.x, bottomLeft.y);		    //왼쪽 아래
		break;

	default:
		cerr << "ERROR: ROI point is not finding " << endl;
	}

	dst[0] = Point2d(0, 0);
	dst[1] = Point2d(maxWidth - 1, 0);
	dst[2] = Point2d(maxWidth - 1, maxHeight - 1);
	dst[3] = Point2d(0, maxHeight - 1);

	//Transformation Matrix 구하기
	Mat transformMat = getPerspectiveTransform(src, dst);

	//Warping
	Mat copySrcFrame, dstFrame;
	//srcFrame.copyTo(copySrcFrame);

	warpPerspective(srcFrame, dstFrame, transformMat, Size2d(maxWidth, maxHeight));

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
		erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(4, 4))); //고정 
		dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(11, 11)));
	}
	//dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));
	//erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));

	//4. foreground의 threshold (150 - 255)
	int thresholdValue = 50;
	int maxBinaryValue = 255;

	cvtColor(foreground, foreground, CV_BGR2GRAY);
	threshold(foreground, foreground, thresholdValue, maxBinaryValue, THRESH_BINARY);
	
	return foreground;
}

Mat diffFrameFun2(Mat srcFrame, Ptr<BackgroundSubtractor> pMOG2) {
	//Foreground 추출

	Mat foreground; 
	pMOG2->apply(srcFrame, foreground); 

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
		int x = centroids.at<int>(i, 0); 
		int y = centroids.at<int>(i, 1);

		circle(srcFrame, Point(x, y), 5, Scalar(255, 0, 0), 1);

		//cout << area << endl;
		rectangle(srcFrame, Point(left, top), Point(left + width, top + height),
			Scalar(0, 0, 255), 1);

		putText(srcFrame, to_string(i), Point(left + 20, top + 20), FONT_HERSHEY_SIMPLEX,
			1, Scalar(255, 0, 0), 2);
	}
}

Mat calibration(Mat srcFrame) {
	Mat dstFrame;

	double dCameraMatrix[] = { 1453.613, 0., 952.558, 0., 1460.921, 373.498, 0., 0., 1. };
	double dDistCoeffs[] = { -0.519183, 0.335450, -0.003719, -0.003036 };

	Mat cameraMatrix = Mat(3, 3, CV_64FC1, (void*)dCameraMatrix);   // 사용된 카메라 
	Mat distCoeffs = Mat(1, 4, CV_64FC1, (void*)dDistCoeffs);      // 왜곡계수 

	undistort(srcFrame, dstFrame, cameraMatrix, distCoeffs);
	resize(dstFrame, dstFrame, Size(srcFrame.cols / RESIZE, srcFrame.rows / RESIZE), 0, 0, CV_INTER_NN);   //Resize Frame

	return dstFrame;
}

void pushParkingLotPoint(vector<ParkingLotArea>* vecArea, int ID, Mat background, Point topLeft, Point bottomLeft, Point bottomRight, Point topRight, int side) {
	ParkingLotArea area;
	area.setID(ID);
	area.setParkingLot(topLeft, bottomLeft, bottomRight, topRight);

	Mat backWarpFrame = warpingFun(background, area, side);
	area.setBackground(backWarpFrame);

	vecArea->push_back(area);
}

void updateBackground(Mat srcFrame, vector<ParkingLotArea>* vecArea){
	vector<ParkingLotArea>::iterator iter; 
	
	for (iter = vecArea->begin(); iter < vecArea->end(); ++iter){
		int level = iter->getLevel();

		//주차장이 비어있을때만 background 업데이트 
		if (level == EMPTY) {
			Mat newBackground = warpingFun(srcFrame, *iter, NOSIDE);

			//Update 가중치 0.99*Origin + 0.01*New = new
			addWeighted(iter->getBackground(), 0.99, newBackground, 0.01, 0, newBackground);

			iter->setBackground(newBackground); // Update Background
		}
	}
}

void setParkingLotPoint(vector<ParkingLotArea>* vecArea, Mat background, int side) {

	int ID = 0;	//ID 0부터 시작 

	if (side == FRONTSIDE) {
		//-------------------------------------------------- FRONT ---------------------------------------------------------------
		pushParkingLotPoint(vecArea, ID++, background, Point(126, 187), Point(97, 238), Point(136, 243), Point(160, 190), side);	//ID00;
		pushParkingLotPoint(vecArea, ID++, background, Point(160, 190), Point(136, 243), Point(176, 246), Point(194, 190), side);	//ID01;
		pushParkingLotPoint(vecArea, ID++, background, Point(194, 190), Point(176, 246), Point(220, 248), Point(236, 191), side);	//ID02;
		pushParkingLotPoint(vecArea, ID++, background, Point(236, 191), Point(220, 248), Point(269, 252), Point(279, 194), side);	//ID03;
		pushParkingLotPoint(vecArea, ID++, background, Point(279, 194), Point(269, 252), Point(319, 252), Point(323, 195), side);	//ID04;
		pushParkingLotPoint(vecArea, ID++, background, Point(323, 195), Point(319, 252), Point(368, 253), Point(364, 195), side);	//ID05;
		pushParkingLotPoint(vecArea, ID++, background, Point(364, 195), Point(368, 253), Point(417, 253), Point(409, 195), side);	//ID06;
		pushParkingLotPoint(vecArea, ID++, background, Point(409, 195), Point(417, 253), Point(464, 253), Point(450, 195), side);	//ID07;
		pushParkingLotPoint(vecArea, ID++, background, Point(450, 195), Point(464, 253), Point(507, 252), Point(488, 195), side);	//ID08;
		pushParkingLotPoint(vecArea, ID++, background, Point(488, 195), Point(507, 252), Point(547, 248), Point(530, 194), side);	//ID09;
		pushParkingLotPoint(vecArea, ID++, background, Point(530, 194), Point(547, 248), Point(583, 246), Point(562, 193), side);	//ID10;
		pushParkingLotPoint(vecArea, ID++, background, Point(562, 193), Point(583, 246), Point(611, 245), Point(596, 192), side);	//ID11;
		pushParkingLotPoint(vecArea, ID++, background, Point(94, 184), Point(151, 187), Point(161, 160), Point(106, 158), side);	//ID12;
		pushParkingLotPoint(vecArea, ID++, background, Point(106, 158), Point(161, 160), Point(163, 134), Point(123, 135), side);	//ID13;
		//-------------------------------------------------------------------------------------------------------------------------
	}
	else if (side == BACKSIDE) {
		//-------------------------------------------------- BACK -----------------------------------------------------------------
		pushParkingLotPoint(vecArea, ID++, background, Point(219, 271), Point(325, 275), Point(325, 234), Point(226, 230), side);	//ID00;
		pushParkingLotPoint(vecArea, ID++, background, Point(226, 230), Point(325, 234), Point(325, 197), Point(231, 195), side);	//ID01;
		pushParkingLotPoint(vecArea, ID++, background, Point(231, 195), Point(325, 197), Point(327, 161), Point(235, 161), side);	//ID02;
		pushParkingLotPoint(vecArea, ID++, background, Point(235, 161), Point(327, 161), Point(326, 131), Point(241, 131), side);	//ID03;
		pushParkingLotPoint(vecArea, ID++, background, Point(241, 131), Point(326, 131), Point(326, 108), Point(247, 106), side);	//ID04;
		pushParkingLotPoint(vecArea, ID++, background, Point(247, 106), Point(326, 108), Point(325, 82), Point(250, 81), side);	//ID05;
		pushParkingLotPoint(vecArea, ID++, background, Point(250, 81), Point(325, 82), Point(325, 63), Point(254, 61), side);		//ID06;
		pushParkingLotPoint(vecArea, ID++, background, Point(254, 61), Point(325, 63), Point(325, 44), Point(257, 44), side);		//ID07;
		pushParkingLotPoint(vecArea, ID++, background, Point(257, 44), Point(325, 44), Point(328, 27), Point(261, 27), side);		//ID08;
		pushParkingLotPoint(vecArea, ID++, background, Point(261, 27), Point(328, 27), Point(325, 9), Point(266, 9), side);		//ID09;

		ID = 12;
		pushParkingLotPoint(vecArea, ID++, background, Point(206, 319), Point(218, 232), Point(171, 229), Point(156, 314), side);      //ID12;
		pushParkingLotPoint(vecArea, ID++, background, Point(156, 314), Point(171, 229), Point(128, 227), Point(110, 307), side);      //ID13;
		pushParkingLotPoint(vecArea, ID++, background, Point(110, 307), Point(128, 227), Point(92, 223), Point(71, 299), side);       //ID14
		pushParkingLotPoint(vecArea, ID++, background, Point(71, 299), Point(92, 223), Point(57, 219), Point(37, 293), side);          //ID15;
		pushParkingLotPoint(vecArea, ID++, background, Point(37, 293), Point(57, 219), Point(25, 216), Point(5, 285), side);          //ID16;


		pushParkingLotPoint(vecArea, ID++, background, Point(489, 273), Point(565, 265), Point(553, 225), Point(481, 231), side);		//ID17;
		pushParkingLotPoint(vecArea, ID++, background, Point(481, 231), Point(553, 225), Point(548, 194), Point(472, 197), side);		//ID18;
		pushParkingLotPoint(vecArea, ID++, background, Point(472, 197), Point(548, 194), Point(539, 166), Point(465, 165), side);		//ID19;
		pushParkingLotPoint(vecArea, ID++, background, Point(465, 165), Point(539, 166), Point(528, 140), Point(461, 138), side);		//ID20;
		pushParkingLotPoint(vecArea, ID++, background, Point(461, 138), Point(528, 140), Point(519, 118), Point(454, 114), side);		//ID21;
		pushParkingLotPoint(vecArea, ID++, background, Point(454, 114), Point(519, 118), Point(511, 93), Point(448, 90), side);			//ID22;
		pushParkingLotPoint(vecArea, ID++, background, Point(448, 90), Point(511, 93), Point(504, 73), Point(441, 70), side);			//ID23;
		pushParkingLotPoint(vecArea, ID++, background, Point(441, 70), Point(504, 73), Point(495, 53), Point(436, 50), side);			//ID24;
		pushParkingLotPoint(vecArea, ID++, background, Point(436, 50), Point(495, 53), Point(489, 39), Point(432, 34), side);			//ID25;
		pushParkingLotPoint(vecArea, ID++, background, Point(432, 34), Point(489, 39), Point(483, 23), Point(428, 20), side);			//ID26;
		pushParkingLotPoint(vecArea, ID++, background, Point(428, 20), Point(483, 23), Point(475, 7), Point(424, 1), side);				//ID27;



		
		

	}
}

//REVISE(SPEED IS TOO LOW)
void prioritizeParkingLotLevel(vector<ParkingLotArea>* vec_area1, vector<ParkingLotArea>* vec_area2){
	vector<ParkingLotArea>::iterator iter_area1, iter_area2;	//Iterator

	for (iter_area1 = vec_area1->begin(); iter_area1 != vec_area1->end(); ++iter_area1) {
		int area1_ID = iter_area1->getID();
		double area1_degree = iter_area1->getDegree();

		for (iter_area2 = vec_area2->begin(); iter_area2 != vec_area2->end(); ++iter_area2) {
			int area2_ID = iter_area2->getID();

			if (area1_ID == area2_ID) {
				double area2_degree = iter_area2->getDegree();
				int level = iter_area2->getLevel();	// Level
				
				//각도에 따른 우선순위 정하기 
				if (area1_degree < area2_degree) {
					level = iter_area1->getLevel();
					iter_area2->setLevel(level);
				}else 
					iter_area1->setLevel(level);

				break;
			}

		}
	}
}

void decideParkingLotPoint(Mat srcFrame, Mat background, vector<ParkingLotArea>* vecArea){
	//주차 공간 추출
	vector<ParkingLotArea>::iterator iter;
	
	//vector에 있는 값을 전부 추출
	for (iter = vecArea->begin(); iter != vecArea->end(); ++iter) {
		
		Mat warpFrame, backWarpFrame, foreWarpFrame;

		warpFrame = warpingFun(srcFrame, *iter, NOSIDE);								//FRAME
		foreWarpFrame = diffFrameFun(warpFrame, iter->getBackground(), true);	//FORGROUND

		//Detecting object 
		int level = 0;
		float extent = 0;

		// Matrix 가로 세로
		int matCols = foreWarpFrame.cols;
		int matRows = foreWarpFrame.rows;

		extent = calculateExtent(foreWarpFrame, 0 , matCols, 0, matRows);	//Foreground 영역구하기
		
	
		//주차장 영역의 넘을 경우 => FULL
		if (extent <= iter->getArea()*MINEXTENT) {
			level = EMPTY;

			if (extent) { //Extent가 0이 아닐경우에만 
				// 주차공간 업데이트
				Mat newBackground = warpingFun(srcFrame, *iter, NOSIDE);

				//Update 가중치 0.99*Origin + 0.01*New = new
				addWeighted(iter->getBackground(), 0.99, newBackground, 0.01, 0, newBackground);
				iter->setBackground(newBackground); // Update Background
			}

		}else if (iter->getDegree() < 10) {	 // 기울기가 20도 미만일 경우 Penalty X

			if (extent <= iter->getArea()*MAXEXTENT)
				level = AMBIGUOUS;
			else
				level = FULL;

		}else {								// 기울기가 20도 이상일 경우 Penalty 부여
			extent = rearrangeExtent(foreWarpFrame);

			if (extent <= iter->getArea()*MINEXTENT) 
				level = EMPTY;
			else if (extent <= iter->getArea()*MAXEXTENT)
				level = AMBIGUOUS;
			else
				level = FULL;
		}

		cout << "AREA(ID) :   " << iter->getID()
			<< "\tAREA :  " << iter->getArea()
			<< "\tAREA(MAX) :  " << iter->getArea()*MAXEXTENT
			<< "\tAREA(MIN) :  " << iter->getArea()*MINEXTENT
			<< "\tEXTENT :   " << extent
			<< "\tANGLE :  " << iter->getDegree() << endl;

		iter->setLevel(level); //Level 저장
	}
}

float calculateExtent(Mat srcFrame, int startCol, int endCol, int startRow, int endRow) {

	float extent = 0; //영역넓이

	for (int y = startRow; y < endRow; y++) {
		for (int x = startCol; x < endCol; x++) {
			int binary = srcFrame.at<uchar>(y, x);

			if (binary) extent++; //idx에 값이 존재할경우 영역 넓이 +1
		}
	}

	return extent;
}

float rearrangeExtent(Mat srcFrame) {
	//영역넓이
	float extent = 0;

	// Matrix 가로 세로
	int matCols = srcFrame.cols;
	int matRows = srcFrame.rows;

	float frontExtent, middleExtent, backExtent;
	
	float scale = 3;

	if (matCols > matRows) {	// 세로의 길이가 더 길경우 
		frontExtent = calculateExtent(srcFrame, 0, matCols, 0, (int)(matRows / scale));
		middleExtent = calculateExtent(srcFrame, 0, matCols, (int)(matRows / scale), (int)(matRows * (scale - 1) / scale));
		backExtent = calculateExtent(srcFrame, 0, matCols, (int)(matRows * (scale - 1) / scale), matRows);
		
		extent = (float)((frontExtent * 0.1) + (middleExtent * 0.3) + (backExtent * 0.6));	//FRONT, BACK 비율 0.9 Middle 비율 0.1

	}else {						// 가로의 길이가 더 길경우 

		frontExtent = calculateExtent(srcFrame, 0, (int)(matCols / scale), 0, matRows);
		middleExtent = calculateExtent(srcFrame, (int)(matCols / scale), (int)(matCols*(scale - 1) / scale), 0, matRows);
		backExtent = calculateExtent(srcFrame, (int)(matCols*(scale - 1) / scale), matCols, 0, matRows);

		extent = (float)((frontExtent * 0.1) + (middleExtent * 0.3) + (backExtent * 2.6));	//FRONT, BACK 비율 0.9 Middle 비율 0.1

	}

	return extent;
}

void drawParkingLotPoint(Mat srcFrame, vector<ParkingLotArea>* vecArea) {
	Scalar lineColor(0, 0, 255);
	vector<ParkingLotArea>::iterator iter;

	//vector에 있는 값을 전부 추출
	for (iter = vecArea->begin(); iter != vecArea->end(); ++iter) {
		ParkingLotArea area;
		area = *iter;

		Point center;		// 중점 ID값 표시
		center.x = ((area.getTopLeft().x + area.getBottomRight().x) / 2) - 5;
		center.y = ((area.getTopLeft().y + area.getBottomRight().y) / 2) + 5;

		line(srcFrame, area.getTopLeft(), area.getBottomLeft(), lineColor, 1);
		line(srcFrame, area.getBottomLeft(), area.getBottomRight(), lineColor, 1);
		line(srcFrame, area.getBottomRight(), area.getTopRight(), lineColor, 1);
		line(srcFrame, area.getTopRight(), area.getTopLeft(), lineColor, 1);

		/*line(srcFrame, area.getPTopLeft(), area.getPBottomLeft(), lineColor, 1);
		line(srcFrame, area.getPBottomLeft(), area.getPBottomRight(), lineColor, 1);
		line(srcFrame, area.getPBottomRight(), area.getPTopRight(), lineColor, 1);
		line(srcFrame, area.getPTopRight(), area.getPTopLeft(), lineColor, 1);
*/
		//주차장 ID값 표시
		char IDStr[20];
		sprintf_s(IDStr, 20, "%d", area.getID());
		putText(srcFrame, IDStr, center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);

		//주차장 주차 가능 확인
		circle(srcFrame, area.getParkingLotPoint(), 4, area.getDecideParkingLot(), 4);
	}
}

void showFPSFun(Mat srcFrame, int64 freq, int64 start, int64 finish) {
	double fps = freq / double(finish - start + 1);
	char fpsStr[20];
	sprintf_s(fpsStr, 20, "FPS: %.1lf", fps);
	putText(srcFrame, fpsStr, Point(400, 320), FONT_HERSHEY_SIMPLEX, 1., Scalar(0, 255, 0), 2);
}
                                                                 
void mouseClickFun(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) 
		cout << "좌표 = (" << x << ", " << y << ")" << endl;
}

//DO NOT USE FUNCTION
void detectHaarcascadesCar(Mat srcFrame) {

	//Loading haarcascade xml file
	string cascadeName = "cas1.xml";
	CascadeClassifier detector;

	if (!detector.load(cascadeName)) 
		cerr << "ERROR: Could not load classifier cascade" << endl;

	//Parameters
	int grThr = 2;
	double scaleStep = 1.1;
	Size minObjSize(40, 40);
	Size maxObjSize(200, 200);

	vector<Rect> found;
	detector.detectMultiScale(srcFrame, found, scaleStep, grThr, 0, minObjSize, maxObjSize);

	// draw results (bounding boxes)
	for (int i = 0; i<(int)found.size(); i++)
		rectangle(srcFrame, found[i], Scalar(0, 255, 0), 2);
}

#endif