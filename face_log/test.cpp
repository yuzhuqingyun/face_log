#include "stdafx.h"
#include "test.h"
#include "zlib.h"
#include "quality.h"
#include "sqi.h"



//string cascadeName =
//"haarcascades/haarcascade_frontalface_alt.xml";   //使用的预先训练好的检测器人前脸检测
//string nestedCascadeName =
//"haarcascades/haarcascade_eye_tree_eyeglasses.xml";  //预先训练好的眼睛和眼睛框检测

//人脸检测
//int detectFace( Mat& img, CascadeClassifier& cascade);
//获得文件夹下所有视频的路径
int getPaths(string sdirectory, vector<string>& vpaths);
//播放视频
int video(string path);

void test()
{
	//获取视频路径
	vector<string> vpaths;
	getPaths("G:\\Video\\NRC-IIT", vpaths);
	for (vector<string>::size_type i = 0; i < vpaths.size(); i++)
	{
		video(vpaths[i]);
	}
}


//获得文件夹下所有视频的路径
int getPaths(string sdirectory, vector<string>& vpaths)
{
	Directory dir;
	vpaths = dir.GetListFilesR(sdirectory, "*.*");
	return 1;
}


int video(string path)
{

	//打开视频文件
	VideoCapture capture(path);
	//检查视频是否打开
	if(!capture.isOpened())
		return 1;

	// 得到帧率
	double rate= capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	Mat frame; // 现在的视频帧
	namedWindow("Extracted Frame");

	// 两帧之间的间隔时间
	int delay= 1000/rate;
	// 循环播放所有的帧
	while (!stop) {
		// 读下一帧
		if (!capture.read(frame))
			break;

		//TODO:预处理


		//TODO:质量评估
		vector<Mat> vfaces;
		detectAndDraw(frame, vfaces); 

		//在窗口中显示图像
		imshow("Extracted Frame",frame);
		// 按Esc键停止视频播放
		if (waitKey(delay) == 27)
			stop= true;
		waitKey(20);
	}
	// 关闭视频文件
	capture.release();
	return 0;
}



void showSymmetrical(string path, Mat& picture, Scalar color)
{
	//string test_path = "G:\\Picture\\pel\\good";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (vector<string>::size_type i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 0));
	}
	for (size_t m=0; m<test.size();m++)
	{
		//尺度归一化
		resize(test[m], test[m], Size(200, 200));
		//高斯平滑
		GaussianBlur(test[m], test[m],Size(3, 3), 0.5, 0.5);
	}
	//Mat picture = Mat::zeros(260, test.size()*11, CV_8UC1);
	Mat symmetric(test.size(), 1, CV_32FC1/*,Scalar(0,0,0)*/);
	for (vector<Mat>::size_type i = 0; i < test.size(); i++)
	{
		float cc = zsymmetry_hist(test[i]);
		symmetric.at<float>(i, 0) = cc;
		char txt[50];
		sprintf(txt, "%f", cc);
		putText(test[i], txt, Point( 10,10),CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 0, 0) );
		//imshow("distance", test[i] );
		char savePath[50];
		sprintf(savePath, "%s%i%s%f%s", "G:\\Picture\\result\\", i, "-", cc, ".bmp");
		imwrite(savePath, test[i]);
		//waitKey(500);
	}
	/*FileStorage fs("output.xml", FileStorage::WRITE);
	fs<<"output"<<symmetric;
	fs.release();*/
	//normalize(symmetric,symmetric,0,255,NORM_MINMAX);
	//symmetric.convertTo(symmetric, CV_8UC1);
	//symmetric = symmetric;
	//for (int i = 0; i<symmetric.rows; i++)
	//{
	//	Point center = Point(i*10,symmetric.at<float>(i, 0));
	//	//半径
	//	int r = 2;
	//	//承载图像
	//	//参数为：承载的图像、圆心、半径、颜色、粗细、线型
	//	circle(picture,center,r,color);
	//}
	//imshow("底板",picture);
	//waitKey(0);

}

void testSymmetrical()
{
	string good_path = "G:\\Picture\\quality\\PEL";/*temp3*/
	//string good_path = "G:\\Picture\\pel\\good";
	//string bad_path = "G:\\Picture\\pel\\bad";
	Mat picture = Mat::zeros(2000, 320*11, CV_8UC3);
	showSymmetrical(good_path, picture, Scalar(255,0,0));
	//showSymmetrical(bad_path, picture, Scalar(0,0,255));
	imshow("SHOW", picture);
	waitKey(500);
}

