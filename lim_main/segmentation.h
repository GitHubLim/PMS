#ifndef DEFINE_SEGMENTATION_H_

#define DEFINE_SEGMENTATION_H_

#include<opencv2/opencv.hpp>

#define MINOBJECTSIZE 100
#define MAXOBJECTSIZE 800000
#define PASSBYMAT 30

using namespace cv;

//원하는 color에서 영역 추출
class Object {

private:
	Point startPoint, endPoint;
	float extent;

public:
	Object(){}
	~Object(){}

	Object(Point sp, Point ep) {
		setStartPoint(sp.x, sp.y);
		setEndPoint(ep.x, ep.y);
	}

	void setStartPoint(int x, int y) {
		startPoint.x = x;
		startPoint.y = y;
	}

	void setEndPoint(int x, int y) {
		endPoint.x = x;
		endPoint.y = y;
	}

	Point getStartPoint() {
		return startPoint;
	}

	Point getEndPoint() {
		return endPoint;
	}

	float getExtent() {
		extent = (endPoint.x - startPoint.x)*(endPoint.y - startPoint.y);
		return extent;
	}
};

void findObjectFun(int x, int y, int preLabel, Object* object, Mat* mat) {
	//Start point, End point 지정
	Point startPoint, endPoint;
	int label;

	//예외처리(frame 외각지역)
	if (x < 0 || x > mat->size().width - 1 || y < 0 || y > mat->size().height - 1  )
		return;

	//1.Labeling
	label = mat->at<uchar>(y, x);

	if (label != preLabel)
		return;

	// 이미 지나간점 색상 지정 
	mat->at<uchar>(y, x) = PASSBYMAT;

	//No address type
	startPoint = object->getStartPoint();
	endPoint = object->getEndPoint();

	//Start Point 처리
	if (startPoint.x > x)
		startPoint.x = x;

	if (startPoint.y > y)
		startPoint.y = y;

	//End Point 처리
	if (endPoint.x < x)
		endPoint.x = x;

	if (endPoint.y < y)
		endPoint.y = y;

	object->setStartPoint(startPoint.x, startPoint.y);
	object->setEndPoint(endPoint.x, endPoint.y);

	//FOR문으로 변경..
	//왼쪽 -> 위 -> 아래 -> 오른쪽
	findObjectFun(x - 1, y, label, object, mat);
	findObjectFun(x , y - 1, label, object, mat);
	findObjectFun(x, y + 1, label, object, mat);
	findObjectFun(x + 1, y, label, object, mat);
}

#endif