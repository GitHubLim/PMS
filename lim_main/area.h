#pragma once
#ifndef PMS_AREA_HEADER
#define PMS_AREA_HEADER

#include <iostream>
#include <opencv2/opencv.hpp>

#define	EMPTY 0
#define AMBIGUOUS 1
#define FULL 2

using namespace std;
using namespace cv;

class ParkingLotArea {

private:
	//���� ���� ��ġ
	Point topLeft;
	Point bottomLeft;
	Point bottomRight;
	Point topRight;

	////Penalty ��ġ
	//Point pTopLeft;
	//Point pBottomLeft;
	//Point pBottomRight;
	//Point pTopRight;

	//�� Matrix
	Mat background;

	int ID; //�������� ID
	int area;
	double degree;

	int level = 0; //���� ���� �ܰ躰 �Ǵ�
	Point parkingLotPoint;
	Scalar decideParkingLot; //���� ���� ��

public:
	ParkingLotArea();
	~ParkingLotArea();

	void setParkingLot(Point topLeft, Point bottomLeft, Point bottomRight, Point topRight);
	void setID(int id);
	void setLevel(int level);
	void setBackground(Mat background);

	Point getTopLeft();
	Point getBottomLeft();
	Point getTopRight();
	Point getBottomRight();

	/*Point getPTopLeft();
	Point getPBottomLeft();
	Point getPTopRight();
	Point getPBottomRight();*/

	Point getParkingLotPoint();
	int getLevel();
	double getDegree();

	Mat getBackground();

	int getID();
	int getArea();
	Scalar getDecideParkingLot();
};

ParkingLotArea::ParkingLotArea() {
	setLevel(this->level);
}

ParkingLotArea::~ParkingLotArea() {
}


double ParkingLotArea::getDegree() {
	return this->degree;
}

void ParkingLotArea::setID(int id) {
	this->ID = id;
}

int ParkingLotArea::getID() {
	return this->ID;
}

void ParkingLotArea::setBackground(Mat background) {
	this->background = background;
}

Mat ParkingLotArea::getBackground() {
	return this->background;
}

int ParkingLotArea::getLevel() {
	return this->level;
}

