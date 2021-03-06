#include "stdafx.h"
#include "quality.h"

Mat ComputeGabor(Mat image);
double BrightnessDarkSpot(Mat image, double alpha);
double BrightnessY(Mat image);
double BrightnessEqualize(Mat image);
double BrightnessMiddlePixel(Mat image);
double PoseEyesAngle(Vec4f eyePoints);
double PoseEyesLength(Vec4f eyePoints);
double SymmetryHist(Mat image);
double SymmetryGabor(Mat image);
double SymmetryLbph(Mat image);	//LBPH衡量对称


//LBPH衡量对称性
double SymmetryLbph(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	Mat hist0 = ComputerLBPH(image0);
	Mat hist1 = ComputerLBPH(image1);
	return CompareLBPH(hist0, hist1);

}

//计算gabor
Mat ComputeGabor(Mat image)
{
	IplImage* img = &IplImage(image);
	//创建一个方向是PI/4而尺度是3的gabor
	double Sigma = 2*PI;  
	double F = sqrt(2.0);  
	CvGabor *gabor1 = new CvGabor;  
	gabor1->Init(4*PI/8, 5, Sigma, F);

	//获得实部并显示它
	//IplImage *kernel = cvCreateImage( cvSize(gabor1->get_mask_width(), gabor1->get_mask_width()), IPL_DEPTH_8U, 1);
	//kernel = gabor1->get_image(CV_GABOR_REAL);
	//cvNamedWindow("Gabor Kernel", 1);
	//cvShowImage("Gabor Kernel", kernel);
	//cvWaitKey(0);

	//载入一个图像并显示
	//IplImage *img = cvLoadImage( "D:/Demo.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	//cvNamedWindow("Original Image", 1);
	//cvShowImage("Original Image", img);
	//cvWaitKey(0);

	//获取载入图像的gabor滤波响应的模并且显示
	IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	gabor1->conv_img(img, reimg, CV_GABOR_MAG);	//CV_GABOR_REAL, CV_GABOR_IMAG, CV_GABOR_MAG
	//cvNamedWindow("Magnitude Response", 1);
	//cvShowImage("Magnitude Response",reimg);
	//cvWaitKey(0);
	//cvDestroyWindow("Magnitude Response");

	return cvarrToMat(reimg);
}
//gabor衡量对称性
double Symmetry_gabor(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	image0 = ComputeGabor(image0);
	image1 = ComputeGabor(image1);
	absdiff(image0, image1, image1);
	Scalar ssum = sum(image1);
	return ssum[0];
}

//直方图衡量左右脸对称性
double SymmetryHist(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	//imshow("left", image0);
	//imshow("right", image1);
	Mat hist0 = ComputeHist(image0);
	Mat hist1 = ComputeHist(image1);
	return compareHist(hist0, hist1, CV_COMP_CHISQR);//CV_COMP_CORREL; CV_COMP_CHISQR; CV_COMP_INTERSECT; CV_COMP_BHATTACHARYYA
}

//计算两眼坐标的距离
double PoseEyesLength(Vec4f eyePoints)
{
	return abs(eyePoints[3]-eyePoints[1])*abs(eyePoints[3]-eyePoints[1]) + abs(eyePoints[2]-eyePoints[0])*abs(eyePoints[2]-eyePoints[0]);	
}

//************************************
// 函数名称: zpose_eyes_angle
// 函数说明:  计算两眼坐标角度
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: double
// 参    数:Vec4f eyePoints两眼坐标
//************************************
double PoseEyesAngle(Vec4f eyePoints)
{
	return abs( atan2((eyePoints[3]-eyePoints[1]), (eyePoints[2]-eyePoints[0])) );	//atan(y,x)返回弧度
}

