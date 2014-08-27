#include "stdafx.h"
#include "test.h"
#include "zlib.h"
#include "quality.h"
#include "sqi.h"



//string cascadeName =
//"haarcascades/haarcascade_frontalface_alt.xml";   //ʹ�õ�Ԥ��ѵ���õļ������ǰ�����
//string nestedCascadeName =
//"haarcascades/haarcascade_eye_tree_eyeglasses.xml";  //Ԥ��ѵ���õ��۾����۾�����

//�������
//int detectFace( Mat& img, CascadeClassifier& cascade);
//����ļ�����������Ƶ��·��
int getPaths(string sdirectory, vector<string>& vpaths);
//������Ƶ
int video(string path);

void test()
{
	//��ȡ��Ƶ·��
	vector<string> vpaths;
	getPaths("G:\\Video\\NRC-IIT", vpaths);
	for (vector<string>::size_type i = 0; i < vpaths.size(); i++)
	{
		video(vpaths[i]);
	}
}


//����ļ�����������Ƶ��·��
int getPaths(string sdirectory, vector<string>& vpaths)
{
	Directory dir;
	vpaths = dir.GetListFilesR(sdirectory, "*.*");
	return 1;
}


int video(string path)
{

	//����Ƶ�ļ�
	VideoCapture capture(path);
	//�����Ƶ�Ƿ��
	if(!capture.isOpened())
		return 1;

	// �õ�֡��
	double rate= capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	Mat frame; // ���ڵ���Ƶ֡
	namedWindow("Extracted Frame");

	// ��֮֡��ļ��ʱ��
	int delay= 1000/rate;
	// ѭ���������е�֡
	while (!stop) {
		// ����һ֡
		if (!capture.read(frame))
			break;

		//TODO:Ԥ����


		//TODO:��������
		vector<Mat> vfaces;
		detectAndDraw(frame, vfaces); 

		//�ڴ�������ʾͼ��
		imshow("Extracted Frame",frame);
		// ��Esc��ֹͣ��Ƶ����
		if (waitKey(delay) == 27)
			stop= true;
		waitKey(20);
	}
	// �ر���Ƶ�ļ�
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
		//�߶ȹ�һ��
		resize(test[m], test[m], Size(200, 200));
		//��˹ƽ��
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
	//	//�뾶
	//	int r = 2;
	//	//����ͼ��
	//	//����Ϊ�����ص�ͼ��Բ�ġ��뾶����ɫ����ϸ������
	//	circle(picture,center,r,color);
	//}
	//imshow("�װ�",picture);
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
		//��˹ģ��
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

//��ȡ����
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

//canny��Ե
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


//ͨ���Ƚ���ƽ�������ͼƬ����������
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
//������
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

//����������darkchannel
void testBrightness1()
{
	string path = "G:\\Picture\\jiaodaku\\ԭʼͼ";
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
		//����ͼƬ
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

//��������
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
	//string path = "G:\\Picture\\������";
	vector<Mat> test;
	vector<string> vpaths;
	getPaths(path, vpaths);
	for (size_t i = 0; i < vpaths.size(); i++)
	{
		test.push_back(imread(vpaths[i], 1));
	}
	for (size_t m = 0; m < test.size(); m++) 
	{
		blur(test[m],test[m],Size(3,3));	//ģ��ȥ��

		
		Mat src/*, abs0, abs1, showImage*/;
		Mat contourImage0, contourImage1;
		Mat diffImg0, diffImg1;
		cvtColor(test[m], src, CV_BGR2GRAY);
		//src = zSetDark(src, 0.1);	//������һ����ֵ��������0
		//ȡ������ͨ��
		Mat dst0 = DarkImage(src, 4, 0);
		dst0 = DarkImage(dst0, 4, 0);
		dst0 = DarkImage(dst0, 4, 1);
		//Mat edgeImage0 = zFindEdge(dst0, 3, 2);
		//dilate(edgeImage0, edgeImage0, Mat(), Point(-1, -1), 1);
		//Mat blob0 = zFindBlobs(src);
		//Mat blob1 = zFindBlobs(src);

		//Mat dst1 = DarkImage(src, 3, 1);
		//ȡ��ͼ��
		//absdiff(src, dst0, abs0);
		//absdiff(dst0, dst1, abs1);
		//��Ե���
		Mat edgeImage0 = zFindEdge(dst0, 3, 2);
		//��̬ѧ����
		dilate(edgeImage0, edgeImage0, Mat(), Point(-1, -1), 1);
		erode(edgeImage0, edgeImage0, Mat(), Point(-1, -1), 1);

		contourImage0 = zFindContours(edgeImage0);
		cvtColor(contourImage0, contourImage0, CV_BGR2GRAY);
		absdiff(src, contourImage0, diffImg0);

		//Mat edgeImage1 = zFindEdge(src, 3, 2);
		////contourImage1 = zFindContours(edgeImage1);
		////cvtColor(contourImage1, contourImage1, CV_BGR2GRAY);
		//subtract(src, edgeImage1, diffImg1);
		//��ֵ�ָ�
		//for(int i=0; i<abs0.rows; i++)
		//	for (int j=0; j<abs0.cols; j++)
		//	{
		//		//��darkͨ��ֱ����ֵ�ָ�
		//		if (dst0.at<uchar>(i, j) < 50)
		//		{
		//			abs0.at<uchar>(i, j)=255;
		//		}
		//		else
		//		{
		//			abs0.at<uchar>(i, j)=0;
		//		}
		//		////��ֵ�ָ�
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

		//����ͼƬ
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
	AfxMessageBox("�ָ�ߵ������");
}

//Ѱ������
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
		//��ȥ���������С��
		if (fabs(contourArea(contours[idx])) > 20 && fabs(contourArea(contours[idx])) < 200)
		{
			//��ȥ�ܳ�̫���
			if (contours[idx].size()<100)
			{

				Scalar color( rand()&255, rand()&255, rand()&255 );
				drawContours( dst, contours, idx, Scalar::all(255), CV_FILLED, 8, hierarchy );
			}
		}
	}
	//for(int i=0;i<(int)contours.size();i++)//���з��ŵ�contours.size()ǿ��ת��λ�Ƿ��ŵģ�
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

//sobel��Laplacion��canny
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
		convertScaleAbs( dst, dst );//���ؾ���ֵ
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



//����
Mat zFindBlobs(Mat image)
{
	Mat descriptors;  
	vector<KeyPoint> keypoints;  
	SimpleBlobDetector::Params params;

	//params.minThreshold = 10;  
	//params.maxThreshold = 100;  
	//params.thresholdStep = 10; 
	//params.minDistBetweenBlobs = 1.0;	//blob����С����

	params.filterByColor = true;	//��ɫ
	params.blobColor = 0;	//0��dark��255��light

	params.filterByArea = true;	//���
	params.minArea = 6.0;   //��С���
	params.maxArea = 100.0;	//������

	//params.filterByCircularity = true;  
	//params.minCircularity = 0.5;
	//params.maxCircularity = 1;

	//params.filterByInertia = true;	//��������
	//params.minInertiaRatio = 0.01; 
	//params.maxInertiaRatio = 0.1; 

	//params.filterByConvexity = true;	//͹��
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

//��̬ѧ����
Mat zMorphology(Mat image, int flag)
{
	 int morph_elem = 0;	//MORPH_RECT MORPH_ELLIPSE MORPH_CROSS 
	 int morph_size = 1;	//�˴�С
	 Mat dst;
	 Mat element = getStructuringElement(morph_elem, Size(2*morph_size+1, 2*morph_size+1), Point(morph_size, morph_size));
	 morphologyEx(image, dst, flag+2, element);
	 return dst;
}

//��ȡ�����п�Ժ˵���ı�����txt
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

//���ļ�������ȡ��Ϣid�͹��յı��
void read_filename(const string& filename, int& id, string& sign)
{	
	id = atoi(filename.substr(3, 6).c_str());
	sign = filename.substr(17, 5);	//���ձ�־(11, 5)����̬(17, 5)
}

//�����п�Ժ�����ӿ�
void test_cas_light()
{
	int lightFlag = 1;	//ѡ�����ָ������
	int saveFlag = 1;	//0��ʾ�� 1д��
	string filename = "G:\\Picture\\Lighting\\FaceFP_2.txt";	//txt·��
	string savePath;	//����·��

	vector<Face> vfaces;
	vector<LightQuality> vlightQuality;
	getFaceFromTxt(filename, vfaces);	//��txt��ȡ������Ϣ
	for (size_t i=0; i<vfaces.size(); i++)//�����������
	{
		Mat image = vfaces[i].face.clone();
		LightQuality lightQuality = computerLightQuality(image);
		vlightQuality.push_back(lightQuality);
		//����Ϊ�����ص�ͼƬ����������֣����ֵ�λ�ã��ı������½ǣ������壬��С����ɫ
		//char words[50];
		//sprintf(words, "%4.2f%s%4.2f%s%4.2f%s%4.2f", lightQuality.dark_spot, "_", lightQuality.equalization, "_",lightQuality.y, "_", lightQuality.dark_channel);
		//putText( image, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
		//imshow("image", image);
		//waitKey(0);
	}
	//ѡ�������ָ��
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
	sort(vfaces.begin(), vfaces.end(), less<Face>());	//��ָ��value��������
	for (size_t m=0; m<vfaces.size(); m++)
	{
		savePath = "G:\\Picture\\result\\temp\\";
		Mat image0 = vfaces[m].face.clone();
		char words[50];
		if (saveFlag == 1)	//����
		{
			sprintf(words, "%d%s%4.2f%s%s%s", vfaces[m].id, "_", vfaces[m].value, "_", vfaces[m].sign.c_str(), ".jpg");
			savePath += string(words);
			imwrite(savePath, image0);
		} 
		else				//��ʾ
		{
			sprintf(words, "%s%4.2f", "dark_spot: ", vfaces[m].value);
			putText( image0, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
			imshow("image0", image0);
			waitKey(0);
		}		

	}
}

//�����п�Ժ�Ƕ��ӿ�
void test_cas_pose()
{
	int poseFlag = 0;	//ѡ��Ƕ�ָ������
	int saveFlag = 1;	//0��ʾ�� 1д��
	string filename = "G:\\Picture\\pose\\FaceFP_2.txt";	//txt·��
	string savePath;	//����·��

	vector<Face> vfaces;
	vector<PoseQuality> vposeQuality;
	getFaceFromTxt(filename, vfaces);	//��txt��ȡ������Ϣ
	for (size_t i=0; i<vfaces.size(); i++)//�����������
	{
		PoseQuality poseQuality = computerPoseQuality(vfaces[i].eyePoints);
		vposeQuality.push_back(poseQuality);
		//����Ϊ�����ص�ͼƬ����������֣����ֵ�λ�ã��ı������½ǣ������壬��С����ɫ
		//char words[50];
		//sprintf(words, "%4.2f%s%4.2f%s%4.2f%s%4.2f", lightQuality.dark_spot, "_", lightQuality.equalization, "_",lightQuality.y, "_", lightQuality.dark_channel);
		//putText( image, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
		//imshow("image", image);
		//waitKey(0);
	}
	//ѡ�������ָ��
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
	sort(vfaces.begin(), vfaces.end(), less<Face>());	//��ָ��value��������
	for (size_t m=0; m<vfaces.size(); m++)
	{
		savePath = "G:\\Picture\\result\\temp\\";
		Mat image0 = vfaces[m].face.clone();
		char words[50];
		if (saveFlag == 1)	//����
		{
			sprintf(words, "%d%s%4.2f%s%s%s", vfaces[m].id, "_", vfaces[m].value, "_", vfaces[m].sign.c_str(), ".jpg");
			savePath += string(words);
			imwrite(savePath, image0);
		} 
		else				//��ʾ
		{
			sprintf(words, "%s%4.2f", "dark_spot: ", vfaces[m].value);
			putText( image0, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
			imshow("image0", image0);
			waitKey(0);
		}		

	}
}

//�����п�Ժ���պ���̬�ӿ�Գ���
void test_cas_symmetry()
{
	int symmetryFlag = 2;	//ѡ�����ָ������
	int saveFlag = 1;	//0��ʾ�� 1д��
	string directory = "G:\\Picture\\PEL";	//txt·��
	string savePath;	//����·��
	int saveFace = 1;	//0-����ԭʼ·��ԭʼͼ��1-�����һ������
	vector<Face> vfaces;
	vector<SymmetryQuality> vsymmetryQuality;
	getPelFace(directory, vfaces);	//��txt��ȡ������Ϣ
	for (size_t i=0; i<vfaces.size(); i++)//�����������
	{
		Mat image = vfaces[i].face.clone();
		SymmetryQuality symmetryQuality = computerSymmetryQuality(image);
		vsymmetryQuality.push_back(symmetryQuality);
		//����Ϊ�����ص�ͼƬ����������֣����ֵ�λ�ã��ı������½ǣ������壬��С����ɫ
		//char words[50];
		//sprintf(words, "%4.2f%s%4.2f%s%4.2f%s%4.2f", lightQuality.dark_spot, "_", lightQuality.equalization, "_",lightQuality.y, "_", lightQuality.dark_channel);
		//putText( image, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
		//imshow("image", image);
		//waitKey(0);
	}
	//ѡ�������ָ��
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
	sort(vfaces.begin(), vfaces.end(), less<Face>());	//��ָ��value��������
	for (size_t m=0; m<vfaces.size(); m++)
	{
		savePath = "G:\\Picture\\result\\temp\\";
		Mat image0;
		if (saveFace == 0)
		{
			string long_path = "G:\\Picture\\pose\\" + vfaces[m].filename.substr(0, vfaces[m].filename.size()-1) + ".jpg";	//�ָ�õ����ļ������һ���ո�-1ȥ��
			image0 = imread(long_path, 0);
		} 
		else
		{
			image0 = vfaces[m].face.clone();
		}

		char words[50];
		if (saveFlag == 1)	//����
		{
#if 0	//cas������̬
			sprintf(words, "%d%s%4.2f%s%s%s", vfaces[m].id, "_", vfaces[m].value, "_", vfaces[m].sign.c_str(), ".jpg");
			savePath += string(words);
			imwrite(savePath, image0);
#else 1	//pel�Գ���
			sprintf(words, "%s%4.2f%s", "-", vfaces[m].value, ".jpg");
			string filename = getFilenameFromPath(vfaces[m].filename);
			savePath += filename+string(words);
			imwrite(savePath, image0);
#endif
		} 
		else				//��ʾ
		{
			sprintf(words, "%s%4.2f", "dark_spot: ", vfaces[m].value);
			putText( image0, words, Point( 5,15),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255) );
			imshow("image0", image0);
			waitKey(0);
		}		

	}
}


//��txt��ȡ������Ϣ
/*"G:\\Picture\\Lighting\\FaceFP_2.txt"*/
void getFaceFromTxt(const string& filename, vector<Face>& vfaces)
{
	string sign;
	int id;
	vector<string> vpaths;	//�ļ�·��
	vector<Vec4f> vpoints;	//���������
	vector<int> vids;		//����ID��
	vector<string> vsigns;	//���ձ��
	vector<Mat> vmfaces;		//��һ����������
	read_txt(filename, vpaths, vpoints, ' ');	//�����ļ�������������
	size_t maxSize = 1000;	///vpaths.size()
	for (size_t i = 0; i<maxSize; i++)	//�ֽ��ļ������õ�ID��״̬��־
	{
		string path = vpaths[i];
		read_filename(path, id, sign);
		vids.push_back(id);
		vsigns.push_back(sign);
	}
	for (size_t j = 0; j<maxSize; j++)
	{
		string long_path = "G:\\Picture\\pose\\" + vpaths[j].substr(0, vpaths[j].size()-1) + ".jpg";	//�ָ�õ����ļ������һ���ո�-1ȥ��
		Mat image = imread(long_path, 0);
		if (!image.empty())
		{
			Point2f pLeft = Point2f(vpoints[j][0],vpoints[j][1]);
			Point2f pRight = Point2f(vpoints[j][2],vpoints[j][3]);
#if 0	//����ʱ���һ��
			Mat dst = alignFace(image, pRight, pLeft);	//�ߴ��һ��

#elif 0	//ͷ����̬������۾���
			Mat dst = image;
			line(dst, pLeft, pRight, Scalar(0, 0, 0), 2, 8);
			circle(dst, pLeft, 2, Scalar(255, 255, 255), 2, 8);
			circle(dst, pRight, 2, Scalar(255, 255, 255), 2, 8);

#else 1	//�Գ���
			Mat dst = alignFace(image, pRight, pLeft);	//�ߴ��һ��

#endif
			vmfaces.push_back(dst);
	/*		imshow("image", dst);
			waitKey(0);*/
		}
	}
	//������������Ϣ����Face
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
	double scale = 80/dEyeDis;	//���ۼ����80
	Point2f LeftTop,RightBot;
	/*LeftTop.x = potRight.x-(int)(dEyeDis/2);
	LeftTop.y = potRight.y-(int)(80/scale);*/

	//////�Ƕ�У��
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


	CvRect RcFace = cvRect(potRight.x-(int)20/scale,potRight.y-(int)(20/scale),120/scale,120/scale);	//ѡȡ�����������С
	cvSetImageROI(dstImage,RcFace);
	IplImage *imgSave = cvCreateImage(cvSize(RcFace.width,RcFace.height),img->depth,img->nChannels);
	cvResize(dstImage,imgSave);
	IplImage *norImage = cvCreateImage(cvSize(120,140),img->depth,img->nChannels);	//���������һ��
	cvResize(imgSave,norImage);
	Mat dst(norImage, true);	//��������
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

//����ͼƬ��������
LightQuality computerLightQuality(Mat image)
{
	LightQuality lightQuality;
	if (image.channels() == 1)
	{
		lightQuality.dark_spot = zbrightness_dark_spot(image, 0.3);
		lightQuality.equalization = zbrightness_equalize(image);
		lightQuality.y = 0;
		lightQuality.dark_channel = zbrightness_middle_pixel(image);	//ʵ����80%������ռ�Ҷ�ֵ
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

//����ͼƬ��̬����
PoseQuality computerPoseQuality(Vec4f eyePoints)
{
	PoseQuality poseQuality;

	poseQuality.eyes_angle = zpose_eyes_angle(eyePoints);
	poseQuality.eyes_length = zpose_eyes_length(eyePoints);
	return poseQuality;
}

//����ͼƬ�Գ���
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

//���pel��������Ϣ
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

//��·����ȡ�ļ���
string getFilenameFromPath(string path)
{
	size_t npos = path.find_last_of('/');
	return path.substr(npos+1, path.size()-npos-1-4-4);
}