void ParkingLotArea::setParkingLot(Point topLeft, Point bottomLeft, Point bottomRight, Point topRight) {

	//Parking lot ��ǥ �Է�
	// TopLeft = ������ ���� �亯�� ������ ��, BottomLeft = TopLeft �Ʒ�
	// TopRight =  ������ ���� �亯�� ���� ��, BottomRight = TopRight �Ʒ�

	this->topLeft = topLeft;
	this->bottomLeft = bottomLeft;
	this->topRight = topRight;
	this->bottomRight = bottomRight;

	//Parking lot ���� Ȯ�� ��ǥ 
	parkingLotPoint.x = (bottomLeft.x + bottomRight.x) / 2;
	parkingLotPoint.y = (bottomLeft.y + bottomRight.y) / 2;

	////Penalty Point ���� ��ǥ
	//pTopLeft.x = topLeft.x + (bottomLeft.x - topLeft.x) / 3;
	//pTopLeft.y = topLeft.y + (bottomLeft.y - topLeft.y) / 3;

	//pBottomLeft.x = bottomLeft.x - (bottomLeft.x - topLeft.x) / 3;
	//pBottomLeft.y = bottomLeft.y - (bottomLeft.y - topLeft.y) / 3;

	//pBottomRight.x = bottomRight.x - (bottomRight.x - topRight.x) / 3;
	//pBottomRight.y = bottomRight.y - (bottomRight.y - topRight.y) / 3;

	//pTopRight.x = topRight.x + (bottomRight.x - topRight.x) / 3;
	//pTopRight.y = topRight.y + (bottomRight.y - topRight.y) / 3;

	//Parking lot ���� ����
	double widthTop;
	double widthBottom;
	double heightLeft;
	double heightRight;
	double crossLine;

	widthTop = sqrt(pow(topLeft.x - topRight.x, 2.0) + pow(topLeft.y - topRight.y, 2.0));
	widthBottom = sqrt(pow(bottomLeft.x - bottomRight.x, 2.0) + pow(bottomLeft.y - bottomRight.y, 2.0));
	heightLeft = sqrt(pow(topLeft.x - bottomLeft.x, 2.0) + pow(topLeft.y - bottomLeft.y, 2.0));
	heightRight = sqrt(pow(bottomRight.x - topRight.x, 2.0) + pow(bottomRight.y - topRight.y, 2.0));

	crossLine = sqrt(pow(topLeft.x - bottomRight.x, 2.0) + pow(topLeft.y - bottomRight.y, 2.0));

	//�ﰢ�� ���� S1, S2(����� ����) = �簢�� ���� = S1+S2
	double s0, s1, s2;

	s0 = (widthTop + heightLeft + crossLine) / 2;
	s1 = sqrt(s0*(s0 - widthTop)*(s0 - heightLeft)*(s0 - crossLine));

	s0 = (widthBottom + heightRight + crossLine) / 2;
	s2 = sqrt(s0*(s0 - widthBottom)*(s0 - heightRight)*(s0 - crossLine));

	this->area = (int)(s1 + s2);

	//���� �� ���� ����
	double maxWidth = (widthTop > widthBottom) ? widthTop : widthBottom;
	double maxHeight = (heightLeft > heightRight) ? heightLeft : heightRight;
	double maxLength = (maxWidth > maxHeight) ? maxWidth : maxHeight;

	double dx, dy;	//x, y��ȭ��
	double rad, degree;

	if(maxLength == maxWidth){ // ���ΰ� �� ��� 
		bool isHigh;
		isHigh = (widthTop > widthBottom) ? 0 : 1;//

		if (!isHigh){
			dx = abs(topLeft.x - topRight.x);
			dy = abs(topLeft.y - topRight.y);
		}else {
			dx = abs(bottomLeft.x - bottomRight.x);
			dy = abs(bottomLeft.y - bottomRight.y);
		}

	}else { // ���ΰ� �� ���
		bool isHigh;
		isHigh = (heightLeft > heightRight) ? 0 : 1;

		if (!isHigh){
			dx = abs(topLeft.x - bottomLeft.x);
			dy = abs(topLeft.y - bottomLeft.y);
		}else {
			dx = abs(topRight.x - bottomRight.x);
			dy = abs(topRight.y - bottomRight.y);
		}
	}

	rad = atan2(dx, dy);
	degree = abs(rad * 180) / CV_PI;

	if (degree > 90) // ȸ���� ����
		degree -= 90;
	
	this->degree = degree;
}

Point ParkingLotArea::getTopLeft() {
	return this->topLeft;
}

Point ParkingLotArea::getBottomLeft() {
	return this->bottomLeft;
}

Point ParkingLotArea::getTopRight() {
	return this->topRight;
}

Point ParkingLotArea::getBottomRight() {
	return this->bottomRight;
}

//Point ParkingLotArea::getPTopLeft() {
//	return this->pTopLeft;
//}
//
//Point ParkingLotArea::getPBottomLeft() {
//	return this->pBottomLeft;
//}
//
//Point ParkingLotArea::getPTopRight() {
//	return this->pTopRight;
//}
//
//Point ParkingLotArea::getPBottomRight() {
//	return this->pBottomRight;
//}

Point ParkingLotArea::getParkingLotPoint() {
	return this->parkingLotPoint;
}

int ParkingLotArea::getArea() {
	return this->area;
}

void ParkingLotArea::setLevel(int level) {
	this->level = level;

	switch (this->level) {
		case EMPTY: 		//GREEN(������ �� �� ����)
			this->decideParkingLot[0] = 0;
			this->decideParkingLot[1] = 255;
			this->decideParkingLot[2] = 0;
		break;

		case AMBIGUOUS:		//YELLOW(������ �� �� ����(�ǽ� ����)
			this->decideParkingLot[0] = 0;
			this->decideParkingLot[1] = 255;
			this->decideParkingLot[2] = 255;
			break;

		case FULL:			// RED(������ ������)
			this->decideParkingLot[0] = 0;
			this->decideParkingLot[1] = 0;
			this->decideParkingLot[2] = 255;
			break;
		default:
			cerr << "[ERROR] area::setDecideParkingLot Function is not operating" << endl;
	}
}

Scalar ParkingLotArea::getDecideParkingLot() {
	return this->decideParkingLot;
}

#endif