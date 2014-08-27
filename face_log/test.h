#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\opencv.hpp"

#include <string>
#include <iostream>
#include <fstream>

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
	//重载操作符< >为sort做准备
	//eg. sort(vpw.begin(), vpw.end(),greater<ValuePerWeight>());//降序	sort(vpw.begin(), vpw.end(),less<ValuePerWeight>());//升序
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
	double dark_channel;	//暗通道
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

void test();

//测试人脸对称性
void testSymmetrical();

//测试清晰度
void testSharpness();

//图片模糊（平滑）
void testBlur();

//************************************
// 函数名称: testBrightness
// 函数说明:  测试光照
// 作    者: Juliwei
// 完成日期: 2014/07/15
// 返 回 值: void
//************************************
void testBrightness();

void testBrightness1();

//截取人脸
void GetFace();

//canny边缘
void GetCanny();

//用SQI做光照
void testSQI();
 
//
//************************************
// 函数名称: zFindContours
// 函数说明:  寻找轮廓
// 作    者: Juliwei
// 完成日期: 2014/07/19
// 返 回 值: Mat 3通道
// 参    数:Mat image单通道
//************************************
Mat zFindContours(Mat image);

//************************************
// 函数名称: zFindEdge
// 函数说明:  sobel、Laplacion、canny
// 作    者: Juliwei
// 完成日期: 2014/07/19
// 返 回 值: Mat	二值边缘图像
// 参    数:Mat image	灰度图
// 参    数:int flag 0=sobel；1=laplacion；2=canny
//参    数:int kernel_size 核大小
//************************************
Mat zFindEdge(Mat image,  int kernel_size, int flag);

//************************************
// 函数名称: zFindBlobs
// 函数说明:  块检测
// 作    者: Juliwei
// 完成日期: 2014/07/21
// 返 回 值: Mat
// 参    数:Mat image
//************************************
Mat zFindBlobs(Mat image);

//************************************
// 函数名称: zSetDark
// 函数说明:  将低于一定像素值的点置0
// 作    者: Juliwei
// 完成日期: 2014/07/21
// 返 回 值: Mat
// 参    数:CONST Mat & image
// 参    数:double alpha threshold=alpha*(max-min)
//************************************
Mat zSetDark(Mat image, double alpha);


//************************************
// 函数名称: zMorphology
// 函数说明:  //形态学处理
// 作    者: Juliwei
// 完成日期: 2014/07/28
// 返 回 值: Mat
// 参    数:Mat image
// 参    数:int flag+2 = 2-开 3-闭 4-GRADIENT 5-白 6-黑
//************************************
Mat zMorphology(Mat image, int flag);

//************************************
// 函数名称: read_txt
// 函数说明:  读取读入中科院说明文本内容txt
// 作    者: Juliwei
// 完成日期: 2014/08/05
// 返 回 值: void
// 参    数:const string & filename
// 参    数:vector<string> & vpaths文件名
// 参    数:vector<Vec4i> & vpoints左右眼睛坐标点
// 参    数:char separator分隔符
//************************************
void read_txt(const string& filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator);
//************************************
// 函数名称: test_cas_light
// 函数说明:  简单的测试中科院光照库
// 作    者: Juliwei
// 完成日期: 2014/08/05
// 返 回 值: 
//************************************
void test_cas_light();

//
//************************************
// 函数名称: test_cas_pose
// 函数说明:  测试中科院角度子库
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: void
//************************************
void test_cas_pose();


//
//************************************
// 函数名称: test_cas_symmetry
// 函数说明:  测试中科院光照和姿态子库对称性
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: void
//************************************
//由于无法根据两眼坐标大致圈出人脸区域，所以背景干扰大，效果不好
void test_cas_symmetry();


//************************************
// 函数名称: GetPointDistance
// 函数说明:  获得两点间距离
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: double
// 参    数:Point2f p1
// 参    数:Point2f p2
//************************************
double GetPointDistance(Point2f p1,Point2f p2);

//************************************
// 函数名称: alignFace
// 函数说明:  根据两眼坐标，人脸归一化
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: cv::Mat
// 参    数:Mat image
// 参    数:Point2f potLeft真实的左眼，观察者的右边
// 参    数:Point2f potRight真实的右眼
//************************************
Mat alignFace(Mat image,Point2f potLeft, Point2f potRight);

//************************************
// 函数名称: getFaceFromTxt
// 函数说明:  从txt中获取人脸所需所有信息
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: void
// 参    数:const string & filename
// 参    数:vector<Face> & vfaces
//************************************
void getFaceFromTxt(const string& filename, vector<Face>& vfaces);

//************************************
// 函数名称: computerLightQuality
// 函数说明:  计算图片光照质量,分单通道、3通道
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: LightQuality
// 参    数:Mat image输入人脸图片
//************************************
LightQuality computerLightQuality(Mat image);

//************************************
// 函数名称: computerPoseQuality
// 函数说明:  计算图片姿态质量
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: PoseQuality
// 参    数:Vec4f eyePoints两眼坐标点
//************************************
PoseQuality computerPoseQuality(Vec4f eyePoints);

//
//************************************
// 函数名称: computerSymmetryQuality
// 函数说明: 计算图片对称性 
// 作    者: Juliwei
// 完成日期: 2014/08/12
// 返 回 值: LightQuality
// 参    数:Mat image
//************************************//
SymmetryQuality computerSymmetryQuality(Mat image);

//
//************************************
// 函数名称: getPelFace
// 函数说明:  获得pel人脸库信息
// 作    者: Juliwei
// 完成日期: 2014/08/13
// 返 回 值: void
// 参    数:string directory文件夹目录
// 参    数:vector<Face> & vfaces存放文件名，灰度图
//************************************
void getPelFace(string directory, vector<Face>& vfaces);
//
//************************************
// 函数名称: getFilenameFromPath
// 函数说明:  从路径获取文件名
// 作    者: Juliwei
// 完成日期: 2014/08/13
// 返 回 值: std::string文件名
// 参    数:string path文件路径
//************************************
string getFilenameFromPath(string path);
