#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>  
#include <stdio.h>


using  namespace cv;
using  namespace std;

#define CAMER_CHANNEL     0

#define RESOLUTION_WIDTH  1280
#define RESOLUTION_HIGH   720
#define FPS               25 
/*
#define RESOLUTION_WIDTH  640
#define RESOLUTION_HIGH   480
#define FPS               25  */  

#define WINDOWS_NAME     "RECORDER, enter:START, enter again:STOP, space:EXIT"
int main(void)
{
	VideoCapture inputVideo;
	VideoWriter writeVideo;
#ifndef CAMER_CHANNEL
	inputVideo.open("test2802.avi");
#else
	inputVideo.open(CAMER_CHANNEL);
#endif

	if (!inputVideo.isOpened()) {
		cout << "Error:Cannot open the video" << endl;
		fprintf(stderr, "Error: Can't open video\n");
		return -1;
	}

	double videoFps = 25.0;
	inputVideo.set(CV_CAP_PROP_FPS, videoFps);
	videoFps = inputVideo.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
	cout << "Input video's Frame per seconds : " << videoFps << endl;

	int videoWidth = RESOLUTION_WIDTH;
	int videoHeight = RESOLUTION_HIGH;
	inputVideo.set(CV_CAP_PROP_FRAME_WIDTH, videoWidth);
	inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT, videoHeight);

	videoWidth =static_cast<int>(inputVideo.get(CV_CAP_PROP_FRAME_WIDTH));//get the width of frames of the video
	videoHeight = static_cast<int>(inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));//get the height of frames of the video
	std::cout << "Frame Size = " << videoWidth << "x" << videoHeight << std::endl;

	Mat frame;
	Mat tempFrame;
	int key_pressed=0;
	int countFrame=0;
	bool bRecord = false;
	int  fileNum = 0;

	char cbuf[50];
	memset(cbuf, 0, sizeof(cbuf));
	namedWindow(WINDOWS_NAME, WINDOW_NORMAL);
	while (key_pressed != 32) {

		if (!inputVideo.read(frame)){
			cout << "read video's Frame error" << endl;
			break;
		}
		frame.copyTo(tempFrame);
		// show middle line
		line(tempFrame, Point(frame.cols / 2, 0),
			Point(frame.cols / 2, frame.rows), CV_RGB(255, 255, 0), 1);

		line(tempFrame, Point(0, frame.rows / 2),
			Point(frame.cols, frame.rows/2), CV_RGB(0, 255, 0), 1);

		if (bRecord == true){
		    writeVideo.write(frame);
			countFrame++;
		    circle(tempFrame, Point(20, 20), 7, CV_RGB(255, 0, 0), 7,  8, 0);
			memset(cbuf, 0, sizeof(cbuf));
			sprintf(cbuf, "%dS", countFrame/15);
			putText(tempFrame, // 图像矩阵
				cbuf,                  // string型文字内容
				cv::Point(60, 30),           // 文字坐标，以左下角为原点
				cv::FONT_HERSHEY_SIMPLEX,   // 字体类型
				1, // 字体大小
				cv::Scalar(0, 255, 0)// 字体颜色
				, 2);       
		}
		imshow(WINDOWS_NAME, tempFrame);

		key_pressed = cvWaitKey(1000/videoFps);
		if (key_pressed == 13){//回车键

			if (bRecord == false)//开始录像
			{
				countFrame = 0;
				bRecord = true;
				fileNum++;
				memset(cbuf, 0, sizeof(cbuf));
				sprintf(cbuf, "D://record%d.avi", fileNum);
				//writeVideo.open(outFlie, -1, videoFps, Size(videoWidth, videoHeight), true);
				writeVideo.open(cbuf, CV_FOURCC('M', 'J', 'P', 'G'), videoFps, Size(RESOLUTION_WIDTH, RESOLUTION_HIGH), true);
				std::cout << "start record, filename:" << cbuf << endl;
				
			}
			else{
				countFrame = 0;
				bRecord = false;
				writeVideo.release();
				std::cout << "stop record!" << endl;
			}
		}
	}
	inputVideo.release();
	writeVideo.release();
	return 0;
}