#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "kcftracker.hpp"
using namespace std;
using namespace cv;
bool drawing_box = false;
bool gotBB = false;
Rect box;
//画矩形框
void drawBox(Mat& image, CvRect box, Scalar color, int thick){
	rectangle(image, cvPoint(box.x, box.y), cvPoint(box.x + box.width, box.y + box.height), color, thick);
}

//鼠标追踪函数
void mouseHandler(int event, int x, int y, int flags, void *param){
	switch (event){
	case CV_EVENT_MOUSEMOVE:
		if (drawing_box){
			box.width = x - box.x;
			box.height = y - box.y;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = Rect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width < 0){
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0){
			box.y += box.height;
			box.height *= -1;
		}
		gotBB = true;   //已经获得bounding box  
		break;
	}
}
int main(int argc, char* argv[]){
	VideoCapture capture;//打开摄像头函数
	capture.open(0);
	if(!capture.isOpened())
	{
		cout << "capture device failed to open!" << endl;
		return -1;
	}
	
	//注册鼠标画框事件
	cvNamedWindow("KCF", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("KCF", mouseHandler, NULL);  //用鼠标选中初始目标的bounding box  
	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = false;
	bool LAB = false;
	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
	// Frame readed
	Mat frame;
	// Tracker results
	Rect result;
	
	///Initialization  
	while (!gotBB)
	{
			capture >> frame;
			drawBox(frame, box,(0,0,255),2);  //把bounding box 画出来  
			imshow("KCF", frame);
			if (cvWaitKey(33) == 'q')
				return 0;
	}
	//取消鼠标点击事件
	cvSetMouseCallback("KCF", NULL, NULL);  //如果已经获得第一帧用户框定的box了，就取消鼠标响应  
	printf("Initial Bounding Box = x:%d y:%d h:%d w:%d\n", box.x, box.y, box.width, box.height);
	tracker.init(box, frame);//初始化追踪框
	while (capture.read(frame))
	{
		result = tracker.update(frame);//追踪
		//画矩形框
		rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
		if (!SILENT){
			imshow("KCF", frame);
			waitKey(10);
		}
	}
}
