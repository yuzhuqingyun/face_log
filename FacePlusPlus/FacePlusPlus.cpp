// FacePlusPlus.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <iostream>
using namespace std;
#include "HttpClient.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include <sstream>
#include <Windows.h>
#include <cstring>

using namespace cv;
using namespace std;
std::string strResult;

std::string HTTPRESULT;

void Write_data(void* buffer, size_t size, size_t nmemb, void* user_p){
	HTTPRESULT += (const char*)buffer;
}

//去除首位空格
string& trim(string &s) 
{
	if (s.empty()) 
	{
		return s;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

//
Point2f Get_xy(string result, size_t startPos)
{
	Point2f pos;
	size_t pos_x_start = result.find_first_of("x", startPos);	//find x after eye_left
	size_t pos_x_end = result.find_first_of(",", pos_x_start);	//find , after x
	string strpos_x = result.substr(pos_x_start+3, pos_x_end-pos_x_start-3);
	strpos_x = trim(strpos_x);	//去除首位空格
	pos.x = atof(strpos_x.c_str());
	cout<<pos.x<<endl;

	size_t pos_y_start = result.find_first_of("y", pos_x_end);	//find x after eye_left
	size_t pos_y_end = result.find_first_of("}", pos_y_start);	//find , after x
	string strpos_y = result.substr(pos_y_start+3, pos_y_end-pos_y_start-3);
	strpos_y = trim(strpos_y);	//去除首位空格
	pos.y = atof(strpos_y.c_str());
	cout<<pos.y<<endl;
	return pos;
}

Size2f GetFaceSize(string result)
{
	Size2f pos;
	size_t width_start = HTTPRESULT.find("width");	//width
	//size_t width_start = result.find_first_of("x", widthPos);	//find x after eye_left
	size_t width_end = result.find_first_of("}", width_start);	//find , after x
	string strpos_width = result.substr(width_start+7, width_end-width_start-7);
	strpos_width = trim(strpos_width);	//去除首位空格
	pos.width = atof(strpos_width.c_str());
	cout<<pos.width<<endl;

	size_t height_start = HTTPRESULT.find("height");	//height
	//size_t height_start = result.find_first_of("x", heightPos);	//find x after eye_left
	size_t height_end = result.find_first_of(",", height_start);	//find , after x
	string strpos_height = result.substr(height_start+8, height_end-height_start-8);
	strpos_height = trim(strpos_height);	//去除首位空格
	pos.height = atof(strpos_height.c_str());
	cout<<pos.height<<endl;

	return pos;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	HttpClient httpClient;

	//httpClient.Post(HttpClient::URL_DETECT,"url=http://g.hiphotos.baidu.com/image/pic/item/0df431adcbef7609758498962cdda3cc7cd99e2f.jpg&api_secret=自己secret&api_key=自己key",strResult);
	std::map<std::string,std::string> mapPost;
	//mapPost["url"] = "http://g.hiphotos.baidu.com/image/pic/item/0df431adcbef7609758498962cdda3cc7cd99e2f.jpg";
	mapPost["img"] = "c:\\test.jpg";
	httpClient.Post(HttpClient::URL_DETECT,mapPost,strResult);	
	std::cout<<std::endl<<strResult<<std::endl;
	*/

	string faceImageName = "G:\\Picture\\PEL2\\1-04b.jpg";
	Mat faceImage = imread(faceImageName, 1);
	
	CURL *curl = curl_easy_init();
	CURLcode res = curl_global_init(CURL_GLOBAL_WIN32);

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	//        struct curl_slist *headerlist=NULL;
	//        static const char buf[] = "Expect:";

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "api_key", 
		CURLFORM_COPYCONTENTS, "aaf0f4ada35581441e2c70787fad20b5",/*"自己的key",*/ 
		CURLFORM_END);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "api_secret", 
		CURLFORM_COPYCONTENTS, "_zspOfKqFe1UjD_N1_y-NhEC__KYML9j",/*"自己的secret",*/
		CURLFORM_END);
	/*
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "img", 
		CURLFORM_FILE, "c:\\test.png", 
		CURLFORM_END);
		*/
	
	char* file_data = NULL;
	long file_size = 0;
	
	FILE* fp = fopen(faceImageName.c_str(),"rb");
	if (fp)
	{		
		fseek(fp, 0, SEEK_END);	//重定向流指针到文件结尾
		file_size = ftell(fp);	//返回当前指针位置，即文件大小
		fseek(fp, 0, SEEK_SET);	////重定向流指针到文件开头
		file_data = new char[file_size+1];
		fread(file_data,1,file_size,fp);	//将文件内容存储进char数组
		cout<<file_data<<endl;
		fclose(fp);
	}
	
	
	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "img",
		CURLFORM_BUFFER, "test.jpg",
		CURLFORM_BUFFERPTR, file_data,
		CURLFORM_BUFFERLENGTH, file_size,
		CURLFORM_CONTENTTYPE, "image/jpeg",
		CURLFORM_END);
		
	
	if(curl) {
		/* what URL that receives this POST */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://apicn.faceplusplus.com/v2/detection/detect");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &HTTPRESULT);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Write_data);

		char error[1024];
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);

		res = curl_easy_perform(curl);
		if(res != CURLE_OK) cout<<endl<<error<<endl;
	}
	curl_easy_cleanup(curl);
	curl_formfree(formpost);
	cout<<endl<<HTTPRESULT<<endl;

	size_t centerPos = HTTPRESULT.find("center");	//center
	Point2f center = Get_xy(HTTPRESULT, centerPos);

	Size2f faceSize = GetFaceSize(HTTPRESULT);

	size_t leftEyePos = HTTPRESULT.find("eye_left");	//eye_left
	Point2f leftEye = Get_xy(HTTPRESULT, leftEyePos);

	size_t rightEyePos = HTTPRESULT.find("eye_right");	//eye_right
	Point2f rightEye = Get_xy(HTTPRESULT, rightEyePos);

	size_t leftMouthPos = HTTPRESULT.find("mouth_left");	//mouth_left
	Point2f leftMouth = Get_xy(HTTPRESULT, leftMouthPos);

	size_t rightMouthPos = HTTPRESULT.find("mouth_right");	//mouth_right
	Point2f rightMouth = Get_xy(HTTPRESULT, rightMouthPos);

	size_t nosePos = HTTPRESULT.find("nose");	//nose
	Point2f nose = Get_xy(HTTPRESULT, nosePos);

	//Mat faceImage = imread("G:\\Picture\\PEL2\\1-04b.jpg", 1);
	Rect rec;
	rec.width = (int)(faceSize.width/100*320);
	rec.height = (int)(faceSize.height/100*400);
	rec.x = (int)( (center.x-faceSize.width/2.0)/100*320 );
	rec.y = (int)( (center.y-faceSize.height/2.0)/100*400 );
	center.x = center.x /100*320;
	center.y = center.y /100*400;
	leftEye.x = leftEye.x /100*320;
	leftEye.y = leftEye.y /100*400;
	rightEye.x = rightEye.x /100*320;
	rightEye.y = rightEye.y /100*400;
	leftMouth.x = leftMouth.x /100*320;
	leftMouth.y = leftMouth.y /100*400;
	rightMouth.x = rightMouth.x /100*320;
	rightMouth.y = rightMouth.y /100*400;
	nose.x = nose.x /100*320;
	nose.y = nose.y /100*400;

	circle(faceImage, center, 1, Scalar(0, 255, 0), -1);
	circle(faceImage, leftEye, 1, Scalar(0, 255, 0), -1);
	circle(faceImage, rightEye, 1, Scalar(0, 255, 0), -1);
	circle(faceImage, leftMouth, 1, Scalar(0, 255, 0), -1);
	circle(faceImage, rightMouth, 1, Scalar(0, 255, 0), -1);
	circle(faceImage, nose, 1, Scalar(0, 255, 0), -1);
	rectangle(faceImage, rec, Scalar(255, 0, 0), 1);
	imshow("face", faceImage);
	waitKey(0);

	if(file_data != NULL)
		delete [] file_data;
	system("pause");
	return 0;
}
