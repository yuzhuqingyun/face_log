#pragma once
#ifndef QUALITY_ASSESSMENT_LBPH_H
#define QUALITY_ASSESSMENT_LBPH_H

#include "opencv2\opencv.hpp"

using namespace cv;

Mat ComputerLBPH(Mat src);
double CompareLBPH(Mat image1, Mat image2);	//比较两图片的LBPH

#endif