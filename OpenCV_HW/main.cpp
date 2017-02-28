#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;

Mat video_frame;
int slider_value;
Mat camera_frame, gray, Gblur;
Mat dst, detected_edges;
int edgeThresh = 1;
int Threshold;
int const max_Threshold = 100;
char* window = "OpenCV_Canny";

void CannyThreshold(int, void*) {

	//Canny(Gblur, detected_edges, Threshold, Threshold*3, 3);
	//Canny(Gblur, detected_edges, Threshold /2 , Threshold * 1.5, 3);
	Canny(Gblur, detected_edges, 50, 150, 3);
	
	dst = Scalar::all(0);
	cvtColor(detected_edges, detected_edges, CV_GRAY2BGR);
	
	float alpha = (float)(100 - Threshold) / 100;
	float beta = (float)(1-alpha);

	addWeighted(camera_frame, alpha, detected_edges, beta, 0.0, dst);

	Mat imageROI; 
	Mat small;
	resize(dst, small, Size(dst.cols/2.5, dst.rows/2.5));
	Rect roi(Point(0, 0), small.size());
	small.copyTo(video_frame(roi));
	imshow(window, video_frame);
}

int main()
{
	slider_value = 0;
	int slider_max = 100;
	namedWindow(window, CV_WINDOW_AUTOSIZE);

	VideoCapture video_cap("video.mp4");
	if (!video_cap.isOpened()) return -1;
	VideoCapture camera_cap(0);
	if (!camera_cap.isOpened()) return -1;

	for (;;){
		
		video_cap.read(video_frame);
		camera_cap >> camera_frame;

		if (!camera_frame.data)
			return -1;
		if (!video_frame.data)
			return -1;

		dst.create(camera_frame.size(), camera_frame.type());
		cvtColor(camera_frame, gray, CV_BGR2GRAY);
		GaussianBlur(gray, Gblur, Size(3, 3), 0, 0);

		createTrackbar("Threshold:", window, &Threshold, max_Threshold, CannyThreshold);
		CannyThreshold(0, 0);

		waitKey(30);
	}
	return 0;
}