//定义一些常用的函数、类
#pragma once
#ifndef QUALITY_ASSESSMENT_LIB_H
#define QUALITY_ASSESSMENT_LIB_H

#include "opencv2\opencv.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

double GetPointDistance(Point2f p1,Point2f p2);
Mat AlignFace(Mat image,Point2f potLeft, Point2f potRight);
string GetFilenameFromPath(string path);
Mat ComputeHist(Mat src);	//计算直方图
double Mse(Mat image1, Mat image2);	//计算两个矩阵协方差

#endif