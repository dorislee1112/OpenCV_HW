#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;

Mat video_frame;
int slider_value1, slider_value2;
Mat camera_frame, gray, Gblur;
Mat dst1, dst2, detected_edges1, detected_edges2;
int edgeThresh = 1;
int Threshold1, Threshold2;
int const max_Threshold = 100;
int kernel_size = 5;
char* window1 = "OpenCV_Canny";
char* window2 = "OpenCV_Scharr";
    
void CannyThreshold(int, void*) {

	Canny(Gblur, detected_edges1, Threshold1, Threshold1*10, kernel_size);
	
	dst1 = Scalar::all(0);
	cvtColor(detected_edges1, detected_edges1, CV_GRAY2BGR);
	
	float alpha = (float)(100 - Threshold1) / 100;
	float beta = (float)(1-alpha);

	addWeighted(camera_frame, alpha, detected_edges1, beta, 0.0, dst1);

	Mat imageROI; 
	Mat small;
	resize(dst1, small, Size(dst1.cols/2.5, dst1.rows/2.5));
	Rect roi(Point(0, 0), small.size());
	small.copyTo(video_frame(roi));
	imshow(window1, video_frame);
}

void ScharrThreshold(int, void*) {
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Scharr(Gblur, grad_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);  //Âà¦¨CV_8U
	Scharr(Gblur, grad_y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	Mat mod_img;
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, mod_img);
	threshold(mod_img, detected_edges2, 127, 255, THRESH_BINARY | THRESH_OTSU);

	dst2 = Scalar::all(0);
	cvtColor(mod_img, mod_img, CV_GRAY2BGR);

	float alpha = (float)(100 - Threshold2) / 100;
	float beta = (float)(1 - alpha);

	addWeighted(camera_frame, alpha, mod_img, beta, 0.0, dst2);

	Mat imageROI;
	Mat small;
	resize(dst2, small, Size(dst2.cols / 2.5, dst2.rows / 2.5));
	Rect roi(Point(0, 0), small.size());
	small.copyTo(video_frame(roi));
	imshow(window2, video_frame);
}

int main()
{
	slider_value1 = 0;
	slider_value2 = 0;
	int slider_max = 100;
	namedWindow(window1, CV_WINDOW_AUTOSIZE);
	namedWindow(window2, CV_WINDOW_AUTOSIZE);

	VideoCapture video_cap("video.mp4");
	if (!video_cap.isOpened()) return -1;
	VideoCapture camera_cap(0);
	if (!camera_cap.isOpened()) return -1;

	for (;;){
		
		video_cap.read(video_frame);
		camera_cap >> camera_frame;

		if (!camera_frame.data)
			return -1;

		dst1.create(camera_frame.size(), camera_frame.type());
		dst2.create(camera_frame.size(), camera_frame.type());
		cvtColor(camera_frame, gray, CV_BGR2GRAY);
		GaussianBlur(gray, Gblur, Size(3, 3), 0, 0);

		createTrackbar("Threshold:", window1, &Threshold1, max_Threshold, CannyThreshold);
		createTrackbar("Threshold:", window2, &Threshold2, max_Threshold, CannyThreshold);
		CannyThreshold(0, 0);
		ScharrThreshold(0, 0); 

		waitKey(30);
	}
	return 0;
}