//************************************
// 函数名称: zbrightness_middle_pixel
// 函数说明:  计算中间80%的像素数量的灰度范围和总的灰度范围的80%的比例
// 作    者: Juliwei
// 完成日期: 2014/08/07
// 返 回 值: double
// 参    数:Mat image单通道
//************************************
//计算中间80%的像素数量的灰度范围和总的灰度范围的80%的比例
double BrightnessMiddlePixel(Mat image)
{
	//计算直方图
	Mat imageHist = ComputeHist(image);	//行向量
	//输出直方图
	//FileStorage fs("hist.xml", FileStorage::WRITE);
	//fs<<"Hist"<<imageHist;
	//fs.release();
	int sum = 0;
	double lowRate = 0.1, upRate = 0.1;	//上下限的比例阈值
	int low=0, up=255;	//上下限
	int pixelNum = image.rows*image.cols;
	int minLoc = 0, maxLoc = 255;

	for (int i = 0; i < 255; i++)	//求最小灰度
	{
		int temp = imageHist.at<float>(i);
		if (temp > 5)	//排除像素数目过小的灰度
		{
			minLoc = i;
			break;
		}
	}
	for (int j = 255; j > 0; j--)	//最大灰度
	{
		if (imageHist.at<float>(j) > 5)
		{
			maxLoc = j;
			break;
		}
	}

	for (int i = minLoc; i < maxLoc; i++)	//求下限
	{
		sum += imageHist.at<float>(i);
		if (sum > lowRate*pixelNum)
		{
			low = i;
			break;
		}
	}
	sum = 0;
	for (int j = maxLoc; j > minLoc; j--)	//上限
	{
		sum += imageHist.at<float>(j);
		if (sum > upRate*pixelNum)
		{
			up = j;
			break;
		}
	}
	return abs((double)(up-low))/abs((1.0-lowRate-upRate)*(double)(maxLoc-minLoc));
}

//************************************
// 函数名称: BrightnessEqualize
// 函数说明:  直方图均衡之后与原图的均方差
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: double
// 参    数:Mat image灰度图
//************************************
double BrightnessEqualize(Mat image)
{
	int flag = 1;	//0-均衡图与原图比较；0-分块均衡与整体均衡比较
	double dmse;
	CV_Assert(image.channels() == 1);
	Mat imageHist = image.clone();
	Mat imageHist_block = image.clone();
	equalizeHist(image , imageHist);
	if (flag ==0)
	{
		dmse = Mse(image, imageHist);
		//double dmse = zPSNR(image, imageHist);
	} 
	else
	{
		int w = image.cols;
		int h = image.rows;
		Rect rec0 = Rect(0, 0, w/2, h/2);
		Rect rec1 = Rect(0, h/2, w/2, h/2);
		Rect rec2 = Rect(w/2, 0, w/2, h/2);
		Rect rec3 = Rect(w/2, h/2, w/2, h/2);
		equalizeHist(image(rec0) , imageHist_block(rec0));
		equalizeHist(image(rec1) , imageHist_block(rec1));
		equalizeHist(image(rec2) , imageHist_block(rec2));
		equalizeHist(image(rec3) , imageHist_block(rec3));
		dmse = Mse(image, imageHist_block);
	}
	//imshow("equalize", imageHist_block);
	//waitKey(0);
	return dmse;
}

//************************************
// 函数名称: BrightnessY
// 函数说明:  用YUV中Y分量的均值衡量
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: double
// 参    数:Mat image 必须是彩图
//************************************
double BrightnessY(Mat image)
{
	Mat temp;
	vector<Mat> dst;
	cvtColor(image, temp, CV_BGR2YUV);
	split(temp, dst);
	Scalar scmean = mean(dst[0]);
	return (double)scmean[0];
}

//************************************
// 函数名称: BrightnessDarkSpot
// 函数说明:  人脸图片中阴影像素点比例
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: double
// 参    数:Mat image 灰度图
// 参    数:double alpha  小于alpha*maxValue的算暗像素
//************************************
double BrightnessDarkSpot(Mat image, double alpha)
{
	double num = 0.0;
	double iMaxValue = 0, iMinValue;
	minMaxLoc(image, NULL, &iMaxValue);
	for (int i=0; i<image.rows; i++)
		for (int j=0; j<image.cols; j++)
		{
			if (image.at<uchar>(i, j) < alpha * iMaxValue) num+=1.0;
		}
		return num / (double)(image.rows * image.cols);
}

