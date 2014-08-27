#pragma once
#include "quality.h"
#include "zlib.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "cvgabor.h"
#include "sqi.h"

using namespace cv;

//计算两个矩阵协方差
double zmse(Mat image1, Mat image2);
//检测并画出人脸区域
void detectAndDraw( Mat& img, vector<Mat>& vfaces);
// computes PSNR image/video quality metric opencv自带接口为PSNR(), imgproc.hpp
double zPSNR(Mat im1,Mat im2);
//方差
double zstdDev(Mat src, Mat dst);
//计算直方图
Mat zcomputeHist(Mat src);
//均方差-对比度
double zc0(Mat image);

//人脸区域占图片比例
double zf1(Mat image, Mat face);

//梯度表清晰度
double zcl1(Mat image);

//直方图求对比度
double zc1(Mat image);

//图像序列求明亮度
double zlight(Mat image, vector<Mat> images);

//DCT计算清晰度， belta控制滤除的高频比例
double zsharpness1(Mat image, double belta);

//直方图衡量左右脸对称性
double zsymmetry_hist(Mat image);

//Gabor衡量对称
double zsymmetry_gabor(Mat image);

//LBPH衡量对称
double zsymmetry_lbph(Mat image);

//************************************
// 函数名称: zbrightness_dark_spot
// 函数说明:  人脸图片中阴影像素点比例
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: double
// 参    数:Mat image 灰度图
// 参    数:double alpha  小于alpha*maxValue的算暗像素
//************************************
double zbrightness_dark_spot(Mat image, double alpha);

//************************************
// 函数名称: zbrightness2
// 函数说明:  用YUV中Y分量的均值衡量
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: double
// 参    数:Mat image 必须是彩图
//************************************
double zbrightness_Y(Mat image);

//************************************
// 函数名称: zbrightness_dark
// 函数说明:  计算暗通道的均值
// 作    者: Juliwei
// 完成日期: 2014/07/18
// 返 回 值: double
// 参    数:Mat image彩图
//************************************
double zbrightness_dark(Mat image);


//************************************
// 函数名称: zbrightness_equalize
// 函数说明:  直方图均衡之后与原图的均方差
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: double
// 参    数:Mat image灰度图
//************************************
double zbrightness_equalize(Mat image);


//************************************
// 函数名称: zbrightness_middle_pixel
// 函数说明:  计算中间80%的像素数量的灰度范围和总的灰度范围的80%的比例
// 作    者: Juliwei
// 完成日期: 2014/08/07
// 返 回 值: double
// 参    数:Mat image单通道
//************************************
double zbrightness_middle_pixel(Mat image);


//************************************
// 函数名称: zdarkChannel
// 函数说明:  获取黑色通道
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: cv::Mat
// 参    数:Mat image， 单通道
// 参    数:int patch， 核大小
//************************************
Mat zdarkChannel(const Mat image,int patch); 

//************************************
// 函数名称: zdarkImage
// 函数说明:  将3通道图片的dark channel合并
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: Mat 灰度图
// 参    数:Mat image 必须为彩图
//************************************
Mat zdarkImage(Mat image);

//************************************
// 函数名称: zpose_eyes_angle
// 函数说明:  计算两眼坐标角度
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: double
// 参    数:Vec4f eyePoints两眼坐标
//************************************
double zpose_eyes_angle(Vec4f eyePoints);

//************************************
// 函数名称: zpose_eyes_length
// 函数说明:  计算两眼坐标的距离
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: double
// 参    数:Vec4f eyePoints两眼坐标
//************************************
double zpose_eyes_length(Vec4f eyePoints);
