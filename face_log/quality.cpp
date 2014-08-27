#include "stdafx.h"
#include "quality.h"



string cascadeName = "cascade\\haarcascade_frontalface_alt.xml";  
string nestedCascadeName = "cascade\\haarcascade_eye_tree_eyeglasses";
//计算PSNR

//计算两个矩阵协方差
double zmse(Mat image1, Mat image2)
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
double zPSNR(Mat im1,Mat im2)
{
	//IplImage* img1 = &IplImage(im1);
	//IplImage* img2 = &IplImage(im2);
	//IplImage* dst = cvCreateImage( cvGetSize(img1), 8, 1);
	//cvAbsDiff(img1,img2,dst);
	//CvScalar stddev;
	//cvAvgSdv(dst,NULL,&stddev);
	//double psnr=20*log10(255/stddev.val[0]);
	//cvReleaseImage(&dst);
	//cvReleaseImage(&img1);
	//cvReleaseImage(&img2);
	double dmse = zmse(im1, im2);
	if (dmse < 1e-10)
	{
		return 0;
	} 
	else
	{
		double psnr = 10.0 * log10((255 * 255) / dmse);
		return psnr;
	}
	
}

//方差
double zstdDev(Mat src, Mat dst)
{
	Scalar std_dev = 0;
	Mat mean;
	meanStdDev(src, mean, std_dev);
	return (double)std_dev[0];
}


//计算直方图
Mat zcomputeHist(Mat src)
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


//人脸检测

void detectAndDraw( Mat& img, vector<Mat>& vfaces)  
{ 
	CascadeClassifier cascade; 
	if( !cascade.load( cascadeName ) )  
	{  
		string error_message = "分类器导入失败";
		CV_Error(CV_StsBadArg, error_message);  
	}  
	int i = 0;  
	double t = 0;  
	vector<Rect> faces;  
	const static Scalar colors[] =  { CV_RGB(0,0,255),  
		CV_RGB(0,128,255),  
		CV_RGB(0,255,255),  
		CV_RGB(0,255,0),  
		CV_RGB(255,128,0),  
		CV_RGB(255,255,0),  
		CV_RGB(255,0,0),  
		CV_RGB(255,0,255)} ;  
	Mat gray;  
	Mat frame( cvRound(img.rows), cvRound(img.cols), CV_8UC1 );  

	cvtColor( img, gray, CV_BGR2GRAY );  
	resize( gray, frame, frame.size(), 0, 0, INTER_LINEAR );  
	equalizeHist( frame, frame );  

	//t = (double)cvGetTickCount();  
	cascade.detectMultiScale( frame, faces,  
		1.1, 2, 0  
		//|CV_HAAR_FIND_BIGGEST_OBJECT  
		//|CV_HAAR_DO_ROUGH_SEARCH  
		|CV_HAAR_SCALE_IMAGE  
		,  
		Size(30, 30) );  
	//t = (double)cvGetTickCount() - t;  
	//printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );  

	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )  
	{  
		//画圈
/*		Point center;  
		Scalar color = colors[i%8];  
		int radius;  
		center.x = cvRound(r->x + r->width*0.5);  
		center.y = cvRound(r->y + r->height*0.5);  
		radius = (int)(cvRound(r->width + r->height)*0.25);  
		circle( img, center, radius, color, 3, 8, 0 ); */ 

		vfaces.push_back(img(*r));	//获得矩形人脸区域

	}  

	//imshow( "result", img );  
	//waitKey(0);
} 



//对比度
double zc0(Mat image)
{
	//Mat image0 = Mat::zeros(image.rows, image.cols,CV_32F);
	Scalar m0 = mean(image);	//均值
	absdiff(image, m0[0], image);
	pow(image, 2, image);
	double contrast = 0;
	for (int i = 0; i < image.rows; i++)
		for (int j =0; j < image.cols; j++)
		{
			contrast +=(double)image.at<uchar>(i, j);
		}
	contrast/= (image.rows * image.cols);
	contrast = std::pow(contrast, 0.5);
	return contrast*m0[0];

}


//人脸区域占图片比例
double zf1(Mat image, Mat face)
{
	return (double)face.rows * face.cols / (double)(image.rows * image.cols);

}