void testBlur()
{
	string path = "G:\\Picture\\quality\\AI&R";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (vector<string>::size_type i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 0));
	}
	int num = 0;
	for (vector<Mat>::size_type m = 0; m < test.size(); m++) 
	{
		//高斯模糊
		for (int i = 1; i <= 41; i+=8)
		{
			char cpath[50];
			sprintf(cpath, "%s%i%s%i%s","G:\\Picture\\quality\\AI&R\\GaussianBlur-", m, "-", i, ".bmp");
			//string save_path = "GaussianBlur";
			Mat dst1 = test[m].clone();
			//blur(src, dst1, Size(i, i));
			GaussianBlur(test[m], dst1, Size(i, i), 0, 0);
			//medianBlur(src, dst3, i);
			//bilateralFilter(src, dst4, i, 2*i, i/2);
	
			//imshow("src", src);
			//imshow("blur", dst1);
			imwrite(cpath, dst1);
			//imshow("medianBlur", dst3);
			//imshow("bilateralFilter", dst4);
			waitKey(0);
		}

	}
}

//截取人脸
void GetFace()
{
	string path = "G:\\Picture\\pel";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 1));
	}
	int num = 0;
	for (size_t m = 0; m < test.size(); m++) 
	{
		vector<Mat> vface;
		detectAndDraw(test[m], vface);
		for (size_t n=0; n<vface.size(); n++)
		{
			char cpath[50];
			sprintf(cpath, "%s%i%s%i%s","G:\\Picture\\quality\\temp\\face-", m, "-", n, ".bmp");
			imwrite(cpath, vface[n]);
		}
	}
}

//canny边缘
void GetCanny()
{
	string path = "G:\\Picture\\quality\\PEL";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 0));
	}
	//canny
	//Mat src, src_gray;  
	Mat /*dst,*/ detected_edges;  
	//int edgeThresh = 1;  
	int lowThreshold;  
	int const max_lowThreshold = 100;  
	int ratio = 3;  
	int kernel_size = 3;  
	char* window_name = "Edge Map"; 
	for (size_t j=0; j<test.size(); j++)
	{
		lowThreshold = 0;
		while(waitKey(500) != 27)
		{
			if (lowThreshold < max_lowThreshold)
			{
				blur( test[j], detected_edges, Size(3,3) );  
				/// Canny detector  
				Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );  
				//dst = Scalar::all(0);  
				//src.copyTo( dst, detected_edges);  
				imshow( window_name, detected_edges );
				imshow("src",  test[j]);
				lowThreshold+=10;
			}
			else break;

		}
	}
}


//通过比较与平滑过后的图片衡量清晰度
double zsharpness(Mat image)
{
	Mat blur_image;
	int i = 3;
	//blur(src, blur_image, Size(i, i));
	//GaussianBlur(image, blur_image, Size(i, i), 0, 0);
	medianBlur(image, blur_image, i);
	//bilateralFilter(image, blur_image, i, 2*i, i/2);
	absdiff(image, blur_image, blur_image);
	Scalar scmean = mean(blur_image);
	return scmean[0];
}
//清晰度
void testSharpness()
{
	string path = "G:\\Picture\\quality\\AI&R";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 0));
	}
	for (size_t m = 0; m < test.size(); m++) 
	{
		double dsharpness = zsharpness1(test[m], 0.5);

		char txt[50];
		sprintf(txt, "%f", dsharpness);
		putText(test[m], txt, Point( 10,10),CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 0, 0) );

		char cpath[50];
		sprintf(cpath, "%s%i%s%f%s","G:\\Picture\\result\\temp\\", m, "-", dsharpness, ".bmp");
		imwrite(cpath, test[m]);

	}
}

