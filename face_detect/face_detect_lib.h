#pragma once
#ifndef FACE_DETECT_FACE_DECTECT_LIB_H
#define FACE_DETECT_FACE_DECTECT_LIB_H

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"


#include "yw_ASMDll.h"

#pragma comment(lib,"yw_ASMDll_d.lib")
using namespace cv;
//************************************
// 函数名称: GetASMFace
// 函数说明:  用ASM获取人脸区域，定位关键点
// 作    者: Juliwei
// 完成日期: 2014/09/25
// 返 回 值: Mat
// 参    数:Mat _src 1、3通道
// 参    数:int flag	0整张图片，标明关键点；1人脸区域，标明关键点；2人脸区域
//************************************
Mat GetASMFace(Mat _src, int flag);
Mat GetOpencvFace( Mat frame);
#endif