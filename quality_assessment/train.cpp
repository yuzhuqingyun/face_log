#include "stdafx.h"
#include "train.h"
#include "quality.h"
#include "lbph.h"


void ReadTxt(const string filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator);	//��ȡ�����п�Ժ˵���ı�����txt
void ReadFilename(const string filename, int& id, string& sign);	//���ļ�������ȡ��Ϣid�͹��յı��
void GetFaceFromTxt(const string txtname, vector<Face>& vfaces);	//��txt��ȡ������Ϣ
int GetFaceFromFilename(const string filePath, const Vec4f point, Face& faces);

//************************************
// ��������: ReadTxt
// ����˵��:  ��ȡ�����п�Ժ˵���ı�����txt
// ��    ��: Juliwei
// �������: 2014/08/05
// �� �� ֵ: void
// ��    ��:const string & filename	txt��·��
// ��    ��:vector<string> & vpaths�ļ���
// ��    ��:vector<Vec4i> & vpoints�����۾������
// ��    ��:char separator�ָ���
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


//���ļ�������ȡ��Ϣid�͹��յı��
void ReadFilename(const string filename, int& id, string& sign)
{	
	id = atoi(filename.substr(3, 6).c_str());
	sign = filename.substr(10, filename.size()-10-4);	//���ձ�־(11, 5)����̬(17, 5)
}

int GetFaceFromFilename(const string filePath, const Vec4f point, Face& faces)
{
	string sign;
	int id;
	string filename = GetFilenameFromPath(filePath);
	ReadFilename(filename, id, sign);
	Mat image = imread(filePath, 0);
	if (image.empty()) return 0;	//ͼƬ��ȡʧ�ܣ�����
	Point2f pLeft = Point2f(point[0],point[1]);
	Point2f pRight = Point2f(point[2],point[3]);
	Mat dst = AlignFace(image, pRight, pLeft);	//�ߴ��һ��
	faces.filename = filePath;
	faces.id = id;
	faces.sign = sign;
	faces.eyePoints = point;
	faces.face = dst;
	faces.value = 0.0;
	return 1;
}

//************************************
// ��������: getFaceFromTxt
// ����˵��:  ��txt�л�ȡ��������������Ϣ
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: void
// ��    ��:const string & filename�ļ�·��
// ��    ��:vector<Face> & vfaces
//************************************
/*"G:\\Picture\\Lighting\\FaceFP_2.txt"*/
void GetFaceFromTxt(const string txtname, vector<Face>& vfaces)
{
	size_t maxSize = 1000;	///vpaths.size()
	string sign;
	int id;
	vector<string> vpaths(maxSize);	//�ļ�·��
	vector<Vec4f> vpoints(maxSize);	//���������
	vector<int> vids(maxSize);		//����ID��
	vector<string> vsigns(maxSize);	//���
	vector<Mat> vmfaces(maxSize);		//��һ����������
	ReadTxt(txtname, vpaths, vpoints, ' ');	//�����ļ�������������
	for (size_t i = 0; i<maxSize; i++)	//�ֽ��ļ������õ�ID��״̬��־
	{
		string path = vpaths[i];
		ReadFilename(path, id, sign);
		vids.push_back(id);
		vsigns.push_back(sign);
	}
	for (size_t j = 0; j<maxSize; j++)
	{
		string long_path = "G:\\Picture\\pose\\" + vpaths[j].substr(0, vpaths[j].size()-1) + ".jpg";	//�ָ�õ����ļ������һ���ո�-1ȥ��
		Mat image = imread(long_path, 0);
		if (!image.empty())
		{
			Point2f pLeft = Point2f(vpoints[j][0],vpoints[j][1]);
			Point2f pRight = Point2f(vpoints[j][2],vpoints[j][3]);
#if 0	//����ʱ���һ��
			Mat dst = alignFace(image, pRight, pLeft);	//�ߴ��һ��

#elif 0	//ͷ����̬������۾���
			Mat dst = image;
			line(dst, pLeft, pRight, Scalar(0, 0, 0), 2, 8);
			circle(dst, pLeft, 2, Scalar(255, 255, 255), 2, 8);
			circle(dst, pRight, 2, Scalar(255, 255, 255), 2, 8);

#else 1	//�Գ���
			Mat dst = AlignFace(image, pRight, pLeft);	//�ߴ��һ��

#endif
			vmfaces.push_back(dst);
	/*		imshow("image", dst);
			waitKey(0);*/
		}
	}
	//������������Ϣ����Face
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
	string strNormalTxt = strDirectory + "normal_image.txt";	//��׼ͼ·��
	string strLogTxt = strDirectory + "image_log.txt";	//ͼƬ����·��
	vector<string> vnormalPath, vlogPath;
	vector<Vec4f> vnormalPoint, vlogPoint;
	vector<Face> vnormalFaces, vlogFaces;
	//��txt�л�ȡͼƬ·������������
	ReadTxt(strNormalTxt, vnormalPath, vnormalPoint, ' ');	
	ReadTxt(strLogTxt, vlogPath, vlogPoint, ' ');
	//��ͼƬ·����������������ϢFace
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
		
	//��logͼƬ�������ҵ���Ӧ��normalͼ
	vector<Face> vlastNormalFace;
	vector<vector<Face>> vlastLogFace;
	for (size_t m=0; m<vnormalFaces.size();m++)
	{
		vector<Face> vtempFace;
		for (size_t n=0; n<vlogFaces.size(); n++)
		{
			if (vnormalFaces[m].id == vlogFaces[n].id)	//�����ͬһ����
			{
				vtempFace.push_back(vlogFaces[n]);
			}
		}
		if (!vtempFace.empty())		//normalͼ�ܹ���logͼ�������ҵ���Ӧ
		{
			vlastNormalFace.push_back(vnormalFaces[m]);
			vlastLogFace.push_back(vtempFace);
		}
	}
	if (vlastNormalFace.empty() || vlastLogFace.empty() || (vlastNormalFace.size() != vlastLogFace.size()))
	{
		string error_message = "�޷�������׼ͼ��ͼ�����е�ӳ��";
		CV_Error(CV_StsBadArg, error_message);
	}

	//����ÿ��ͼ��������ƥ���
	for (size_t i=0; i<vlastLogFace.size(); i++)
	{
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			
			vlastLogFace[i][j].quality = ComputerQuality(vlastLogFace[i][j]);	//����
			Mat stdLbph = ComputerLBPH(vlastNormalFace[i].face);
			Mat dstLbph = ComputerLBPH(vlastLogFace[i][j].face);
			vlastLogFace[i][j].value = CompareLBPH(stdLbph, dstLbph);

		}
	}

	return 1;
	
}