//评估光照用darkchannel
void testBrightness1()
{
	string path = "G:\\Picture\\jiaodaku\\原始图";
	//string path = "G:\\Picture\\quality\\temp3";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 1));
	}
	for (size_t m = 0; m < test.size(); m++) 
	{
		//vector<Mat> plane;
		//split(test[m], plane);
		//for (size_t n=0; n<plane.size();n++)
		//{
		//	plane[n] = zdarkChannel(plane[n], 10);
		//	//imshow("dark", plane[n]);
		//	//waitKey(0);
		//}
		//Mat dst = Mat::zeros(plane[0].size(), plane[0].type());
		//for(int i=0; i < plane[0].rows; i++)
		//	for(int j=0; j < plane[0].cols; j++)
		//	{
		//		int iMin2=INT_MAX;
		//		for (size_t n=0; n<plane.size();n++)
		//		{
		//			int cc = plane[n].at<UCHAR>(i, j);
		//			if (plane[n].at<UCHAR>(i, j)<iMin2)
		//			{
		//				iMin2 = plane[n].at<UCHAR>(i, j);
		//			}
		//		}
		//		dst.at<UCHAR>(i, j) = iMin2;
		//	}

		Mat dst = zdarkImage(test[m]);
		Mat src, abs;
		cvtColor(test[m], src, CV_BGR2GRAY);
		absdiff(src, dst, abs);
		//保存图片
	/*	char cpath0[50];
		sprintf(cpath0, "%s%i%s%s%s","G:\\Picture\\result\\temp\\", m, "-", "src", ".bmp");
		imwrite(cpath0, src);
		char cpath1[50];
		sprintf(cpath1, "%s%i%s%s%s","G:\\Picture\\result\\temp\\", m, "-", "dark", ".bmp");
		imwrite(cpath1, dst);
		char cpath2[50];
		sprintf(cpath2, "%s%i%s%s%s","G:\\Picture\\result\\temp\\", m, "-", "abs", ".bmp");
		imwrite(cpath2, abs);*/

		imshow("src", src);
		imshow("dst", dst);
		imshow("abs", abs);

		waitKey(500);
	}
}

//评估光照
void testBrightness()
{
	string path = "G:\\Picture\\quality\\temp3";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 0));
	}
	for (size_t m = 0; m < test.size(); m++) 
	{
		double bb = zbrightness_equalize(test[m]);
		char txt[50];
		sprintf(txt, "%f", bb);
		putText(test[m], txt, Point( 10,10),CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 0, 0) );

		char cpath[50];
		sprintf(cpath, "%s%i%s%f%s","G:\\Picture\\result\\temp\\", m, "-", bb, ".bmp");
		imwrite(cpath, test[m]);
	}
}

