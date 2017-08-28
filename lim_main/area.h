#pragma once
#ifndef PMS_AREA_HEADER
#define PMS_AREA_HEADER

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ParkingLotArea {

private:
	//���� ���� ��ġ
	Point topLeft;
	Point bottomLeft;
	Point bottomRight;
	Point topRight;

	int ID; //�������� ID
	int area;

	int level = 0; //���� ���� �ܰ躰 �Ǵ�
	Point parkingLotPoint;
	Scalar decideParkingLot; //���� ���� ��

public:
	ParkingLotArea();
	~ParkingLotArea();

	void setParkingLot(Point topLeft, Point bottomLeft, Point bottomRight, Point topRight);
	void setID(int id);
	void setDecideParkingLot(int level);
	Point getTopLeft();
	Point getBottomLeft();
	Point getTopRight();
	Point getBottomRight();
	Point getParkingLotPoint();

	int getID();
	int getArea();
	Scalar getDecideParkingLot();
};

ParkingLotArea::ParkingLotArea() {
	setDecideParkingLot(this->level);
}

ParkingLotArea::~ParkingLotArea() {
}

void ParkingLotArea::setID(int id) {
	this->ID = id;
}

int ParkingLotArea::getID() {
	return this->ID;
}
void ParkingLotArea::setParkingLot(Point topLeft, Point bottomLeft, Point bottomRight, Point topRight) {
	//parking lot ��ǥ �Է�
	this->topLeft = topLeft;
	this->bottomLeft = bottomLeft;
	this->topRight = topRight;
	this->bottomRight = bottomRight;

	//parking lot ���� Ȯ�� ��ǥ 
	parkingLotPoint.x = (bottomLeft.x + bottomRight.x) / 2;
	parkingLotPoint.y = (bottomLeft.y + bottomRight.y) / 2;

	//parking lot ���� ����
	float widthTop;
	float widthBottom;
	float heightLeft;
	float heightRight;
	float crossLine;

	widthTop = sqrt(pow(topLeft.x - topRight.x, 2.0) + pow(topLeft.y - topRight.y, 2.0));
	widthBottom = sqrt(pow(bottomLeft.x - bottomRight.x, 2.0) + pow(bottomLeft.y - bottomRight.y, 2.0));
	heightLeft = sqrt(pow(topLeft.x - bottomLeft.x, 2.0) + pow(topLeft.y - bottomLeft.y, 2.0));
	heightRight = sqrt(pow(bottomRight.x - topRight.x, 2.0) + pow(bottomRight.y - topRight.y, 2.0));
	crossLine = sqrt(pow(topLeft.x - bottomRight.x, 2.0) + pow(topLeft.y - bottomRight.y, 2.0));

	//�ﰢ�� ���� s1, s2(����� ����) = �簢�� ���� = S1+S2
	float s0, s1, s2;
	s0 = (widthTop + heightLeft + crossLine) / 2;
	s1 = sqrt(s0*(s0 - widthTop)*(s0 - heightLeft)*(s0 - crossLine));

	s0 = (widthBottom + widthBottom + crossLine) / 2;
	s2 = sqrt(s0*(s0 - widthBottom)*(s0 - widthBottom)*(s0 - crossLine));

	this->area = (int)(s1 + s2);
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

Point ParkingLotArea::getParkingLotPoint() {
	return this->parkingLotPoint;
}

int ParkingLotArea::getArea() {
	return this->area;
}

void ParkingLotArea::setDecideParkingLot(int level) {
	this->level = level;

	switch (this->level) {
		case 0:		// RED(������ ������)
			this->decideParkingLot[0] = 0;
			this->decideParkingLot[1] = 255;
			this->decideParkingLot[2] = 0;
			break;
		case 1:		//YELLOW(������ �� �� ����(�ǽ� ����)
			this->decideParkingLot[0] = 0;
			this->decideParkingLot[1] = 255;
			this->decideParkingLot[2] = 255;
			break;
		case 2: 	//GREEN(������ �� �� ����)
			this->decideParkingLot[0] = 0;
			this->decideParkingLot[1] = 0;
			this->decideParkingLot[2] = 255;
			break;
		default:
			cerr << "setDecideParkingLot Function is not operating" << endl;
	}

}

Scalar ParkingLotArea::getDecideParkingLot() {
	return this->decideParkingLot;
}

#endif