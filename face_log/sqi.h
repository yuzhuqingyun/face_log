#include "opencv2\opencv.hpp"
#include "quality.h"
using namespace cv;


//************************************
// 函数名称: DarkChannel
// 函数说明:  获取暗通道
// 作    者: Juliwei
// 完成日期: 2014/07/17
// 返 回 值: Mat
// 参    数:const Mat & image	单通道
// 参    数:int patch	核大小，为偶
// 参    数:int flag	为0求最小；为1求最大
//************************************
Mat DarkChannel(const Mat& image,int patch, int flag);

//************************************
// 函数名称: DarkImage
// 函数说明:  合并多通道的暗通道
// 作    者: Juliwei
// 完成日期: 2014/07/17
// 返 回 值: Mat
// 参    数:const Mat & image	为多通道
// 参    数:int patch	核大小
// 参    数:int flag	0求最小， 1求最大
//************************************
Mat DarkImage(const Mat& image, int patch, int flag);

//************************************
// 函数名称: DogFilter
// 函数说明:  差分高斯滤波
// 作    者: Juliwei
// 完成日期: 2014/07/17
// 返 回 值: Mat
// 参    数:const Mat & image
// 参    数:Size dsize	核大小
//************************************
Mat DogFilter(const Mat& image, Size dsize);

//************************************
// 函数名称: ChannelDiff
// 函数说明:  灰度图 减去多通道差值的平均
// 作    者: Juliwei
// 完成日期: 2014/07/18
// 返 回 值: cv::Mat单通道
// 参    数:Mat image3通道
//************************************
Mat ChannelDiff(Mat image);