//梯度表清晰度
double zcl1(Mat image)
{
	double sum = 0;
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

//直方图求对比度
double zc1(Mat image)
{
	//double contrast = 0.0;
	Mat hist = zcomputeHist(image);
	Scalar mean, dev;
	meanStdDev(image, mean, dev);
	return 1.0 - dev[0];
}


//图像序列求明亮度
double zlight(Mat image, vector<Mat> images)
{
	Mat mmean = Mat::zeros(images.size(), 1, CV_32FC1);
	for (vector<Mat>::size_type i = 0; i < images.size(); i++)
	{
		Scalar smean = mean(images[i]);
		mmean.at<float>(i, 0) = smean[0];
	}
	Scalar smean0 = mean(mmean);
	Scalar smean1 = mean(image);
	double light = abs((double)smean1[0] - (double)smean0[0]);
	light =1.0 - light / 255.0;
	return light;
}


//DCT计算清晰度
double zsharpness1(Mat image, double belta)
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


//直方图衡量左右脸对称性
double zsymmetry_hist(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	//imshow("left", image0);
	//imshow("right", image1);
	Mat hist0 = zcomputeHist(image0);
	Mat hist1 = zcomputeHist(image1);
	return compareHist(hist0, hist1, CV_COMP_CHISQR);//CV_COMP_CORREL; CV_COMP_CHISQR; CV_COMP_INTERSECT; CV_COMP_BHATTACHARYYA
}


//计算gabor
Mat zcomputeGabor(Mat image)
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
double zsymmetry_gabor(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	image0 = zcomputeGabor(image0);
	image1 = zcomputeGabor(image1);
	absdiff(image0, image1, image1);
	Scalar ssum = sum(image1);
	return ssum[0];
}



//LBPH衡量对称性
double zsymmetry_lbph(Mat image)
{
	Mat image0 = image(Range::all(), Range(0, image.cols/2));
	Mat image1 = image(Range::all(), Range(image.cols/2, image.cols));
	Mat hist0 = ComputerLBPH(image0);
	Mat hist1 = ComputerLBPH(image1);
	return compareLBPH(hist0, hist1);
	
}

//暗像素比例衡量明亮度
double zbrightness_dark_spot(Mat image, double alpha)
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

//获取黑色通道
Mat zdarkChannel(const Mat image,int patch)
{
	//定位图片有效像素，排除边界
	//Mat image1 = image.clone();
	int lenght = patch / 2;
	//imshow("image", image);
	//waitKey(0);
	//copyMakeBorder(image, image1, patch/2,patch/2, patch/2, patch/2, BORDER_REPLICATE, Scalar(0, 0, 0));
	Mat dst = Mat::zeros(image.size(), image.type());
	for(int i=lenght; i<image.rows-lenght; i++)
		for (int j=lenght; j<image.cols-lenght; j++)
		{
			int iMin = INT_MAX;
			//int iMax = 0;
			//对patch的邻居像素取最小
			for(int m=-lenght; m<lenght; m++)
				for (int n=-lenght; n<lenght; n++)
				{
					if (image.at<UCHAR>(i+m, j+n) < iMin)
					{
						iMin = image.at<UCHAR>(i+m, j+n);
					}

					//if (image.at<UCHAR>(i+m, j+n) > iMax)
					//{
					//	iMax = image.at<UCHAR>(i+m, j+n);
					//}
				}
			dst.at<UCHAR>(i, j) = iMin;

			//dst.at<UCHAR>(i, j) = iMax;

		}
	return dst;
}

//获得彩图的dark channel 图像
Mat zdarkImage(Mat image)
{
	vector<Mat> plane;
	split(image, plane);
	for (size_t n=0; n<plane.size();n++)
	{
		plane[n] = zdarkChannel(plane[n], 10);

	}
	Mat dst = Mat::zeros(plane[0].size(), plane[0].type());
	for(int i=0; i < plane[0].rows; i++)
		for(int j=0; j < plane[0].cols; j++)
		{
			int iMin2=INT_MAX;
			//int iMax2 = 0;
			for (size_t n=0; n<plane.size();n++)
			{
				int cc = plane[n].at<UCHAR>(i, j);
				if (plane[n].at<UCHAR>(i, j)<iMin2)
				{
					iMin2 = plane[n].at<UCHAR>(i, j);
				}
			}
			dst.at<UCHAR>(i, j) = iMin2;

			//	if (plane[n].at<UCHAR>(i, j) > iMax2)
			//	{
			//		iMax2 = plane[n].at<UCHAR>(i, j);
			//	}
			//}
			//dst.at<UCHAR>(i, j) = iMax2;
		}
	return dst;
}


//YUV中Y分量衡量明亮度
double zbrightness_Y(Mat image)
{
	Mat temp;
	vector<Mat> dst;
	cvtColor(image, temp, CV_BGR2YUV);
	split(temp, dst);
	Scalar scmean = mean(dst[0]);
	return (double)scmean[0];
}

//计算暗通道的均值
double zbrightness_dark(Mat image)
{
	Mat temp = DarkImage(image, 10, 0);//核为10， 取最小
	CV_Assert(temp.channels() == 1);
	Scalar scmean = mean(temp);
	return (double)scmean[0];
}

//直方图均衡之后与原图的均方差
double zbrightness_equalize(Mat image)
{
	int flag = 1;	//0-均衡图与原图比较；0-分块均衡与整体均衡比较
	double dmse;
	CV_Assert(image.channels() == 1);
	Mat imageHist = image.clone();
	Mat imageHist_block = image.clone();
	equalizeHist(image , imageHist);
	if (flag ==0)
	{
		dmse = zmse(image, imageHist);
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
		dmse = zmse(image, imageHist_block);
	}
	//imshow("equalize", imageHist_block);
	//waitKey(0);
	return dmse;
}

//计算中间80%的像素数量的灰度范围和总的灰度范围的80%的比例
double zbrightness_middle_pixel(Mat image)
{
	//计算直方图
	Mat imageHist = zcomputeHist(image);	//行向量
	//输出直方图
	FileStorage fs("hist.xml", FileStorage::WRITE);
	fs<<"Hist"<<imageHist;
	fs.release();
	int sum = 0;
	double lowRate = 0.1, upRate = 0.1;	//上下限的比例阈值
	int low=0, up=255;	//上下限
	int pixelNum = image.rows*image.cols;
	int minLoc, maxLoc;

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

//计算两眼坐标角度
double zpose_eyes_angle(Vec4f eyePoints)
{
	return abs( atan2((eyePoints[3]-eyePoints[1]), (eyePoints[2]-eyePoints[0])) );	//atan(y,x)返回弧度
}

//计算两眼坐标的距离
double zpose_eyes_length(Vec4f eyePoints)
{
	return abs(eyePoints[3]-eyePoints[1])*abs(eyePoints[3]-eyePoints[1]) + abs(eyePoints[2]-eyePoints[0])*abs(eyePoints[2]-eyePoints[0]);	
}