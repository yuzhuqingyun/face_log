#ifndef PROBABILISTIC_QUALITY_QUALITY_H
#define PROBABILISTIC_QUALITY_QUALITY_H

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"

using namespace cv;

void CutBlocks(const Mat& src, vector<Mat>& blocks, int blockSize, int overlap);
Mat ExtractDCT(Mat src, int d);

#endif