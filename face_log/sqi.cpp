#include "stdafx.h"
#include "sqi.h"

Mat DogFilter(const Mat& image, Size dsize)
{
	Mat img_G0, img_G1;
	GaussianBlur(image,img_G0,dsize,0);
	GaussianBlur(img_G0,img_G1,dsize,0);
	//GaussianBlur(image,img_G0,Size(1, 1),0);
	//GaussianBlur(img_G0,img_G1,Size(3, 3),0);
	Mat img_DoG = img_G0 - img_G1;
	normalize(img_DoG,img_DoG,0,255,CV_MINMAX);
	return img_DoG;
}


Mat DarkChannel(const Mat& image,int patch, int flag)
{
	int cc = image.channels();
	CV_Assert(cc == 1);
	Mat image1 = image.clone();
	int lenght = patch / 2;	//要填充的边界大小
	int borderType = BORDER_REPLICATE; //填充类型 BORDER_REPLICATE BORDER_CONSTANT
	Scalar value = Scalar::all(0);	//填充的值
	copyMakeBorder(image, image1, lenght,lenght, lenght, lenght, borderType, value);
	int temp = 0;
	Mat dst = Mat::zeros(image.size(), image.type());
	for(int i=lenght; i<image1.rows-lenght; i++)
		for (int j=lenght; j<image1.cols-lenght; j++)
		{
			//取邻域最小
			if (flag == 0)
			{
				temp = INT_MAX;
				for(int m=-lenght; m<lenght; m++)
					for (int n=-lenght; n<lenght; n++)
					{
						if (image1.at<UCHAR>(i+m, j+n) < temp)
						{
							temp = image1.at<UCHAR>(i+m, j+n);
						}

					}
				dst.at<UCHAR>(i-lenght, j-lenght) = temp;

			}
			//取邻域最大
			if (flag == 1)
			{
				temp = 0;
				for(int m=-lenght; m<lenght; m++)
					for (int n=-lenght; n<lenght; n++)
					{
						if (image1.at<UCHAR>(i+m, j+n) > temp)
						{
							temp = image1.at<UCHAR>(i+m, j+n);
						}

					}
					dst.at<UCHAR>(i-lenght, j-lenght) = temp;
			}
		}
		return dst;
}

//获得彩图的dark channel 图像; 0取小，1取大
Mat DarkImage(const Mat& image, int patch, int flag)
{
	//CV_Assert(image.channels() == 3);
	vector<Mat> plane;
	split(image, plane);
	for (size_t n=0; n<plane.size();n++)
	{
		plane[n] = DarkChannel(plane[n], patch, flag);
		//plane[n] = zdarkChannel(plane[n], 10);
	}
	int temp = 0;
	Mat dst = Mat::zeros(plane[0].size(), plane[0].type());
	for(int i=0; i < plane[0].rows; i++)
		for(int j=0; j < plane[0].cols; j++)
		{
			//求多通道中最小值
			if (flag == 0)
			{
				temp = INT_MAX;
				for (size_t n=0; n<plane.size();n++)
				{
					int cc = plane[n].at<UCHAR>(i, j);
					if (plane[n].at<UCHAR>(i, j)<temp)
					{
						temp = plane[n].at<UCHAR>(i, j);
					}
				}
				dst.at<UCHAR>(i, j) = temp;
			}
			//求多通道中最大值
			if (flag == 1)
			{
				temp = 0;
				for (size_t n=0; n<plane.size();n++)
				{
					int cc = plane[n].at<UCHAR>(i, j);
					if (plane[n].at<UCHAR>(i, j)>temp)
					{
						temp = plane[n].at<UCHAR>(i, j);
					}
				}
				dst.at<UCHAR>(i, j) = temp;
			}

		}
		return dst;
}

//灰度图 减去多通道差值的平均
Mat ChannelDiff(Mat image)
{
	CV_Assert(image.channels() == 3);
	vector<Mat> vimages;
	split(image, vimages);
	Mat dst0, dst1, dst2, dst;
	absdiff(vimages[0], vimages[1], dst0);
	absdiff(vimages[1], vimages[2], dst1);
	absdiff(vimages[0], vimages[2], dst2);
	//dst0.convertTo(dst0, CV_32F);
	//dst1.convertTo(dst1, CV_32F);
	//dst2.convertTo(dst2, CV_32F);
	addWeighted(dst0, 0.33, dst1, 0.33, 0, dst);
	addWeighted(dst, 1, dst2, 0.33, 0, dst);
	Mat grayImage, showImage;
	cvtColor(image, grayImage, CV_BGR2GRAY);
	absdiff(grayImage, dst, showImage);
	imshow("grayImage", grayImage);
	imshow("showImage", showImage);
	imshow("dst", dst);
	waitKey(0);
	return dst;
}