#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{

	Mat frame;
	VideoCapture cap("video.mp4");

	if (!cap.isOpened()){
		cout << "Cannot open the video" << endl;
		return -1;
	}

	for (;;){
		cap.read(frame);
		if (frame.rows > 0) imshow("Video Frame", frame);
		else break;
		waitKey(30);
	}
	return 0;
}