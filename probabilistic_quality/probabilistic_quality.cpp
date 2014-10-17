// probabilistic_quality.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "quality.h"
#include "GMM.h"
using namespace cv;


int _tmain(int argc, _TCHAR* argv[])
{
	Mat image = imread("G:\\Picture\\Lena.jpg", 0);
	CV_Assert(image.empty() != true);
	vector<Mat> blocks;
	ExtractDCT(image, 8);
	//CutBlocks(image, blocks, 8, 4);
	
	return 0;
}

