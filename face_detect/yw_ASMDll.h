#ifndef YW_ASM_DLL_H
#define YW_ASM_DLL_H

#include "opencv2\highgui\highgui.hpp"  
#include <opencv2\opencv.hpp> 
#include "vector"


//#define EXPORT
#ifdef EXPORT
#define YW_ASM_API __declspec(dllexport)
#else
#define YW_ASM_API __declspec(dllimport)
#endif


class YW_ASM_API Cyw_ASM
{
public:
	Cyw_ASM(){};
	/************************************************************/
	//FunctionName:initASM
	//Description:初始化ASM，只需要在程序启动时初始化一次
	//input:分类器所在的文件夹路径
	//return:非0为初始化成功，0为初始化失败
	/**************************************************************/
	int InitASM( const char*  datadir);

	/***********************************************************/
	//FunctionName:Get77FeaturePoints
	//Description:获取人脸的77个特征点
	//input：inputImage输入人脸图像
	//       landmark 输入2*77 的浮点型数组，即 landmark[2*77]
	//output:landmark输出77个特征点的坐标，形式为x0,y0 x1,y1, x2,y2,........
	//retrun:返回1，成功定位关键点 返回0，定位失败
	/*************************************************************/
	int Get77KeyPoints(IplImage* inputImage,std::vector<cv::Point>& landmarks);

	/************************************************************/
	//FunctionName:uninitASM
	//Description:释放ASM，只需要在程序结束时释放一次
	//return:1为释放成功，0为释放失败
	/**************************************************************/
	int UninitASM( const char*  datadir) ;
	~Cyw_ASM(){};

};


#endif