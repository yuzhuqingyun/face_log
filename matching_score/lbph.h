#pragma once
#ifndef QUALITY_ASSESSMENT_LBPH_H
#define QUALITY_ASSESSMENT_LBPH_H

#include "opencv2\opencv.hpp"

using namespace cv;

Mat ComputerLBPH(Mat src);	//src单通道
double CompareLBPH(Mat image1, Mat image2);	//比较两图片的LBPH,输入为直方图

#endif