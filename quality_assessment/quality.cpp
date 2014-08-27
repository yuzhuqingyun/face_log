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
double SymmetryLbph(Mat image);	//LBPH�����Գ�


//LBPH�����Գ���
double SymmetryLbph(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	Mat hist0 = ComputerLBPH(image0);
	Mat hist1 = ComputerLBPH(image1);
	return CompareLBPH(hist0, hist1);

}

//����gabor
Mat ComputeGabor(Mat image)
{
	IplImage* img = &IplImage(image);
	//����һ��������PI/4���߶���3��gabor
	double Sigma = 2*PI;  
	double F = sqrt(2.0);  
	CvGabor *gabor1 = new CvGabor;  
	gabor1->Init(4*PI/8, 5, Sigma, F);

	//���ʵ������ʾ��
	//IplImage *kernel = cvCreateImage( cvSize(gabor1->get_mask_width(), gabor1->get_mask_width()), IPL_DEPTH_8U, 1);
	//kernel = gabor1->get_image(CV_GABOR_REAL);
	//cvNamedWindow("Gabor Kernel", 1);
	//cvShowImage("Gabor Kernel", kernel);
	//cvWaitKey(0);

	//����һ��ͼ����ʾ
	//IplImage *img = cvLoadImage( "D:/Demo.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	//cvNamedWindow("Original Image", 1);
	//cvShowImage("Original Image", img);
	//cvWaitKey(0);

	//��ȡ����ͼ���gabor�˲���Ӧ��ģ������ʾ
	IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	gabor1->conv_img(img, reimg, CV_GABOR_MAG);	//CV_GABOR_REAL, CV_GABOR_IMAG, CV_GABOR_MAG
	//cvNamedWindow("Magnitude Response", 1);
	//cvShowImage("Magnitude Response",reimg);
	//cvWaitKey(0);
	//cvDestroyWindow("Magnitude Response");

	return cvarrToMat(reimg);
}
//gabor�����Գ���
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

//ֱ��ͼ�����������Գ���
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

//������������ľ���
double PoseEyesLength(Vec4f eyePoints)
{
	return abs(eyePoints[3]-eyePoints[1])*abs(eyePoints[3]-eyePoints[1]) + abs(eyePoints[2]-eyePoints[0])*abs(eyePoints[2]-eyePoints[0]);	
}

//************************************
// ��������: zpose_eyes_angle
// ����˵��:  ������������Ƕ�
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: double
// ��    ��:Vec4f eyePoints��������
//************************************
double PoseEyesAngle(Vec4f eyePoints)
{
	return abs( atan2((eyePoints[3]-eyePoints[1]), (eyePoints[2]-eyePoints[0])) );	//atan(y,x)���ػ���
}

//************************************
// ��������: zbrightness_middle_pixel
// ����˵��:  �����м�80%�����������ĻҶȷ�Χ���ܵĻҶȷ�Χ��80%�ı���
// ��    ��: Juliwei
// �������: 2014/08/07
// �� �� ֵ: double
// ��    ��:Mat image��ͨ��
//************************************
//�����м�80%�����������ĻҶȷ�Χ���ܵĻҶȷ�Χ��80%�ı���
double BrightnessMiddlePixel(Mat image)
{
	//����ֱ��ͼ
	Mat imageHist = ComputeHist(image);	//������
	//���ֱ��ͼ
	//FileStorage fs("hist.xml", FileStorage::WRITE);
	//fs<<"Hist"<<imageHist;
	//fs.release();
	int sum = 0;
	double lowRate = 0.1, upRate = 0.1;	//�����޵ı�����ֵ
	int low=0, up=255;	//������
	int pixelNum = image.rows*image.cols;
	int minLoc, maxLoc;

	for (int i = 0; i < 255; i++)	//����С�Ҷ�
	{
		int temp = imageHist.at<float>(i);
		if (temp > 5)	//�ų�������Ŀ��С�ĻҶ�
		{
			minLoc = i;
			break;
		}
	}
	for (int j = 255; j > 0; j--)	//���Ҷ�
	{
		if (imageHist.at<float>(j) > 5)
		{
			maxLoc = j;
			break;
		}
	}

	for (int i = minLoc; i < maxLoc; i++)	//������
	{
		sum += imageHist.at<float>(i);
		if (sum > lowRate*pixelNum)
		{
			low = i;
			break;
		}
	}
	sum = 0;
	for (int j = maxLoc; j > minLoc; j--)	//����
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
// ��������: BrightnessEqualize
// ����˵��:  ֱ��ͼ����֮����ԭͼ�ľ�����
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: double
// ��    ��:Mat image�Ҷ�ͼ
//************************************
double BrightnessEqualize(Mat image)
{
	int flag = 1;	//0-����ͼ��ԭͼ�Ƚϣ�0-�ֿ�������������Ƚ�
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
// ��������: BrightnessY
// ����˵��:  ��YUV��Y�����ľ�ֵ����
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: double
// ��    ��:Mat image �����ǲ�ͼ
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
// ��������: BrightnessDarkSpot
// ����˵��:  ����ͼƬ����Ӱ���ص����
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: double
// ��    ��:Mat image �Ҷ�ͼ
// ��    ��:double alpha  С��alpha*maxValue���㰵����
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
//************************************
// ��������: computerLightQuality
// ����˵��:  ����ͼƬ��������,�ֵ�ͨ����3ͨ��
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: LightQuality
// ��    ��:Mat image��������ͼƬ
//************************************
LightQuality ComputerLightQuality(Mat image)
{
	LightQuality lightQuality;
	if (image.channels() == 1)
	{
		lightQuality.dark_spot = BrightnessDarkSpot(image, 0.3);
		lightQuality.equalization = BrightnessEqualize(image);
		lightQuality.y = 0;
		lightQuality.middle_pixel = BrightnessMiddlePixel(image);	//ʵ����80%������ռ�Ҷ�ֵ
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
// ��������: computerPoseQuality
// ����˵��:  ����ͼƬ��̬����
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: PoseQuality
// ��    ��:Vec4f eyePoints���������
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
// ��������: computerSymmetryQuality
// ����˵��: ����ͼƬ�Գ��� 
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: LightQuality
// ��    ��:Mat image
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


