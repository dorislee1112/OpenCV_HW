/*#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int slider_value;
Mat camera_frame, camera_frame_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Camera Frame";

void CannyThreshold(int, void*){
	//imshow("Camera Frame", camera_frame);
	/// Create a matrix of the same type and size as src (for dst)
	dst.create(camera_frame.size(), camera_frame.type());
	/// Convert the image to grayscale
	cvtColor(camera_frame, camera_frame_gray, CV_BGR2GRAY);
	/// Create a window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	/// Reduce noise with a kernel 3x3
	blur(camera_frame, detected_edges, Size(3, 3));
	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);
	camera_frame.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

int main()
{
	Mat video_frame;

	namedWindow("Video Frame", 1);
	//namedWindow("Camera Frame", 1);

	slider_value = 0;
	int slider_max = 100;

	VideoCapture video_cap("video.mp4");
	if (!video_cap.isOpened()) return -1;
	VideoCapture camera_cap(0);
	if (!camera_cap.isOpened()) return -1;

	for (;;){
		
		video_cap.read(video_frame);
		if (video_frame.rows > 0) imshow("Video Frame", video_frame);
		else break;
		
		camera_cap >> camera_frame;
		//imshow("Camera Frame", camera_frame);

		if (!camera_frame.data)
			return -1;
		
		/// Create a Trackbar for user to enter threshold
		createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
		/// Show the image
		CannyThreshold(0, 0);
		/// Wait until user exit program by pressing a key
		waitKey(30);

		//waitKey(30);
	}
	return 0;
}
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 10;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void CannyThreshold(int, void*){
	
	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);
	
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));
	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold/ratio, kernel_size);
	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

int main(int argc, char** argv){
	/// Load an image
	src = imread("r.png");

	if (!src.data)
	{
		return -1;
	}
	/// Create a window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	/// Create a matrix of the same type and size as src (for dst)
	dst.create(src.size(), src.type());
	/// Create a Trackbar for user to enter threshold
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	/// Show the image
	CannyThreshold(0, 0);
	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}