#include "stdafx.h"
#include "train.h"
#include "quality.h"
#include "lbph.h"


void ReadTxt(const string filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator);	//读取读入中科院说明文本内容txt
void ReadFilename(const string filename, int& id, string& sign);	//从文件名中提取信息id和光照的标号
void GetFaceFromTxt(const string txtname, vector<Face>& vfaces);	//从txt获取人脸信息
int GetFaceFromFilename(const string filePath, const Vec4f point, Face& faces);

//************************************
// 函数名称: ReadTxt
// 函数说明:  读取读入中科院说明文本内容txt
// 作    者: Juliwei
// 完成日期: 2014/08/05
// 返 回 值: void
// 参    数:const string & filename	txt的路径
// 参    数:vector<string> & vpaths文件名
// 参    数:vector<Vec4i> & vpoints左右眼睛坐标点
// 参    数:char separator分隔符
//************************************
void ReadTxt(const string filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator)
{
	ifstream file(filename.c_str(), ifstream::in);
	if (!file) 
	{
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, pt0, pt1, pt2, pt3;
	while (getline(file, line)) 
	{
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, pt0, separator);
		getline(liness, pt1, separator);
		getline(liness, pt2, separator);
		getline(liness, pt3, separator);
		if(!path.empty() && !pt0.empty()&& !pt1.empty()&& !pt2.empty()&& !pt3.empty()) 
		{
			Vec4f vtemp;
			vtemp[0] = atof(pt0.c_str());
			vtemp[1] = atof(pt1.c_str());
			vtemp[2] = atof(pt2.c_str());
			vtemp[3] = atof(pt3.c_str());
			vpaths.push_back(path);
			vpoints.push_back(vtemp);
		}
	}
}


//从文件名中提取信息id和光照的标号
void ReadFilename(const string filename, int& id, string& sign)
{	
	id = atoi(filename.substr(3, 6).c_str());
	sign = filename.substr(10, filename.size()-10-4);	//光照标志(11, 5)；姿态(17, 5)
}

int GetFaceFromFilename(const string filePath, const Vec4f point, Face& faces)
{
	string sign;
	int id;
	string filename = GetFilenameFromPath(filePath);
	ReadFilename(filename, id, sign);
	Mat image = imread(filePath, 0);
	if (image.empty()) return 0;	//图片读取失败，返回
	Point2f pLeft = Point2f(point[0],point[1]);
	Point2f pRight = Point2f(point[2],point[3]);
	Mat dst = AlignFace(image, pRight, pLeft);	//尺寸归一化
	faces.filename = filePath;
	faces.id = id;
	faces.sign = sign;
	faces.eyePoints = point;
	faces.face = dst;
	faces.value = 0.0;
	return 1;
}

