#pragma once
//-------------- 사용하지 않는 함수 및 변수 ---------------
//Rotate

//Point2f src_center(frame.cols / 2.0F, frame.rows / 2.0F);
//Mat rot_matrix = getRotationMatrix2D(src_center, 180, 1.0);
//warpAffine(frame, frame, rot_matrix, frame.size());

//영역박스 그리기

//int max = -1, idx = 0;
//for (int j = 1; j < numOfLables; j++) {
//	int area = stats.at<int>(j, CC_STAT_AREA);
//	if (max < area)
//	{
//		max = area;
//		idx = j;
//	}
//}

//int left = stats.at<int>(idx, CC_STAT_LEFT);
//int top = stats.at<int>(idx, CC_STAT_TOP);
//int width = stats.at<int>(idx, CC_STAT_WIDTH);
//int height = stats.at<int>(idx, CC_STAT_HEIGHT);

/*rectangle(frame, Point(left, top), Point(left + width, top + height),
Scalar(0, 0, 255), 1);*/


//cout << tempTime << endl;
//frame = carHaarCascadeFun(frame);

//HoughLine                                                                                                                    
//frame = houghLine(frame);
/*Mat image = imread("test2.PNG", 1);
frame = houghLine(image)*/
// --------------------------------------------

//HSV Trackbar
//namedWindow("찾을 색범위 설정", CV_WINDOW_AUTOSIZE);

//트랙바에서 사용되는 변수 초기화 
/*int LowH = LOWH;
int HighH = HIGHH;
int LowS = LOWS;
int HighS = HIGHS;
int LowV = LOWV;
int HighV = HIGHV;*/

////트랙바 생성 
//cvCreateTrackbar("LowH", "찾을 색범위 설정", &LowH, 179); //Hue (0 - 179)
//cvCreateTrackbar("HighH", "찾을 색범위 설정", &HighH, 179);

//cvCreateTrackbar("LowS", "찾을 색범위 설정", &LowS, 255); //Saturation (0 - 255)
//cvCreateTrackbar("HighS", "찾을 색범위 설정", &HighS, 255);

//cvCreateTrackbar("LowV", "찾을 색범위 설정", &LowV, 255); //Value (0 - 255)
//cvCreateTrackbar("HighV", "찾을 색범위 설정", &HighV, 255);

//영역 축소
//TopLeft, BottomLeft 조정
//int scale = 3;
//if (bottomLeft.x < topLeft.x) {
//	topLeftX -= (topLeft.x - bottomLeft.x) / scale;
//	bottomLeftX += (topLeft.x - bottomLeft.x) / scale;
//}else {
//	topLeftX += (bottomLeft.x - topLeft.x) / scale;
//	bottomLeftX -= (bottomLeft.x - topLeft.x) / scale;
//}

//topLeftY += (bottomLeft.y - topLeft.y) / scale;
//bottomLeftY -= (bottomLeft.y - topLeft.y) / scale;


////TopLeft, TopRight 조정
//if (topRight.y < topLeft.y) {
//	topLeftY -= (topLeft.y - topRight.y) / scale;
//	topRightY += (topLeft.y - topRight.y) / scale;
//}else {
//	topLeftY += (topRight.y - topLeft.y) / scale;
//	topRightY -= (topRight.y - topLeft.y) / scale;
//}

//topLeftX += (topRight.x - topLeft.x) / scale;
//topRightX -= (topRight.x - topLeft.x) / scale;

////TopRight, BottomRight 조정
//if (bottomRight.x < topRight.x) {
//	topRightX -= (topRight.x - bottomRight.x) / scale;
//	bottomRightX += (topRight.x - bottomRight.x) / scale;
//}
//else {
//	topRightX += (bottomRight.x - topRight.x) / scale;
//	bottomRightX -= (bottomRight.x - topRight.x) / scale;
//}

//topRightY += (bottomRight.y - topRight.y) / scale;
//bottomRightY -= (bottomRight.y - topRight.y) / scale;

////BottomLeft, BottomRight 조정
//if (bottomRight.y < bottomLeft.y) {
//	bottomLeftY -= (bottomLeft.y - bottomRight.y) / scale;
//	bottomRightY += (bottomLeft.y - bottomRight.y) / scale;
//}
//else {
//	bottomLeftY += (bottomRight.y - bottomLeft.y) / scale;
//	bottomRightY -= (bottomRight.y - bottomLeft.y) / scale;
//}

//bottomLeftX += (bottomRight.x - bottomLeft.x) / scale;
//bottomRightX -= (bottomRight.x - bottomLeft.x) / scale;

//Point topLeft1(topLeftX, topLeftY);
//Point bottomLeft1(bottomLeftX, bottomLeftY);
//Point topRight1(topRightX, topRightY);
//Point bottomRight1(bottomRightX, bottomRightY);

//

//plArea11.setParkingLot(topLeft1, bottomLeft1, topRight1, bottomRight1); //step 1

//Scalar lineColor1(255, 0, 0);

/*circle(srcFrame, topLeft1, 4, drawParkingLot, 4);
circle(srcFrame, bottomLeft1, 4, drawParkingLot, 4);
circle(srcFrame, topRight1, 4, drawParkingLot, 4);
circle(srcFrame, bottomRight1, 4, drawParkingLot, 4);*/

/*cout << "AREA : " << plArea1.getArea() << endl;
cout << " AREA(20%) " << plArea1.getArea() * 0.2
<< " AREA(50%) " << plArea1.getArea() * 0.5
<< " AREA(80%) " << plArea1.getArea() * 0.8 << endl;
*/


//int extent = stats.at<int>(i, CC_STAT_AREA);
//int left = stats.at<int>(i, CC_STAT_LEFT);
//int top = stats.at<int>(i, CC_STAT_TOP);
//int width = stats.at<int>(i, CC_STAT_WIDTH);
//int height = stats.at<int>(i, CC_STAT_HEIGHT);
//
////object 중심 좌표
//double x = centroids.at<double>(i, 0);
//double y = centroids.at<double>(i, 1);
//
//if (extent > area.getArea()*0.8) {
//	//min, max X 구하기
//	int minX = area.getTopLeft().x;
//	int maxX = area.getTopRight().x;
//
//	if (minX > area.getBottomLeft().x)
//		minX = area.getBottomLeft().x;
//
//	if (maxX < area.getBottomRight().x)
//		maxX = area.getBottomRight().x;
//
//	//min, max Y 구하기
//	int minY = area.getTopLeft().y;
//	int maxY = area.getBottomLeft().y;
//
//	if (minY > area.getTopRight().y)
//		minY = area.getTopRight().y;
//
//	if (maxY < area.getBottomRight().y)
//		maxY = area.getBottomRight().y;
//
//	if ((minX <= x) && (x <= maxX) && (minY <= y) && (y <= maxY))
//		isFull = true;
//	else
//		isFull = false;
//
//}
//else
//isFull = false;
