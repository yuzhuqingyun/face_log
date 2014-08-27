#include "opencv2\opencv.hpp"
#include "quality.h"
using namespace cv;


//************************************
// ��������: DarkChannel
// ����˵��:  ��ȡ��ͨ��
// ��    ��: Juliwei
// �������: 2014/07/17
// �� �� ֵ: Mat
// ��    ��:const Mat & image	��ͨ��
// ��    ��:int patch	�˴�С��Ϊż
// ��    ��:int flag	Ϊ0����С��Ϊ1�����
//************************************
Mat DarkChannel(const Mat& image,int patch, int flag);

//************************************
// ��������: DarkImage
// ����˵��:  �ϲ���ͨ���İ�ͨ��
// ��    ��: Juliwei
// �������: 2014/07/17
// �� �� ֵ: Mat
// ��    ��:const Mat & image	Ϊ��ͨ��
// ��    ��:int patch	�˴�С
// ��    ��:int flag	0����С�� 1�����
//************************************
Mat DarkImage(const Mat& image, int patch, int flag);

//************************************
// ��������: DogFilter
// ����˵��:  ��ָ�˹�˲�
// ��    ��: Juliwei
// �������: 2014/07/17
// �� �� ֵ: Mat
// ��    ��:const Mat & image
// ��    ��:Size dsize	�˴�С
//************************************
Mat DogFilter(const Mat& image, Size dsize);

//************************************
// ��������: ChannelDiff
// ����˵��:  �Ҷ�ͼ ��ȥ��ͨ����ֵ��ƽ��
// ��    ��: Juliwei
// �������: 2014/07/18
// �� �� ֵ: cv::Mat��ͨ��
// ��    ��:Mat image3ͨ��
//************************************
Mat ChannelDiff(Mat image);
