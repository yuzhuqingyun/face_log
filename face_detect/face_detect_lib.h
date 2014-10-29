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
// ��������: GetASMFace
// ����˵��:  ��ASM��ȡ�������򣬶�λ�ؼ���
// ��    ��: Juliwei
// �������: 2014/09/25
// �� �� ֵ: Mat
// ��    ��:Mat _src 1��3ͨ��
// ��    ��:int flag	0����ͼƬ�������ؼ��㣻1�������򣬱����ؼ��㣻2��������
//************************************
Mat GetASMFace(Mat _src, int flag);
Mat GetOpencvFace( Mat frame);
#endif