#pragma once
//-------------- ������� �ʴ� �Լ� �� ���� ---------------
//Rotate

//Point2f src_center(frame.cols / 2.0F, frame.rows / 2.0F);
//Mat rot_matrix = getRotationMatrix2D(src_center, 180, 1.0);
//warpAffine(frame, frame, rot_matrix, frame.size());

//�����ڽ� �׸���

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
//namedWindow("ã�� ������ ����", CV_WINDOW_AUTOSIZE);

//Ʈ���ٿ��� ���Ǵ� ���� �ʱ�ȭ 
/*int LowH = LOWH;
int HighH = HIGHH;
int LowS = LOWS;
int HighS = HIGHS;
int LowV = LOWV;
int HighV = HIGHV;*/

////Ʈ���� ���� 
//cvCreateTrackbar("LowH", "ã�� ������ ����", &LowH, 179); //Hue (0 - 179)
//cvCreateTrackbar("HighH", "ã�� ������ ����", &HighH, 179);

//cvCreateTrackbar("LowS", "ã�� ������ ����", &LowS, 255); //Saturation (0 - 255)
//cvCreateTrackbar("HighS", "ã�� ������ ����", &HighS, 255);

//cvCreateTrackbar("LowV", "ã�� ������ ����", &LowV, 255); //Value (0 - 255)
//cvCreateTrackbar("HighV", "ã�� ������ ����", &HighV, 255);

//���� ���
//TopLeft, BottomLeft ����
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


////TopLeft, TopRight ����
//if (topRight.y < topLeft.y) {
//	topLeftY -= (topLeft.y - topRight.y) / scale;
//	topRightY += (topLeft.y - topRight.y) / scale;
//}else {
//	topLeftY += (topRight.y - topLeft.y) / scale;
//	topRightY -= (topRight.y - topLeft.y) / scale;
//}

//topLeftX += (topRight.x - topLeft.x) / scale;
//topRightX -= (topRight.x - topLeft.x) / scale;

////TopRight, BottomRight ����
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

////BottomLeft, BottomRight ����
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
////object �߽� ��ǥ
//double x = centroids.at<double>(i, 0);
//double y = centroids.at<double>(i, 1);
//
//if (extent > area.getArea()*0.8) {
//	//min, max X ���ϱ�
//	int minX = area.getTopLeft().x;
//	int maxX = area.getTopRight().x;
//
//	if (minX > area.getBottomLeft().x)
//		minX = area.getBottomLeft().x;
//
//	if (maxX < area.getBottomRight().x)
//		maxX = area.getBottomRight().x;
//
//	//min, max Y ���ϱ�
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
////------------- Warping ---------------
//Mat warpFrame, backWarpFrame;
////Warping ����
//plArea1.setParkingLot(Point(247, 12), Point(255, 58), Point(294, 60), Point(280, 10));

//warpFrame = warpingFun(frame, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));
//backWarpFrame = warpingFun(background, Point(346, 297), Point(341, 352), Point(638, 375), Point(621, 314));

////������(warping)
//Mat foreWarpFrame = diffFrame(backWarpFrame, warpFrame, true);

////Labeling(warping)
////makeLabeling(warpFrame, foreWarpFrame);

//

//////���� ���� �Ǵ�
////decideParkingLot(foreWarpFrame, &plArea1);

////--------------------------------------

////------------- Warping 2---------------
//Mat warpFrame2, backWarpFrame2;
////Warping ����
//plArea2.setParkingLot(Point(77, 77), Point(77, 100), Point(112, 96), Point(112, 73));

//warpFrame2 = warpingFun(frame, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));
//backWarpFrame2 = warpingFun(background, Point(209, 200), Point(63, 399), Point(180, 422), Point(274, 211));

////������(warping)
//Mat foreWarpFrame2 = diffFrame(backWarpFrame2, warpFrame2, true);

////Labeling(warping)
////makeLabeling(warpFrame2, foreWarpFrame2);

////--------------------------------------
////----------------- Decide Parking Lot -----------------

//Mat warpFrame3, backWarpFrame3, warpFrame4, backWarpFrame4;

////Warping ����
//warpFrame3 = warpingFun(warpFrame, Point(247, 12), Point(255, 58), Point(294, 60), Point(280, 10));
//backWarpFrame3 = warpingFun(backWarpFrame, Point(247, 12), Point(255, 58), Point(294, 60), Point(280, 10));
//warpFrame4 = warpingFun(warpFrame2, Point(77, 77), Point(77, 100), Point(112, 96), Point(112, 73));
//backWarpFrame4 = warpingFun(backWarpFrame2, Point(77, 77), Point(77, 100), Point(112, 96), Point(112, 73));

////������(warping)
//Mat foreWarpFrame3 = diffFrame(backWarpFrame3, warpFrame3, true);
//Mat foreWarpFrame4 = diffFrame(backWarpFrame4, warpFrame4, true);

//decideParkingLot(foreWarpFrame3, &plArea1, 0.75, 0.2);
//decideParkingLot(foreWarpFrame4, &plArea2, 0.3, 0.1);

////-----------------------------------------------------
//drawParkingLotFun(warpFrame, plArea1);
//drawParkingLotFun(warpFrame2, plArea2);
////imshow("WARPING4", warpFrame4);
////imshow("BACKWARPFRAME4", backWarpFrame4);
////imshow("FOREWARPINGFRAME4", foreWarpFrame4);

