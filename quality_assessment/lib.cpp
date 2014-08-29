#include "stdafx.h"
#include "lib.h"

//************************************
// 函数名称: GetPointDistance
// 函数说明:  获得两点间距离
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: double
// 参    数:Point2f p1
// 参    数:Point2f p2
//************************************
double GetPointDistance(Point2f p1,Point2f p2)
{
	return sqrt(abs(p1.x-p2.x)*abs(p1.x-p2.x)+abs(p1.y-p2.y)*abs(p1.y-p2.y));
}

//
//************************************
// 函数名称: getFilenameFromPath
// 函数说明:  从路径获取文件名
// 作    者: Juliwei
// 完成日期: 2014/08/13
// 返 回 值: std::string文件名
// 参    数:string path文件路径
//************************************
string GetFilenameFromPath(string path)
{
	size_t npos = path.find_last_of('\\');
	return path.substr(npos+1, 50/*path.size()-npos-1-4-4*/);
}
//************************************
// 函数名称: alignFace
// 函数说明:  根据两眼坐标，人脸归一化,默认大小140*120
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: cv::Mat
// 参    数:Mat image
// 参    数:Point2f potLeft真实的左眼，观察者的右边
// 参    数:Point2f potRight真实的右眼
//************************************
Mat AlignFace(Mat image,Point2f potLeft, Point2f potRight)
{
	IplImage *img = &(IplImage)image;
	double dEyeDis = GetPointDistance(potLeft,potRight);
	double scale = 80/dEyeDis;	//两眼间距离80
	Point2f LeftTop,RightBot;
	/*LeftTop.x = potRight.x-(int)(dEyeDis/2);
	LeftTop.y = potRight.y-(int)(80/scale);*/

	//////角度校正
	double angle = atan((potLeft.y-potRight.y)/(potLeft.x-potRight.x));
	angle = angle * 180 / CV_PI;  
	double m[6];  
	CvMat map_matrix = cvMat(2, 3, CV_64FC1, m);  
	//Point center(static_cast<int>(image.cols/2),static_cast<int>(image.rows/2));
	CvPoint2D32f center;
	center.x = potRight.x;
	center.y = potRight.y;
	cv2DRotationMatrix(center, angle, 1.0, &map_matrix);  
	IplImage* dstImage = cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);  
	cvWarpAffine( img, dstImage, &map_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );  


	CvRect RcFace = cvRect(potRight.x-(int)20/scale,potRight.y-(int)(20/scale),120/scale,120/scale);	//选取的人脸区域大小
	cvSetImageROI(dstImage,RcFace);
	IplImage *imgSave = cvCreateImage(cvSize(RcFace.width,RcFace.height),img->depth,img->nChannels);
	cvResize(dstImage,imgSave);
	IplImage *norImage = cvCreateImage(cvSize(120,140),img->depth,img->nChannels);	//人脸区域归一化
	cvResize(imgSave,norImage);
	Mat dst(norImage, true);	//复制数据
	cvReleaseImage(&imgSave);
	cvReleaseImage(&norImage);
	cvResetImageROI(dstImage);
	cvReleaseImage(&dstImage);

	return dst;
}

//计算直方图
Mat ComputeHist(Mat src)
{
	int bins = 256;  
	int hist_size[] = {bins};  
	float range[] = { 0, 256 };  
	const float* ranges[] = { range};  
	Mat hist;  //
	int channels[] = {0};  
	calcHist( &src, 1, channels, Mat(), // do not use mask  
		hist, 1, hist_size, ranges,  
		true, // the histogram is uniform  
		false ); 
	//normalize(hist, hist, 0.0, 1.0, NORM_MINMAX);	//归一化
	return hist;
}

//计算两个矩阵协方差
double Mse(Mat image1, Mat image2)
{
	Mat s1;
	absdiff(image1, image2, s1);
	s1.convertTo(s1, CV_32F);
	s1= s1.mul(s1);
	Scalar s = sum(s1);
	double sse = s.val[0] + s.val[1] + s.val[2];
	if (sse <= 1e-10)
		return 0;
	else
	{
		double mse = sse / (double)(image1.channels()*image1.total());
		return mse;
	}
}
