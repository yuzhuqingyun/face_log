//�й�ͼ�����������Ķ���ͺ���
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


//�洢���еĹ�������
typedef struct LightQuality 
{
	double dark_spot;	//�������
	double y;	//Y������ֵ
	double middle_pixel;	//ʵ����80%������ռ�Ҷ�ֵ
	double equalization;	//�ֿ������ԭͼ�Ƚ�
	double mean;	//�Ҷ�ͼ��ֵ������δ�����ȷ��
} LightQuality;

//�洢���еĽǶ�����
typedef struct PoseQuality 
{
	double eyes_angle;
	double eyes_length;
} PoseQuality;

//�洢���еĶԳ���ָ��
typedef struct SymmetryQuality 
{
	double hist;
	double gabor;
	double lbph;
} SymmetryQuality;

//�洢���е���������ָ���������
typedef struct Quality 
{
	LightQuality lightQuality;
	PoseQuality poseQuality;
	SymmetryQuality symmetryQuality;
} Quality;

//�洢������Ϣ
typedef struct Face
{
	string filename;	//�ļ���
	string sign;		//ʵ���������
	int id;				//����ID
	Vec4f eyePoints;	//���������
	Mat face;		//��һ������
	double value;	//ָ���С
	Quality quality;	//ͼƬ����

	//���ز�����< >Ϊsort��׼��	eg. sort(vpw.begin(), vpw.end(),greater<ValuePerWeight>());//����	sort(vpw.begin(), vpw.end(),less<ValuePerWeight>());//����
	bool operator <(const Face &other)const   //��������
	{
		return value>other.value;
	}
	//����
	bool operator >(const Face &other)const   //��������
	{
		return value>other.value;
	}

} Face;

//bool MaxFaceValue(const Face &face1, const Face &face2)
//{
//	if (face1.value > face2.value)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//bool MinFaceValue(const Face &face1, const Face &face2)
//{
//	if (face1.value < face2.value)
//	{
//		return true;
//	}
//	else
//		return false;
//}

LightQuality ComputerLightQuality(Mat image);
PoseQuality ComputerPoseQuality(Vec4f eyePoints);
SymmetryQuality ComputerSymmetryQuality(Mat image);
Quality ComputerQuality(const Face& faceInfo);
int QualityAsRowMatrix(const Quality& quality, Mat& matOfQuality);	//����0�ɹ�

#endif