void testSQI()
{
	//string path = "C:\\Users\\Juliet\\Desktop\\1";
	string path = "G:\\Picture\\quality\\temp2";
	//string path = "G:\\Picture\\右脸颊";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 1));
	}
	for (size_t m = 0; m < test.size(); m++) 
	{
		blur(test[m],test[m],Size(3,3));	//模糊去噪

		
		Mat src/*, abs0, abs1, showImage*/;
		Mat contourImage0, contourImage1;
		Mat diffImg0, diffImg1;
		cvtColor(test[m], src, CV_BGR2GRAY);
		//src = zSetDark(src, 0.1);	//将低于一定数值的像素置0
		//取暗、亮通道
		Mat dst0 = DarkImage(src, 4, 0);
		dst0 = DarkImage(dst0, 4, 0);
		dst0 = DarkImage(dst0, 4, 1);
		//Mat edgeImage0 = zFindEdge(dst0, 3, 2);
		//dilate(edgeImage0, edgeImage0, Mat(), Point(-1, -1), 1);
		//Mat blob0 = zFindBlobs(src);
		//Mat blob1 = zFindBlobs(src);

		//Mat dst1 = DarkImage(src, 3, 1);
		//取差图像
		//absdiff(src, dst0, abs0);
		//absdiff(dst0, dst1, abs1);
		//边缘检测
		Mat edgeImage0 = zFindEdge(dst0, 3, 2);
		//形态学处理
		dilate(edgeImage0, edgeImage0, Mat(), Point(-1, -1), 1);
		erode(edgeImage0, edgeImage0, Mat(), Point(-1, -1), 1);

		contourImage0 = zFindContours(edgeImage0);
		cvtColor(contourImage0, contourImage0, CV_BGR2GRAY);
		absdiff(src, contourImage0, diffImg0);

		//Mat edgeImage1 = zFindEdge(src, 3, 2);
		////contourImage1 = zFindContours(edgeImage1);
		////cvtColor(contourImage1, contourImage1, CV_BGR2GRAY);
		//subtract(src, edgeImage1, diffImg1);
		//阈值分割
		//for(int i=0; i<abs0.rows; i++)
		//	for (int j=0; j<abs0.cols; j++)
		//	{
		//		//对dark通道直接阈值分割
		//		if (dst0.at<uchar>(i, j) < 50)
		//		{
		//			abs0.at<uchar>(i, j)=255;
		//		}
		//		else
		//		{
		//			abs0.at<uchar>(i, j)=0;
		//		}
		//		////阈值分割
		//		//if (abs0.at<uchar>(i, j) >20 )
		//		//{
		//		//	abs0.at<uchar>(i, j) = 255;
		//		//} 
		//		//else
		//		//{
		//		//	abs0.at<uchar>(i, j) = 0;

		//		//}
		//	}
		//Mat dst1 = DarkImage(test[m], 10, 1);
		//cvtColor(test[m], src, CV_BGR2GRAY);
		//absdiff(test[m], dst0, abs0);
		//absdiff(src, dst1, abs1);
		//imshow("src", test[m]);
		//imshow("dst0", dst0);
		////imshow("dst1", dst1);
		//imshow("abs0", abs0);
		////imshow("abs1", abs1);
		//waitKey(0);

		//Mat logDst0 = Mat::zeros(dst0.size(), CV_32F);
		//Mat logSrc= Mat::zeros(test[m].size(), CV_32F);
		//dst0.convertTo(logDst0, CV_32F);
		//test[m].convertTo(logSrc, CV_32F);
		//log(logDst0, logDst0);
		//log(logSrc, logSrc);
		//absdiff(logDst0, logSrc, abs);

		//exp(logSrc, showImage);
		//divide(test[m], dst0, showImage);
		//divide(test[m], abs, abs);
		//normalize(showImage, showImage, 0, 255, NORM_MINMAX);

		//ChannelDiff( test[m]);
		//Mat showImage = DogFilter(test[m], Size(3, 3));
		//imshow("showImage", showImage);
		//imshow("srcImage", test[m]);
		//waitKey(0);

		//保存图片
		//char cpath0[50];
		//sprintf(cpath0, "%s%i%s%s%s","G:\\Picture\\result\\temp\\", m, "-", "src", ".bmp");
		//imwrite(cpath0, test[m]);
		//char cpath1[50];
		//sprintf(cpath1, "%s%i%s%s%s","G:\\Picture\\result\\temp\\", m, "-", "dark", ".bmp");
		//imwrite(cpath1, dst0);
		char cpath2[50];
		sprintf(cpath2, "%s%i%s%s%s","G:\\Picture\\result\\temp\\", m, "-", "dark-contourImage", ".bmp");
		imwrite(cpath2, diffImg0);

		//imshow("src", src);
		//imshow("showImage", showImage);
		//imshow("abs", abs);

		//waitKey(0);
	}
	AfxMessageBox("分割斑点结束！");
}

//寻找轮廓
Mat zFindContours(Mat image)
{
	//Mat src;
	// the first command-line parameter must be a filename of the binary (black-n-white) image
	Mat dst = Mat::zeros(image.rows, image.cols, CV_8UC3);
	//src = src > 50;
	//namedWindow( "Source", 1 );
	//imshow( "Source", image );
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( image, contours, hierarchy, CV_RETR_CCOMP , CV_CHAIN_APPROX_NONE );
	// iterate through all the top-level contours,draw each connected component with its own random color
	int idx = 0;
	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		//除去面积过大或过小的
		if (fabs(contourArea(contours[idx])) > 20 && fabs(contourArea(contours[idx])) < 200)
		{
			//除去周长太大的
			if (contours[idx].size()<100)
			{

				Scalar color( rand()&255, rand()&255, rand()&255 );
				drawContours( dst, contours, idx, Scalar::all(255), CV_FILLED, 8, hierarchy );
			}
		}
	}
	//for(int i=0;i<(int)contours.size();i++)//将有符号的contours.size()强制转换位非符号的；
	//{
	//	if(fabs(contourArea(contours[i])) < 10);
	//	//cout<<"area "<<i<<" ="<<area.at<float>(i)<<endl;
	//	drawContours(dst,contours,-1,Scalar(0),2);
	//}
	//namedWindow( "Components", 1 );
	//imshow( "Components", dst );
	//waitKey(0);
	return dst;
}

