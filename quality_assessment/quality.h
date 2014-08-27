//有关图像质量评估的定义和函数
#pragma once
#ifndef QUALITY_ASSESSMENT_QUALITY_H
#define QUALITY_ASSESSMENT_QUALITY_H

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\opencv.hpp"
#include "cvgabor.h"
#include <string>
#include <iostream>
#include <fstream>
#include "lib.h"
#include "lbph.h"

using namespace std;
using namespace cv;

//存储人脸信息
typedef struct Face
{
	string filename;	//文件名
	string sign;		//实际质量标记
	int id;				//人脸ID
	Vec4f eyePoints;	//两眼坐标点
	Mat face;		//归一化人脸
	double value;	//指标大小

	//重载操作符< >为sort做准备	eg. sort(vpw.begin(), vpw.end(),greater<ValuePerWeight>());//降序	sort(vpw.begin(), vpw.end(),less<ValuePerWeight>());//升序
	bool operator <(const Face &other)const   //升序排序
	{
		return value>other.value;
	}
	//或者
	bool operator >(const Face &other)const   //降序排序
	{
		return value>other.value;
	}
} Face;

//存储所有的人脸质量指标评估结果
typedef struct Quality 
{
	double light;
	double symmetry;
	double sharpness;
	double pose;
	double expression;
} Quality;

//存储所有的光照质量
typedef struct LightQuality 
{
	double dark_spot;	//暗点比例
	double y;	//Y分量均值
	double middle_pixel;	//实际是80%像素所占灰度值
	double equalization;	//分块均衡与原图比较
	double mean;	//灰度图均值！！还未检测正确性
} LightQuality;

//存储所有的角度质量
typedef struct PoseQuality 
{
	double eyes_angle;
	double eyes_length;
} PoseQuality;

//存储所有的对称性指标
typedef struct SymmetryQuality 
{
	double hist;
	double gabor;
	double lbph;
} SymmetryQuality;



#endif