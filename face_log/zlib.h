#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"

using namespace cv;

//����LBPֱ��ͼ
Mat ComputerLBPH(Mat src);
//�Ƚ���ͼƬ��LBPH
double compareLBPH(Mat image1, Mat image2);
