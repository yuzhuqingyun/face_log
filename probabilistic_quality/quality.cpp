#include "stdafx.h"
#include "quality.h"

//归一化到0~1
void norm0_1(Mat& image)
{
	image.convertTo(image, CV_64F);
	double minValue, maxValue;
	minMaxLoc(image, &minValue, &maxValue);
	for (int i=0; i<image.rows; i++)
	{
		for (int j=0; j<image.cols; j++)
		{
			image.at<double>(i, j) = (image.at<double>(i, j)-minValue)/maxValue;
		}
	}
}

//将矩阵转变为行向量
Mat MatAsRow(Mat src)
{
	CV_Assert(src.type() == CV_64F);
	Mat rowMat;
	rowMat.create(1 ,src.rows*src.cols-1, CV_64FC1);
	int count=0;
	for (int i=0; i<src.rows; i++)
	{
		for (int j=0; j<src.cols; j++)
		{
			//第0个分量不要
			if (i==0 && j==0)
			{
				continue;
			}
			rowMat.at<double>(0, count) = src.at<double>(i, j);
			count++;
		}
	}
	CV_Assert(count == src.rows*src.cols-1);
	return rowMat;
}
//DCT变换，前d*d个低频成分（除0外）保留
Mat ExtractDCT(Mat src, int d)
{
	Mat dctMat, dctDst;
	dctMat.create(src.size(), CV_64FC1);
	Mat srcImg = src;
	src.convertTo(srcImg, CV_64F);
	dct(srcImg, dctMat, CV_DXT_FORWARD);
	normalize(dctMat, dctMat, 255, 0, NORM_MINMAX);

	imshow("image", src);
	imshow("dct", dctMat);
	waitKey(0);
	Rect rect(0, 0, d, d);
	dctDst = dctMat(rect);
	return MatAsRow(dctDst);
}
//将图片切块
void CutBlocks(const Mat& src, vector<Mat>& blocks, int blockSize, int overlap)
{
	Mat srcImg;
	srcImg.create(src.rows, src.cols, CV_64FC1);
	src.convertTo(srcImg, CV_64F);
	add(srcImg, 1.0, srcImg);
	log(srcImg,srcImg);
	Mat block;
	int step = blockSize-overlap;
	int xblock=0;	//block的左上点坐标
	while ( xblock<=srcImg.cols-blockSize)
	{
		int yblock=0;
		while ( yblock<=srcImg.rows-blockSize)
		{
			Rect rect(xblock, yblock, blockSize, blockSize);
			block = srcImg(rect);
			//归一化到0~1
			//block.convertTo(block, CV_64F);
			normalize(block, block, 1.0, 0.0, NORM_MINMAX);
			Mat blockDct = ExtractDCT(block, 8);
			blocks.push_back(blockDct);
			yblock+=step;
		}
		xblock+=step;

	}
}
