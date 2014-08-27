#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\opencv.hpp"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

//�洢������Ϣ
typedef struct Face
{
	string filename;	//�ļ���
	string sign;		//ʵ���������
	int id;				//����ID
	Vec4f eyePoints;	//���������
	Mat face;		//��һ������
	double value;	//ָ���С
	//���ز�����< >Ϊsort��׼��
	//eg. sort(vpw.begin(), vpw.end(),greater<ValuePerWeight>());//����	sort(vpw.begin(), vpw.end(),less<ValuePerWeight>());//����
	bool operator <(const Face &other)const   //��������
	{
		return value>other.value;
	}
	//����
	bool operator >(const Face &other)const   //��������
	{
		return value>other.value;
	}
} Face;


//�洢���е���������ָ���������
typedef struct Quality 
{
	double light;
	double symmetry;
	double sharpness;
	double pose;
	double expression;
} Quality;

//�洢���еĹ�������
typedef struct LightQuality 
{
	double dark_spot;	//�������
	double y;	//Y������ֵ
	double dark_channel;	//��ͨ��
	double equalization;	//�ֿ������ԭͼ�Ƚ�
	double mean;	//�Ҷ�ͼ��ֵ������δ�����ȷ��
} LightQuality;

//�洢���еĽǶ�����
typedef struct PoseQuality 
{
	double eyes_angle;
	double eyes_length;
} PoseQuality;

//�洢���еĶԳ���ָ��
typedef struct SymmetryQuality 
{
	double hist;
	double gabor;
	double lbph;
} SymmetryQuality;

void test();

//���������Գ���
void testSymmetrical();

//����������
void testSharpness();

//ͼƬģ����ƽ����
void testBlur();

//************************************
// ��������: testBrightness
// ����˵��:  ���Թ���
// ��    ��: Juliwei
// �������: 2014/07/15
// �� �� ֵ: void
//************************************
void testBrightness();

void testBrightness1();

//��ȡ����
void GetFace();

//canny��Ե
void GetCanny();

//��SQI������
void testSQI();
 
//
//************************************
// ��������: zFindContours
// ����˵��:  Ѱ������
// ��    ��: Juliwei
// �������: 2014/07/19
// �� �� ֵ: Mat 3ͨ��
// ��    ��:Mat image��ͨ��
//************************************
Mat zFindContours(Mat image);

//************************************
// ��������: zFindEdge
// ����˵��:  sobel��Laplacion��canny
// ��    ��: Juliwei
// �������: 2014/07/19
// �� �� ֵ: Mat	��ֵ��Եͼ��
// ��    ��:Mat image	�Ҷ�ͼ
// ��    ��:int flag 0=sobel��1=laplacion��2=canny
//��    ��:int kernel_size �˴�С
//************************************
Mat zFindEdge(Mat image,  int kernel_size, int flag);

//************************************
// ��������: zFindBlobs
// ����˵��:  ����
// ��    ��: Juliwei
// �������: 2014/07/21
// �� �� ֵ: Mat
// ��    ��:Mat image
//************************************
Mat zFindBlobs(Mat image);

//************************************
// ��������: zSetDark
// ����˵��:  ������һ������ֵ�ĵ���0
// ��    ��: Juliwei
// �������: 2014/07/21
// �� �� ֵ: Mat
// ��    ��:CONST Mat & image
// ��    ��:double alpha threshold=alpha*(max-min)
//************************************
Mat zSetDark(Mat image, double alpha);


//************************************
// ��������: zMorphology
// ����˵��:  //��̬ѧ����
// ��    ��: Juliwei
// �������: 2014/07/28
// �� �� ֵ: Mat
// ��    ��:Mat image
// ��    ��:int flag+2 = 2-�� 3-�� 4-GRADIENT 5-�� 6-��
//************************************
Mat zMorphology(Mat image, int flag);

//************************************
// ��������: read_txt
// ����˵��:  ��ȡ�����п�Ժ˵���ı�����txt
// ��    ��: Juliwei
// �������: 2014/08/05
// �� �� ֵ: void
// ��    ��:const string & filename
// ��    ��:vector<string> & vpaths�ļ���
// ��    ��:vector<Vec4i> & vpoints�����۾������
// ��    ��:char separator�ָ���
//************************************
void read_txt(const string& filename, vector<string>& vpaths,vector<Vec4f>& vpoints, char separator);
//************************************
// ��������: test_cas_light
// ����˵��:  �򵥵Ĳ����п�Ժ���տ�
// ��    ��: Juliwei
// �������: 2014/08/05
// �� �� ֵ: 
//************************************
void test_cas_light();

//
//************************************
// ��������: test_cas_pose
// ����˵��:  �����п�Ժ�Ƕ��ӿ�
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: void
//************************************
void test_cas_pose();


//
//************************************
// ��������: test_cas_symmetry
// ����˵��:  �����п�Ժ���պ���̬�ӿ�Գ���
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: void
//************************************
//�����޷����������������Ȧ�������������Ա������Ŵ�Ч������
void test_cas_symmetry();


//************************************
// ��������: GetPointDistance
// ����˵��:  �����������
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: double
// ��    ��:Point2f p1
// ��    ��:Point2f p2
//************************************
double GetPointDistance(Point2f p1,Point2f p2);

//************************************
// ��������: alignFace
// ����˵��:  �����������꣬������һ��
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: cv::Mat
// ��    ��:Mat image
// ��    ��:Point2f potLeft��ʵ�����ۣ��۲��ߵ��ұ�
// ��    ��:Point2f potRight��ʵ������
//************************************
Mat alignFace(Mat image,Point2f potLeft, Point2f potRight);

//************************************
// ��������: getFaceFromTxt
// ����˵��:  ��txt�л�ȡ��������������Ϣ
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: void
// ��    ��:const string & filename
// ��    ��:vector<Face> & vfaces
//************************************
void getFaceFromTxt(const string& filename, vector<Face>& vfaces);

//************************************
// ��������: computerLightQuality
// ����˵��:  ����ͼƬ��������,�ֵ�ͨ����3ͨ��
// ��    ��: Juliwei
// �������: 2014/08/06
// �� �� ֵ: LightQuality
// ��    ��:Mat image��������ͼƬ
//************************************
LightQuality computerLightQuality(Mat image);

//************************************
// ��������: computerPoseQuality
// ����˵��:  ����ͼƬ��̬����
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: PoseQuality
// ��    ��:Vec4f eyePoints���������
//************************************
PoseQuality computerPoseQuality(Vec4f eyePoints);

//
//************************************
// ��������: computerSymmetryQuality
// ����˵��: ����ͼƬ�Գ��� 
// ��    ��: Juliwei
// �������: 2014/08/12
// �� �� ֵ: LightQuality
// ��    ��:Mat image
//************************************//
SymmetryQuality computerSymmetryQuality(Mat image);

//
//************************************
// ��������: getPelFace
// ����˵��:  ���pel��������Ϣ
// ��    ��: Juliwei
// �������: 2014/08/13
// �� �� ֵ: void
// ��    ��:string directory�ļ���Ŀ¼
// ��    ��:vector<Face> & vfaces����ļ������Ҷ�ͼ
//************************************
void getPelFace(string directory, vector<Face>& vfaces);
//
//************************************
// ��������: getFilenameFromPath
// ����˵��:  ��·����ȡ�ļ���
// ��    ��: Juliwei
// �������: 2014/08/13
// �� �� ֵ: std::string�ļ���
// ��    ��:string path�ļ�·��
//************************************
string getFilenameFromPath(string path);
