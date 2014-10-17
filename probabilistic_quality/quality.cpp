#include "stdafx.h"
#include "quality.h"

//��һ����0~1
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

//������ת��Ϊ������
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
			//��0��������Ҫ
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
//DCT�任��ǰd*d����Ƶ�ɷ֣���0�⣩����
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
//��ͼƬ�п�
void CutBlocks(const Mat& src, vector<Mat>& blocks, int blockSize, int overlap)
{
	Mat srcImg;
	srcImg.create(src.rows, src.cols, CV_64FC1);
	src.convertTo(srcImg, CV_64F);
	add(srcImg, 1.0, srcImg);
	log(srcImg,srcImg);
	Mat block;
	int step = blockSize-overlap;
	int xblock=0;	//block�����ϵ�����
	while ( xblock<=srcImg.cols-blockSize)
	{
		int yblock=0;
		while ( yblock<=srcImg.rows-blockSize)
		{
			Rect rect(xblock, yblock, blockSize, blockSize);
			block = srcImg(rect);
			//��һ����0~1
			//block.convertTo(block, CV_64F);
			normalize(block, block, 1.0, 0.0, NORM_MINMAX);
			Mat blockDct = ExtractDCT(block, 8);
			blocks.push_back(blockDct);
			yblock+=step;
		}
		xblock+=step;

	}
}