//
//************************************
// 函数名称: SharpnessSobel
// 函数说明:  通过sobel梯度评估清晰度
// 作    者: Juliwei
// 完成日期: 2014/09/11
// 返 回 值: double
// 参    数:Mat image
// 参    数:int numOfDirection梯度方向的数目，默认为2，即0，90度方向
//************************************
double SharpnessSobel(Mat image, int numOfDirection)
{
	double sum = 0.0;
	Mat dst_x, dst_y;
	Sobel(image, dst_x, image.depth(), 1, 0);
	Sobel(image, dst_y, image.depth(), 0, 1);
	convertScaleAbs(dst_x, dst_x);//结果为8U
	convertScaleAbs(dst_y, dst_y);
	dst_x.convertTo(dst_x, CV_32F);
	dst_y.convertTo(dst_y, CV_32F);
	Mat dst_mag = dst_x.clone();
	magnitude(dst_x, dst_y, dst_mag);//要求为32F
	for (int i = 0; i < dst_mag.rows; i++)
		for (int j = 0; j < dst_mag.cols; j++)
		{
			sum +=dst_mag.at<float>(i, j);
		}
		return sum;
}

//************************************
// 函数名称: SharpnessDct
// 函数说明:  取DCT变换后的高频成分，再反变换得到的图像与原图的距离
// 作    者: Juliwei
// 完成日期: 2014/09/11
// 返 回 值: double
// 参    数:Mat image
// 参    数:double belta小于belta*max_value的赋0
//************************************
double SharpnessDct(Mat image, double belta)
{
	Mat image1(image.size(), CV_32FC1);
	Mat dst(image.size(), CV_32FC1);
	image.convertTo(image1, CV_32FC1);
	dct(image1, dst, CV_DXT_FORWARD);
	double max_value = 0;
	minMaxLoc(dst, NULL, &max_value);
	for(int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++)
		{
			if (dst.at<float>(i, j) < belta*max_value)
			{
				dst.at<float>(i, j) = 0.0;
			}
		}
		dct(dst, dst, CV_DXT_INVERSE);
		absdiff(dst, image1, image1);
		pow(image1, 2, image1);
		Scalar sharp = sum(image1);
		double c = pow(sharp[0], 0.5);
		return c / (double)(image1.rows * image1.cols);
}

//************************************
// 函数名称: SharpnessMedianBlur
// 函数说明:  中值滤波前后图像差的绝对值的平均值
// 作    者: Juliwei
// 完成日期: 2014/09/11
// 返 回 值: double
// 参    数:Mat image
// 参    数:int ksize滤波核大小
//************************************
double SharpnessMedianBlur(Mat image, int ksize)
{
	Mat blur_image;
	//blur(src, blur_image, Size(i, i));
	//GaussianBlur(image, blur_image, Size(i, i), 0, 0);
	medianBlur(image, blur_image, ksize);
	//bilateralFilter(image, blur_image, i, 2*i, i/2);
	absdiff(image, blur_image, blur_image);
	Scalar scmean = mean(blur_image);
	return scmean[0];
}
//************************************
// 函数名称: computerLightQuality
// 函数说明:  计算图片光照质量,分单通道、3通道
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: LightQuality
// 参    数:Mat image输入人脸图片
//************************************
LightQuality ComputerLightQuality(Mat image)
{
	LightQuality lightQuality;
	if (image.channels() == 1)
	{
		lightQuality.dark_spot = BrightnessDarkSpot(image, 0.3);
		lightQuality.equalization = BrightnessEqualize(image);
		lightQuality.y = 0;
		lightQuality.middle_pixel = BrightnessMiddlePixel(image);	//实际是80%像素所占灰度值
		lightQuality.mean = sum(image).val[0];
	}
	if (image.channels() == 3)
	{
		lightQuality.y = BrightnessY(image);
		lightQuality.middle_pixel = BrightnessMiddlePixel(image);
		Mat grayImage;
		cvtColor(image, grayImage, CV_BGR2GRAY);
		lightQuality.dark_spot = BrightnessDarkSpot(grayImage, 0.3);
		lightQuality.equalization = BrightnessEqualize(grayImage);
		lightQuality.mean = sum(image).val[0];
	}
	return lightQuality;
}

