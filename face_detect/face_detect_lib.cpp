#include "stdafx.h"
#include "face_detect_lib.h"
#include "opencv2/objdetect/objdetect.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 

using namespace std;

string face_cascade_name = "haarcascade_frontalface_alt.xml"; 
CascadeClassifier face_cascade; 
string window_name = "人脸识别";

Mat GetASMFace(Mat _src, int flag)
{
	IplImage* src = NULL;
	Cyw_ASM* ASM = NULL;
	if (0 == ASM->InitASM("classifier") )//../
	{
		exit(0);
	}
	vector<Point> landmarks;

	src = &IplImage(_src);
	int iasmFlag = ASM->Get77KeyPoints(src,landmarks);
	Mat shape = cvarrToMat(src).clone();
	if ( 0 != iasmFlag )
	{
		Vec3b red(0,0,255);
		switch (flag)
		{
		//整张图片，标明关键点
		case 0:
			for ( int i=0;i<77;i++)
			{

				int x = landmarks[i].x;
				int y = landmarks[i].y;
				//shape.at<Vec3b>(y,x) = red;
				circle(shape, Point(x,y), 2, Scalar(255,0,0),-1);

			}
			break;
		//人脸区域，标明关键点
		case 1:
			break;
		//人脸区域
		case 2:
			break;
		default:
			break;

		}
		//cvReleaseImage(&src);
	}
	return shape;
}


Mat GetOpencvFace( Mat frame)
{ 
	std::vector<Rect> faces; 
	Mat frame_gray; 
	Mat faceimg=frame;
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	face_cascade.load("haarcascade_frontalface_alt.xml");
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) ); 

	//for( int i = 0; i < faces.size(); i++ ){ 
	if(faces.size()>=1)
	{
		float scale=0;
		int maxFaceID=0;	//最大的人脸
		for (int i=0; i<faces.size(); i++ )
		{
			int maxArea=0;
			if (faces[i].width*faces[i].height > maxArea)
			{
				maxArea = faces[i].width*faces[i].height;
				maxFaceID = i;
			}
		}
		int dx=faces[maxFaceID].width*scale;
		int dy=faces[maxFaceID].height*scale;
		Point left=Point( faces[maxFaceID].x+dx, faces[maxFaceID].y+dy );
		Point right=Point(faces[maxFaceID].x+faces[maxFaceID].width-dx, faces[maxFaceID].y+faces[maxFaceID].height-dy);
		Rect rect(left,right);
		faceimg=frame_gray(rect);
	}
	return faceimg;
		//printf("检测失败\n");
	//imshow( window_name, frame ); 	
} 