//sobel、Laplacion、canny
Mat zFindEdge(Mat image, int kernel_size, int flag)
{
	Mat dst;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S; 
	int borderType = BORDER_DEFAULT;
	int lowThreshold = 20;  
	//int const max_lowThreshold = 100;  
	int ratio = 3; 
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	switch (flag)
	{
	//sobel
	case 0:
		/// Generate grad_x and grad_y

		/// Gradient X
		//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
		//Calculates the first, second, third, or mixed image derivatives using an extended Sobel operator.
		Sobel( image, grad_x, ddepth, 1, 0, kernel_size, scale, delta, borderType );   
		convertScaleAbs( grad_x, abs_grad_x );
		/// Gradient Y  
		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
		Sobel( image, grad_y, ddepth, 0, 1, 3, scale, delta, borderType );   
		convertScaleAbs( grad_y, abs_grad_y );
		/// Total Gradient (approximate)
		addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst );
		break;
	//laplacion
	case 1:
		Laplacian( image, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
		convertScaleAbs( dst, dst );//返回绝对值
		break;
	//canny
	case 2: 
		//Mat detected_edges;  
		//int edgeThresh = 1;  
  
		Canny( image, grad_x, lowThreshold, lowThreshold*ratio, kernel_size );  
		dst = Scalar::all(0);  
		image.copyTo( dst, grad_x);  
		break;
	default:
		break;
	}
	return dst;
}



//块检测
Mat zFindBlobs(Mat image)
{
	Mat descriptors;  
	vector<KeyPoint> keypoints;  
	SimpleBlobDetector::Params params;

	//params.minThreshold = 10;  
	//params.maxThreshold = 100;  
	//params.thresholdStep = 10; 
	//params.minDistBetweenBlobs = 1.0;	//blob间最小距离

	params.filterByColor = true;	//颜色
	params.blobColor = 0;	//0表dark；255表light

	params.filterByArea = true;	//面积
	params.minArea = 6.0;   //最小面积
	params.maxArea = 100.0;	//最大面积

	//params.filterByCircularity = true;  
	//params.minCircularity = 0.5;
	//params.maxCircularity = 1;

	//params.filterByInertia = true;	//惯性张量
	//params.minInertiaRatio = 0.01; 
	//params.maxInertiaRatio = 0.1; 

	//params.filterByConvexity = true;	//凸面
	//params.minConvexity = 0.1;  
	//params.maxConvexity = 2.0;  
   
	SimpleBlobDetector blobDetector( params );  
	blobDetector.create("SimpleBlob");  
	blobDetector.detect( image, keypoints );  
	drawKeypoints(image, keypoints, image, Scalar(255,0,0));  
	return image;
}

Mat zSetDark(Mat image, double alpha)
{
	double minValue, maxValue;
	Mat dst = image.clone();
	minMaxLoc(dst, &minValue, &maxValue);
	int threshold = (int)(abs(maxValue - minValue) * alpha);
	for(int i=0; i<dst.rows;i++)
		for (int j=0; j<dst.cols; j++)
		{
			if (dst.at<UCHAR>(i, j) < threshold)
			{
				dst.at<UCHAR>(i, j) = 0;
			}
		}
	//imshow("setDark", dst);
	return dst;
}

//形态学处理
Mat zMorphology(Mat image, int flag)
{
	 int morph_elem = 0;	//MORPH_RECT MORPH_ELLIPSE MORPH_CROSS 
	 int morph_size = 1;	//核大小
	 Mat dst;
	 Mat element = getStructuringElement(morph_elem, Size(2*morph_size+1, 2*morph_size+1), Point(morph_size, morph_size));
	 morphologyEx(image, dst, flag+2, element);
	 return dst;
}

//读取读入中科院说明文本内容txt
void read_txt(const string& filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator)
{
	ifstream file(filename.c_str(), ifstream::in);
	if (!file) 
	{
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, pt0, pt1, pt2, pt3;
	while (getline(file, line)) 
	{
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, pt0, separator);
		getline(liness, pt1, separator);
		getline(liness, pt2, separator);
		getline(liness, pt3, separator);
		if(!path.empty() && !pt0.empty()&& !pt1.empty()&& !pt2.empty()&& !pt3.empty()) 
		{
			Vec4f vtemp;
			vtemp[0] = atof(pt0.c_str());
			vtemp[1] = atof(pt1.c_str());
			vtemp[2] = atof(pt2.c_str());
			vtemp[3] = atof(pt3.c_str());
			vpaths.push_back(path);
			vpoints.push_back(vtemp);
		}
	}
}

