#pragma once
#include "quality.h"
#include "zlib.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "cvgabor.h"
#include "sqi.h"

using namespace cv;

//������������Э����
double zmse(Mat image1, Mat image2);
//��Ⲣ������������
void detectAndDraw( Mat& img, vector<Mat>& vfaces);
// computes PSNR image/video quality metric opencv�Դ��ӿ�ΪPSNR(), imgproc.hpp
double zPSNR(Mat im1,Mat im2);
//����
double zstdDev(Mat src, Mat dst);
//����ֱ��ͼ
Mat zcomputeHist(Mat src);
//������-�Աȶ�
double zc0(Mat image);

//��������ռͼƬ����
double zf1(Mat image, Mat face);

//�ݶȱ�������
double zcl1(Mat image);

//ֱ��ͼ��Աȶ�
double zc1(Mat image);

//ͼ��������������
double zlight(Mat image, vector<Mat> images);

//DCT���������ȣ� belta�����˳��ĸ�Ƶ����
double zsharpness1(Mat image, double belta);

//ֱ��ͼ�����������Գ���
double zsymmetry_hist(Mat image);

//Gabor�����Գ�
double zsymmetry_gabor(Mat image);

//LBPH�����Գ�
double zsymmetry_lbph(Mat image);

//************************************
// ��������: zbrightness_dark_spot
// ����˵��:  ����ͼƬ����Ӱ���ص����
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: double
// ��    ��:Mat image �Ҷ�ͼ
// ��    ��:double alpha  С��alpha*maxValue���㰵����
//************************************
double zbrightness_dark_spot(Mat image, double alpha);

//************************************
// ��������: zbrightness2
// ����˵��:  ��YUV��Y�����ľ�ֵ����
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: double
// ��    ��:Mat image �����ǲ�ͼ
//************************************
double zbrightness_Y(Mat image);

//************************************
// ��������: zbrightness_dark
// ����˵��:  ���㰵ͨ���ľ�ֵ
// ��    ��: Juliwei
// �������: 2014/07/18
// �� �� ֵ: double
// ��    ��:Mat image��ͼ
//************************************
double zbrightness_dark(Mat image);


//************************************
// ��������: zbrightness_equalize
// ����˵��:  ֱ��ͼ����֮����ԭͼ�ľ�����
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: double
// ��    ��:Mat image�Ҷ�ͼ
//************************************
double zbrightness_equalize(Mat image);


//************************************
// ��������: zbrightness_middle_pixel
// ����˵��:  �����м�80%�����������ĻҶȷ�Χ���ܵĻҶȷ�Χ��80%�ı���
// ��    ��: Juliwei
// �������: 2014/08/07
// �� �� ֵ: double
// ��    ��:Mat image��ͨ��
//************************************
double zbrightness_middle_pixel(Mat image);


//************************************
// ��������: zdarkChannel
// ����˵��:  ��ȡ��ɫͨ��
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: cv::Mat
// ��    ��:Mat image�� ��ͨ��
// ��    ��:int patch�� �˴�С
//************************************
Mat zdarkChannel(const Mat image,int patch); 

//************************************
// ��������: zdarkImage
// ����˵��:  ��3ͨ��ͼƬ��dark channel�ϲ�
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: Mat �Ҷ�ͼ
// ��    ��:Mat image ����Ϊ��ͼ
//************************************
Mat zdarkImage(Mat image);

//************************************
// ��������: zpose_eyes_angle
// ����˵��:  ������������Ƕ�
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: double
// ��    ��:Vec4f eyePoints��������
//************************************
double zpose_eyes_angle(Vec4f eyePoints);

//************************************
// ��������: zpose_eyes_length
// ����˵��:  ������������ľ���
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: double
// ��    ��:Vec4f eyePoints��������
//************************************
double zpose_eyes_length(Vec4f eyePoints);
