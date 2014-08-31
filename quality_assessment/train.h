#pragma once
#ifndef QUALITY_ASSESSMENT_TRAIN_H
#define QUALITY_ASSESSMENT_TRAIN_H

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include <string.h>
#include <iostream>

#include "quality.h"
#include "lbph.h"

using namespace cv;
using namespace std;

int TrainQualityAssessment(const string strDirectory);
void ReadTxt(const string filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator);	//��ȡ�����п�Ժ˵���ı�����txt
void ReadFilename(const string filename, int& id, string& sign);	//���ļ�������ȡ��Ϣid�͹��յı��
void GetFaceFromTxt(const string txtname, vector<Face>& vfaces);	//��txt��ȡ������Ϣ
int GetFaceFromFilename(const string filePath, const Vec4f point, Face& faces);
#endif