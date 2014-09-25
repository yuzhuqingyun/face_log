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
	//Description:��ʼ��ASM��ֻ��Ҫ�ڳ�������ʱ��ʼ��һ��
	//input:���������ڵ��ļ���·��
	//return:��0Ϊ��ʼ���ɹ���0Ϊ��ʼ��ʧ��
	/**************************************************************/
	int InitASM( const char*  datadir);

	/***********************************************************/
	//FunctionName:Get77FeaturePoints
	//Description:��ȡ������77��������
	//input��inputImage��������ͼ��
	//       landmark ����2*77 �ĸ��������飬�� landmark[2*77]
	//output:landmark���77������������꣬��ʽΪx0,y0 x1,y1, x2,y2,........
	//retrun:����1���ɹ���λ�ؼ��� ����0����λʧ��
	/*************************************************************/
	int Get77KeyPoints(IplImage* inputImage,std::vector<cv::Point>& landmarks);

	/************************************************************/
	//FunctionName:uninitASM
	//Description:�ͷ�ASM��ֻ��Ҫ�ڳ������ʱ�ͷ�һ��
	//return:1Ϊ�ͷųɹ���0Ϊ�ͷ�ʧ��
	/**************************************************************/
	int UninitASM( const char*  datadir) ;
	~Cyw_ASM(){};

};


#endif