//从文件名中提取信息id和光照的标号
void read_filename(const string& filename, int& id, string& sign)
{	
	id = atoi(filename.substr(3, 6).c_str());
	sign = filename.substr(17, 5);	//光照标志(11, 5)；姿态(17, 5)
}

//测试中科院光照子库
void test_cas_light()
{
	int lightFlag = 1;	//选择光照指标排序
	int saveFlag = 1;	//0显示， 1写入
	string filename = "G:\\Picture\\Lighting\\FaceFP_2.txt";	//txt路径
	string savePath;	//保存路径

	vector<Face> vfaces;
	vector<LightQuality> vlightQuality;
	getFaceFromTxt(filename, vfaces);	//从txt获取人脸信息
	for (size_t i=0; i<vfaces.size(); i++)//计算光照质量
	{
		Mat image = vfaces[i].face.clone();
		LightQuality lightQuality = computerLightQuality(image);
		vlightQuality.push_back(lightQuality);
		//参数为：承载的图片，插入的文字，文字的位置（文本框左下角），字体，大小，颜色
		//char words[50];
		//sprintf(words, "%4.2f%s%4.2f%s%4.2f%s%4.2f", lightQuality.dark_spot, "_", lightQuality.equalization, "_",lightQuality.y, "_", lightQuality.dark_channel);
		//putText( image, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
		//imshow("image", image);
		//waitKey(0);
	}
	//选择排序的指标
	for (size_t j=0; j<vfaces.size(); j++)
	{
		switch (lightFlag)
		{
			case 0:
				vfaces[j].value = vlightQuality[j].dark_spot;
				break;
			case 1:
				vfaces[j].value = vlightQuality[j].equalization;
				break;
			case 2:
				vfaces[j].value = vlightQuality[j].dark_channel;
				break;
			case 3:
				vfaces[j].value = vlightQuality[j].y;
				break;
		}

	}
	sort(vfaces.begin(), vfaces.end(), less<Face>());	//按指标value升序排列
	for (size_t m=0; m<vfaces.size(); m++)
	{
		savePath = "G:\\Picture\\result\\temp\\";
		Mat image0 = vfaces[m].face.clone();
		char words[50];
		if (saveFlag == 1)	//保存
		{
			sprintf(words, "%d%s%4.2f%s%s%s", vfaces[m].id, "_", vfaces[m].value, "_", vfaces[m].sign.c_str(), ".jpg");
			savePath += string(words);
			imwrite(savePath, image0);
		} 
		else				//显示
		{
			sprintf(words, "%s%4.2f", "dark_spot: ", vfaces[m].value);
			putText( image0, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
			imshow("image0", image0);
			waitKey(0);
		}		

	}
}

//测试中科院角度子库
void test_cas_pose()
{
	int poseFlag = 0;	//选择角度指标排序
	int saveFlag = 1;	//0显示， 1写入
	string filename = "G:\\Picture\\pose\\FaceFP_2.txt";	//txt路径
	string savePath;	//保存路径

	vector<Face> vfaces;
	vector<PoseQuality> vposeQuality;
	getFaceFromTxt(filename, vfaces);	//从txt获取人脸信息
	for (size_t i=0; i<vfaces.size(); i++)//计算光照质量
	{
		PoseQuality poseQuality = computerPoseQuality(vfaces[i].eyePoints);
		vposeQuality.push_back(poseQuality);
		//参数为：承载的图片，插入的文字，文字的位置（文本框左下角），字体，大小，颜色
		//char words[50];
		//sprintf(words, "%4.2f%s%4.2f%s%4.2f%s%4.2f", lightQuality.dark_spot, "_", lightQuality.equalization, "_",lightQuality.y, "_", lightQuality.dark_channel);
		//putText( image, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
		//imshow("image", image);
		//waitKey(0);
	}
	//选择排序的指标
	for (size_t j=0; j<vfaces.size(); j++)
	{
		switch (poseFlag)
		{
		case 0:
			vfaces[j].value = vposeQuality[j].eyes_angle;
			break;
		case 1:
			vfaces[j].value = vposeQuality[j].eyes_length;
			break;
		}

	}
	sort(vfaces.begin(), vfaces.end(), less<Face>());	//按指标value升序排列
	for (size_t m=0; m<vfaces.size(); m++)
	{
		savePath = "G:\\Picture\\result\\temp\\";
		Mat image0 = vfaces[m].face.clone();
		char words[50];
		if (saveFlag == 1)	//保存
		{
			sprintf(words, "%d%s%4.2f%s%s%s", vfaces[m].id, "_", vfaces[m].value, "_", vfaces[m].sign.c_str(), ".jpg");
			savePath += string(words);
			imwrite(savePath, image0);
		} 
		else				//显示
		{
			sprintf(words, "%s%4.2f", "dark_spot: ", vfaces[m].value);
			putText( image0, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
			imshow("image0", image0);
			waitKey(0);
		}		

	}
}

//测试中科院光照和姿态子库对称性
void test_cas_symmetry()
{
	int symmetryFlag = 2;	//选择光照指标排序
	int saveFlag = 1;	//0显示， 1写入
	string directory = "G:\\Picture\\PEL";	//txt路径
	string savePath;	//保存路径
	int saveFace = 1;	//0-保存原始路径原始图，1-保存归一化人脸
	vector<Face> vfaces;
	vector<SymmetryQuality> vsymmetryQuality;
	getPelFace(directory, vfaces);	//从txt获取人脸信息
	for (size_t i=0; i<vfaces.size(); i++)//计算光照质量
	{
		Mat image = vfaces[i].face.clone();
		SymmetryQuality symmetryQuality = computerSymmetryQuality(image);
		vsymmetryQuality.push_back(symmetryQuality);
		//参数为：承载的图片，插入的文字，文字的位置（文本框左下角），字体，大小，颜色
		//char words[50];
		//sprintf(words, "%4.2f%s%4.2f%s%4.2f%s%4.2f", lightQuality.dark_spot, "_", lightQuality.equalization, "_",lightQuality.y, "_", lightQuality.dark_channel);
		//putText( image, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
		//imshow("image", image);
		//waitKey(0);
	}
	//选择排序的指标
	for (size_t j=0; j<vfaces.size(); j++)
	{
		switch (symmetryFlag)
		{
		case 0:
			vfaces[j].value = vsymmetryQuality[j].hist;
			break;
		case 1:
			vfaces[j].value = vsymmetryQuality[j].lbph;
			break;
		case 2:
			vfaces[j].value = vsymmetryQuality[j].gabor;
			break;
		}

	}
	sort(vfaces.begin(), vfaces.end(), less<Face>());	//按指标value升序排列
	for (size_t m=0; m<vfaces.size(); m++)
	{
		savePath = "G:\\Picture\\result\\temp\\";
		Mat image0;
		if (saveFace == 0)
		{
			string long_path = "G:\\Picture\\pose\\" + vfaces[m].filename.substr(0, vfaces[m].filename.size()-1) + ".jpg";	//分割得到的文件名多出一个空格，-1去除
			image0 = imread(long_path, 0);
		} 
		else
		{
			image0 = vfaces[m].face.clone();
		}

		char words[50];
		if (saveFlag == 1)	//保存
		{
#if 0	//cas光照姿态
			sprintf(words, "%d%s%4.2f%s%s%s", vfaces[m].id, "_", vfaces[m].value, "_", vfaces[m].sign.c_str(), ".jpg");
			savePath += string(words);
			imwrite(savePath, image0);
#else 1	//pel对称性
			sprintf(words, "%s%4.2f%s", "-", vfaces[m].value, ".jpg");
			string filename = getFilenameFromPath(vfaces[m].filename);
			savePath += filename+string(words);
			imwrite(savePath, image0);
#endif
		} 
		else				//显示
		{
			sprintf(words, "%s%4.2f", "dark_spot: ", vfaces[m].value);
			putText( image0, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
			imshow("image0", image0);
			waitKey(0);
		}		

	}
}


//从txt获取人脸信息
/*"G:\\Picture\\Lighting\\FaceFP_2.txt"*/
void getFaceFromTxt(const string& filename, vector<Face>& vfaces)
{
	string sign;
	int id;
	vector<string> vpaths;	//文件路径
	vector<Vec4f> vpoints;	//两眼坐标点
	vector<int> vids;		//人脸ID号
	vector<string> vsigns;	//光照标记
	vector<Mat> vmfaces;		//归一化人脸区域
	read_txt(filename, vpaths, vpoints, ' ');	//读入文件名和两眼坐标
	size_t maxSize = 1000;	///vpaths.size()
	for (size_t i = 0; i<maxSize; i++)	//分解文件名，得到ID和状态标志
	{
		string path = vpaths[i];
		read_filename(path, id, sign);
		vids.push_back(id);
		vsigns.push_back(sign);
	}
	for (size_t j = 0; j<maxSize; j++)
	{
		string long_path = "G:\\Picture\\pose\\" + vpaths[j].substr(0, vpaths[j].size()-1) + ".jpg";	//分割得到的文件名多出一个空格，-1去除
		Mat image = imread(long_path, 0);
		if (!image.empty())
		{
			Point2f pLeft = Point2f(vpoints[j][0],vpoints[j][1]);
			Point2f pRight = Point2f(vpoints[j][2],vpoints[j][3]);
#if 0	//光照时需归一化
			Mat dst = alignFace(image, pRight, pLeft);	//尺寸归一化

#elif 0	//头部姿态标记两眼距离
			Mat dst = image;
			line(dst, pLeft, pRight, Scalar(0, 0, 0), 2, 8);
			circle(dst, pLeft, 2, Scalar(255, 255, 255), 2, 8);
			circle(dst, pRight, 2, Scalar(255, 255, 255), 2, 8);

#else 1	//对称性
			Mat dst = alignFace(image, pRight, pLeft);	//尺寸归一化

#endif
			vmfaces.push_back(dst);
	/*		imshow("image", dst);
			waitKey(0);*/
		}
	}
	//将人脸各项信息存入Face
	for (size_t m = 0; m<maxSize; m++)
	{
		Face mface;
		mface.filename = vpaths[m];
		mface.id = vids[m];
		mface.sign = vsigns[m];
		mface.eyePoints = vpoints[m];
		mface.face = vmfaces[m];
		mface.value = 0.0;
		vfaces.push_back(mface);
	}
}
Mat alignFace(Mat image,Point2f potLeft, Point2f potRight)
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

double GetPointDistance(Point2f p1,Point2f p2)
{
	return sqrt(abs(p1.x-p2.x)*abs(p1.x-p2.x)+abs(p1.y-p2.y)*abs(p1.y-p2.y));
}

//计算图片光照质量
LightQuality computerLightQuality(Mat image)
{
	LightQuality lightQuality;
	if (image.channels() == 1)
	{
		lightQuality.dark_spot = zbrightness_dark_spot(image, 0.3);
		lightQuality.equalization = zbrightness_equalize(image);
		lightQuality.y = 0;
		lightQuality.dark_channel = zbrightness_middle_pixel(image);	//实际是80%像素所占灰度值
		lightQuality.mean = sum(image).val[0];
	}
	if (image.channels() == 3)
	{
		lightQuality.y = zbrightness_Y(image);
		lightQuality.dark_channel = zbrightness_dark(image);
		Mat grayImage;
		cvtColor(image, grayImage, CV_BGR2GRAY);
		lightQuality.dark_spot = zbrightness_dark_spot(grayImage, 0.3);
		lightQuality.equalization = zbrightness_equalize(grayImage);
		lightQuality.mean = sum(image).val[0];
	}
	return lightQuality;
}

//计算图片姿态质量
PoseQuality computerPoseQuality(Vec4f eyePoints)
{
	PoseQuality poseQuality;

	poseQuality.eyes_angle = zpose_eyes_angle(eyePoints);
	poseQuality.eyes_length = zpose_eyes_length(eyePoints);
	return poseQuality;
}

//计算图片对称性
SymmetryQuality computerSymmetryQuality(Mat image)
{
	SymmetryQuality symmetryQuality;
	if (image.channels() == 3)
	{
		cvtColor(image, image, CV_BGR2GRAY);
	}
	if (image.channels() == 1)
	{
		symmetryQuality.hist = zsymmetry_hist(image);
		symmetryQuality.lbph = zsymmetry_lbph(image);
		symmetryQuality.gabor = zsymmetry_gabor(image);
	}

	return symmetryQuality;
}

//获得pel人脸库信息
void getPelFace(string directory, vector<Face>& vfaces)
{
	vector<string> vfilenames;
	Face face;
	getPaths(directory, vfilenames);
	for (size_t i=0; i<vfilenames.size(); i++)
	{
		face.filename = vfilenames[i];
		face.face = imread(vfilenames[i], 0);
		vfaces.push_back(face);
	}	
}

//从路径获取文件名
string getFilenameFromPath(string path)
{
	size_t npos = path.find_last_of('/');
	return path.substr(npos+1, path.size()-npos-1-4-4);
}