//************************************
// 函数名称: getFaceFromTxt
// 函数说明:  从txt中获取人脸所需所有信息
// 作    者: Juliwei
// 完成日期: 2014/08/06
// 返 回 值: void
// 参    数:const string & filename文件路径
// 参    数:vector<Face> & vfaces
//************************************
/*"G:\\Picture\\Lighting\\FaceFP_2.txt"*/
void GetFaceFromTxt(const string txtname, vector<Face>& vfaces)
{
	size_t maxSize = 1000;	///vpaths.size()
	string sign;
	int id;
	vector<string> vpaths(maxSize);	//文件路径
	vector<Vec4f> vpoints(maxSize);	//两眼坐标点
	vector<int> vids(maxSize);		//人脸ID号
	vector<string> vsigns(maxSize);	//标记
	vector<Mat> vmfaces(maxSize);		//归一化人脸区域
	ReadTxt(txtname, vpaths, vpoints, ' ');	//读入文件名和两眼坐标
	for (size_t i = 0; i<maxSize; i++)	//分解文件名，得到ID和状态标志
	{
		string path = vpaths[i];
		ReadFilename(path, id, sign);
		vids.push_back(id);
		vsigns.push_back(sign);
	}
	for (size_t j = 0; j<maxSize; j++)
	{
		string long_path = "G:\\Picture\\pose\\" + vpaths[j].substr(0, vpaths[j].size()-1) + ".jpg";	//分割得到的文件名多出一个空格，-1去除
		Mat image = imread(long_path, 0);
		if (!image.empty())
		{
			Point2f pLeft = Point2f(vpoints[j][0],vpoints[j][1]);
			Point2f pRight = Point2f(vpoints[j][2],vpoints[j][3]);
#if 0	//光照时需归一化
			Mat dst = alignFace(image, pRight, pLeft);	//尺寸归一化

#elif 0	//头部姿态标记两眼距离
			Mat dst = image;
			line(dst, pLeft, pRight, Scalar(0, 0, 0), 2, 8);
			circle(dst, pLeft, 2, Scalar(255, 255, 255), 2, 8);
			circle(dst, pRight, 2, Scalar(255, 255, 255), 2, 8);

#else 1	//对称性
			Mat dst = AlignFace(image, pRight, pLeft);	//尺寸归一化

#endif
			vmfaces.push_back(dst);
	/*		imshow("image", dst);
			waitKey(0);*/
		}
	}
	//将人脸各项信息存入Face
	for (size_t m = 0; m<maxSize; m++)
	{
		Face mface;
		mface.filename = vpaths[m];
		mface.id = vids[m];
		mface.sign = vsigns[m];
		mface.eyePoints = vpoints[m];
		mface.face = vmfaces[m];
		mface.value = 0.0;
		vfaces.push_back(mface);
	}
}


int TrainQualityAssessment(const string strDirectory)
{
	string strNormalTxt = strDirectory + "normal_image.txt";	//标准图路径
	string strLogTxt = strDirectory + "image_log.txt";	//图片长廊路径
	vector<string> vnormalPath, vlogPath;
	vector<Vec4f> vnormalPoint, vlogPoint;
	vector<Face> vnormalFaces, vlogFaces;
	//从txt中获取图片路径和两眼坐标
	ReadTxt(strNormalTxt, vnormalPath, vnormalPoint, ' ');	
	ReadTxt(strLogTxt, vlogPath, vlogPoint, ' ');
	//从图片路径和坐标获得人脸信息Face
	for (size_t i = 0; i<vnormalPath.size(); i++)
	{
		Face normalFace;
		GetFaceFromFilename(vnormalPath[i], vnormalPoint[i], normalFace);
		vnormalFaces.push_back(normalFace);
	}
	for (size_t j = 0; j<vnormalPath.size(); j++)
	{
		Face logFace;
		GetFaceFromFilename(vlogPath[j], vlogPoint[j], logFace);
		vlogFaces.push_back(logFace);
	}
		
	//从log图片序列中找到对应的normal图
	vector<Face> vlastNormalFace;
	vector<vector<Face>> vlastLogFace;
	for (size_t m=0; m<vnormalFaces.size();m++)
	{
		vector<Face> vtempFace;
		for (size_t n=0; n<vlogFaces.size(); n++)
		{
			if (vnormalFaces[m].id == vlogFaces[n].id)	//如果是同一个人
			{
				vtempFace.push_back(vlogFaces[n]);
			}
		}
		if (!vtempFace.empty())		//normal图能够在log图序列中找到对应
		{
			vlastNormalFace.push_back(vnormalFaces[m]);
			vlastLogFace.push_back(vtempFace);
		}
	}
	if (vlastNormalFace.empty() || vlastLogFace.empty() || (vlastNormalFace.size() != vlastLogFace.size()))
	{
		string error_message = "无法建立标准图和图像序列的映射";
		CV_Error(CV_StsBadArg, error_message);
	}

	//计算每幅图的质量和匹配度
	for (size_t i=0; i<vlastLogFace.size(); i++)
	{
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			
			vlastLogFace[i][j].quality = ComputerQuality(vlastLogFace[i][j]);	//质量
			Mat stdLbph = ComputerLBPH(vlastNormalFace[i].face);
			Mat dstLbph = ComputerLBPH(vlastLogFace[i][j].face);
			vlastLogFace[i][j].value = CompareLBPH(stdLbph, dstLbph);

		}
	}

	return 1;
	
}