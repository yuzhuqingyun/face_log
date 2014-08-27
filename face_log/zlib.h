#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"

using namespace cv;

//计算LBP直方图
Mat ComputerLBPH(Mat src);
//比较两图片的LBPH
double compareLBPH(Mat image1, Mat image2);