//************************************
// 函数名称: computerPoseQuality
// 函数说明:  计算图片姿态质量
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: PoseQuality
// 参    数:Vec4f eyePoints两眼坐标点
//************************************
PoseQuality ComputerPoseQuality(Vec4f eyePoints)
{
	PoseQuality poseQuality;

	poseQuality.eyes_angle = PoseEyesAngle(eyePoints);
	poseQuality.eyes_length = PoseEyesLength(eyePoints);
	return poseQuality;
}

//
//************************************
// 函数名称: computerSymmetryQuality
// 函数说明: 计算图片对称性 
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: LightQuality
// 参    数:Mat image
//************************************//
SymmetryQuality ComputerSymmetryQuality(Mat image)
{
	SymmetryQuality symmetryQuality;
	if (image.channels() == 3)
	{
		cvtColor(image, image, CV_BGR2GRAY);
	}
	if (image.channels() == 1)
	{
		symmetryQuality.hist = SymmetryHist(image);
		symmetryQuality.lbph = SymmetryLbph(image);
		symmetryQuality.gabor = Symmetry_gabor(image);
	}

	return symmetryQuality;
}

//TODO:
SharpnessQuality ComputerSharpnessQuality(Mat image)
{
	SharpnessQuality sharpnessQuality;
	if (image.channels() == 3)
	{
		cvtColor(image, image, CV_BGR2GRAY);
	}
	if (image.channels() == 1)
	{
		sharpnessQuality.diff_sobel = SharpnessSobel(image, 2);
		sharpnessQuality.diff_medianBlur = SharpnessMedianBlur(image, 3);
		sharpnessQuality.diff_dct = SharpnessDct(image, 0.2);
	}
	return sharpnessQuality;
}


//************************************
// 函数名称: ComputerQuality
// 函数说明:  计算Face人脸图片质量
// 作    者: Juliwei
// 完成日期: 2014/08/29
// 返 回 值: Quality
// 参    数:const Face & faceInfo
//************************************
Quality ComputerQuality(const Face& faceInfo)
{
	Quality quality;
	quality.lightQuality = ComputerLightQuality(faceInfo.face);
	quality.poseQuality = ComputerPoseQuality(faceInfo.eyePoints);
	quality.symmetryQuality = ComputerSymmetryQuality(faceInfo.face);
	quality.sharpnessQuality = ComputerSharpnessQuality(faceInfo.face);
	return quality;
}

//************************************
// 函数名称: QualityAsRowMatrix
// 函数说明:  将Quality个指标转化成Mat行，方便训练
// 作    者: Juliwei
// 完成日期: 2014/08/29
// 返 回 值: int
// 参    数:const Quality & quality
// 参    数:Mat & matOfQuality
//************************************
int QualityAsRowMatrix(const Quality& quality, Mat& matOfQuality)
{
	int num = 11;
	matOfQuality.create(1, num, CV_64F);
	if (matOfQuality.empty())
	{
		return -1;
	}
	matOfQuality.at<double>(0, 0) = quality.lightQuality.dark_spot;
	matOfQuality.at<double>(0, 1) = quality.lightQuality.equalization;
	matOfQuality.at<double>(0, 2) = quality.lightQuality.middle_pixel;
	matOfQuality.at<double>(0, 3) = quality.symmetryQuality.hist;
	matOfQuality.at<double>(0, 4) = quality.symmetryQuality.lbph;
	matOfQuality.at<double>(0, 5) = quality.symmetryQuality.gabor;
	matOfQuality.at<double>(0, 6) = quality.poseQuality.eyes_angle;
	matOfQuality.at<double>(0, 7) = quality.poseQuality.eyes_length;
	matOfQuality.at<double>(0, 8) = quality.sharpnessQuality.diff_sobel;
	matOfQuality.at<double>(0, 9) = quality.sharpnessQuality.diff_medianBlur;
	matOfQuality.at<double>(0, 10) = quality.sharpnessQuality.diff_dct;
	return 0;

}