////------------ Main frame --------------
////Preprocessing
//frame = preprocessing(frame, mask);

////Difference between frame
//Mat foreground = diffFrame(background, frame, true);

//Parking line
//drawParkingLotFun(frame);

//Labeling 
//makeLabeling(frame, foreground);

//Car haarcascade
//frame = preprocessing(frame, foreground);
//Mat carFrame = detectHaarcascadesCar(frame);
//imshow("CARCASCADE", carFrame);

////����� ������ ��
//circle(srcFrame, Point(345, 300), 3, Scalar(0, 0, 255), 1);
//circle(srcFrame, Point(397, 303), 3, Scalar(0, 0, 255), 1);
//circle(srcFrame, Point(402, 355), 3, Scalar(0, 0, 255), 1);
//circle(srcFrame, Point(343, 347), 3, Scalar(0, 0, 255), 1);
//
////���� ù��° ��
//circle(srcFrame, Point(158, 450), 3, Scalar(0, 0, 225), 1);
////���� �ι�° ��
//circle(srcFrame, Point(171, 420), 3, Scalar(0, 0, 225), 1);
////���� ����° ��
//circle(srcFrame, Point(189, 382), 3, Scalar(0, 0, 225), 1);
////���� �׹�° ��
//circle(srcFrame, Point(204, 352), 3, Scalar(0, 0, 225), 1);
////���� �ټ���° ��
//circle(srcFrame, Point(215, 326), 3, Scalar(0, 0, 225), 1);
////���� ������° ��
//circle(srcFrame, Point(226, 304), 3, Scalar(0, 0, 225), 1);
////���� �ϰ���° ��
//circle(srcFrame, Point(237, 283), 3, Scalar(0, 0, 225), 1);
////���� ������° ��
//circle(srcFrame, Point(245, 266), 3, Scalar(0, 0, 225), 1);
////���� ��ȩ��° ��
//circle(srcFrame, Point(252, 251), 3, Scalar(0, 0, 225), 1);
////���� ����° ��
//circle(srcFrame, Point(257, 237), 3, Scalar(0, 0, 225), 1);
////���� ���ѹ�° ��
//circle(srcFrame, Point(264, 225), 3, Scalar(0, 0, 225), 1);
////���� ���ι�° ��
//circle(srcFrame, Point(269, 214), 3, Scalar(0, 0, 225), 1);
////���� ������° ��
//circle(srcFrame, Point(272, 204), 3, Scalar(0, 0, 225), 1);
////���� ���׹�° ��
//circle(srcFrame, Point(278, 194), 3, Scalar(0, 0, 225), 1);
////���� ���ټ���° ��
//circle(srcFrame, Point(281, 186), 3, Scalar(0, 0, 225), 1);
////���� ��������° ��
//circle(srcFrame, Point(285, 177), 3, Scalar(0, 0, 225), 1);
////���� ���ϰ���° ��
//circle(srcFrame, Point(287, 171), 3, Scalar(0, 0, 225), 1);
////���� ��������° ��
//circle(srcFrame, Point(291, 165), 3, Scalar(0, 0, 225), 1);
//
////����������� ������ ù��° ���� ��
//circle(srcFrame, Point(449, 359), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
//circle(srcFrame, Point(437, 305), 3, Scalar(0, 0, 225), 1); //�� ��
//
//															//����������� ������ �ι�° ���� ��
//circle(srcFrame, Point(495, 363), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
//circle(srcFrame, Point(477, 307), 3, Scalar(0, 0, 225), 1); //�� ��
//
//															//����������� ������ ����° ���� ��
//circle(srcFrame, Point(542, 367), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
//circle(srcFrame, Point(518, 309), 3, Scalar(0, 0, 225), 1); //�� ��
//
//															//����������� ������ �׹�° ���� ��
//circle(srcFrame, Point(590, 370), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
//circle(srcFrame, Point(563, 311), 3, Scalar(0, 0, 225), 1); //�� ��
//
//															//����������� ������ �ټ���° ���� ��
//circle(srcFrame, Point(636, 374), 3, Scalar(0, 0, 225), 1); // �Ʒ� ��
//circle(srcFrame, Point(605, 314), 3, Scalar(0, 0, 225), 1); //�� ��


//Mat diffFrameFun(Mat srcFrame, Mat background, bool isMorph) {
//	Mat foreground, grayFrame;
//	
//	//1. Source Frame�� Gray Frame���� ��ȯ
//	//cvtColor(srcFrame, grayFrame, COLOR_BayerRG2GRAY);
//	//GaussianBlur(srcFrame, srcFrame, Size(5, 5), 0.5);
//
//	//2. 2���� �������� ������ ���� => foreground
//	absdiff(background, srcFrame, foreground);
//	
//	//3. Erode, Dilate (�̼��� ���� ���� �� ���� ���� ��ħ)
//	if (isMorph) {
//		erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(19, 19)));
//	}
//	//dilate(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));
//	//erode(foreground, foreground, getStructuringElement(MORPH_ELLIPSE, Size(SCALETHIRD, SCALETHIRD)));
//
//	//4. foreground�� threshold (150 - 255)
//	int thresholdValue = 100;
//	int maxBinaryValue = 255;
//
//	cvtColor(foreground, foreground, CV_BGR2GRAY);
//	threshold(foreground, foreground, thresholdValue, maxBinaryValue, THRESH_BINARY);
//	
//	return foreground;
//}