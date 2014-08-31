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
void ReadTxt(const string filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator);	//读取读入中科院说明文本内容txt
void ReadFilename(const string filename, int& id, string& sign);	//从文件名中提取信息id和光照的标号
void GetFaceFromTxt(const string txtname, vector<Face>& vfaces);	//从txt获取人脸信息
int GetFaceFromFilename(const string filePath, const Vec4f point, Face& faces);
#endif