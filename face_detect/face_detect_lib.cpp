#include "stdafx.h"
#include "face_detect_lib.h"

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