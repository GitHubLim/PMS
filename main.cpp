#include <iostream>
#include <opencv2/opencv.hpp>
#include "pms.h"

using namespace std;
using namespace cv;

//DO NOT REVISE THIS MAIN CODE 
int main(void) {

	VideoCapture video(FILENAME);

	//Error in opening the video input
	if (!video.isOpened()) {
		cerr << "Unable to open video file: " << FILENAME << endl;
		exit(EXIT_FAILURE);
	}

	//VIDEO 320 X 240 
	video.set(CAP_PROP_FRAME_WIDTH, 320);
	video.set(CAP_PROP_FRAME_HEIGHT, 240);

	Mat frame, houghFrame, kmeansFrame, ppFrame, background, mask;
	int flag = 0;

	//Background frame
	video >> background;
	mask = preMasking(background);
	background = preprocessing(background, mask);
	
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


	//MAIN LOOP
	while (true) {
		video >> frame;

		//resize(frame, frame, Size(800, 600), 0, 0, CV_INTER_LINEAR);

		//Rotate
		//Point2f src_center(frame.cols / 2.0F, frame.rows / 2.0F);
		//Mat rot_matrix = getRotationMatrix2D(src_center, 180, 1.0);
		//warpAffine(frame, frame, rot_matrix, frame.size());


		////�󺧸� 
		//Mat img_labels, stats, centroids;
		//int numOfLables = connectedComponentsWithStats(img_binary, img_labels,
		//	stats, centroids, 8, CV_32S);


		////�����ڽ� �׸���
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

		

		//�������� ��ġȮ��
		//frame = drawCircle(frame);

		//��ó��
		frame = preprocessing(frame, mask);

		//������
		Mat foreground = diffFrame(background, frame);

		//Labeling 
		makeLabeling(frame, foreground);

		//FPS 
		string fps = to_string((int)video.get(CV_CAP_PROP_FPS));
		putText(frame, "FPS : " + fps, Point(10, 40), 2, 1.2, Scalar(0, 255, 0));

		//frame = carHaarCascadeFun(frame);

		//HoughLine
		//frame = houghLine(frame);
		/*Mat image = imread("test2.PNG", 1);
		frame = houghLine(image)*/;

		//Find object 
		clock_t begin = clock();
		int count = 0;
		
		for (int y = 0; y < foreground.rows; y++) {
			for (int x = 0; x < foreground.cols; x++) {
				
				//Mask�� ���� 255�ϰ�� 
				if ((int)foreground.at<uchar>(y, x)){
					count++;
					int label = (int)foreground.at<uchar>(y, x);
					//cout << "X :" << x << "Y: " << y << endl;
					//circle(frame, Point(x,y), 10, Scalar(0, 0, 255));
					//1.Object ����
					Object object;
					object.setStartPoint(x, y);
					findObjectFun(x, y, -1, &object, &foreground);

					Point sPoint, ePoint;
					sPoint = object.getStartPoint();
					ePoint = object.getEndPoint();

					/*cout << "[SP] x :" << sPoint.x << " y : " << sPoint.y << endl;
					cout << "[EP] x :" << ePoint.x << " y : " << ePoint.y << endl;*/

					//2.Object�� Car���� �Ǻ�

					//CODE �����...
					//

					// ���� 1000 �̸� ������
					/*if (object.getExtent() > MINOBJECTSIZE) {
						circle(frame, sPoint, 10, Scalar(0, 0, 255));
						rectangle(frame, sPoint, ePoint, Scalar(0, 0, 255), 1);
					}*/

				}
			}
		}
		cout << count << endl;
		clock_t end = clock();

		//cout << "Algorithm ����ð� : " << (float)(end - begin) / CLOCKS_PER_SEC << endl;
		

		imshow("PMS", frame);
		imshow("FOREGROUND", foreground);
		imshow("BACKGROUND", background);
		//imshow("����ȭ ����", img_binary);

		//mouse call back 
		setMouseCallback("PMS", mouseClickFun, NULL);

		//Pushing ESC key is terminate
		if (waitKey(DELAYMILL) == ESC)
			break;
	}
	return 0;
}