#include "stdafx.h"
#include "quality_lib.h"

int NormalValue(vector<Face>& vfaces, int flag=0);	//对Face的Value值归一化



bool MaxFaceValue(const Face &face1, const Face &face2)
{
	if (face1.value > face2.value)
	{
		return true;
	}
	else
		return false;
}
bool MinFaceValue(const Face &face1, const Face &face2)
{
	if (face1.value < face2.value)
	{
		return true;
	}
	else
		return false;
}

//************************************
// 函数名称: vector
// 函数说明: //对Face的Value值归一化
//				0:sample/Max; 1:1- samply/Max; 
// 作    者: Juliwei
// 完成日期: 2014/08/31
// 返 回 值: int
// 参    数:vector<Face> & vfaces
// 参    数:int flag
//************************************
int NormalValue(vector<Face>& vfaces, int flag)
{
	if (vfaces.empty())
	{
		return 0;
	}
	Face maxFace = *max_element(vfaces.begin(), vfaces.end(), MinFaceValue/*MaxFaceValue*/);
	for (size_t i=0; i<vfaces.size();i++)
	{
		switch (flag)
		{
		case 0:
			vfaces[i].value = vfaces[i].value / maxFace.value;
			break;
		case 1:
			vfaces[i].value = 1 - vfaces[i].value / maxFace.value;
			break;
		default:
			vfaces[i].value = vfaces[i].value / maxFace.value;
			break;
		}
	}
}

int NormalizeQuality(vector<Face>& vfaces)
{	
	//归一化dark_spot
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.lightQuality.dark_spot;
		vfaces[i].quality.lightQuality.dark_spot = temp;
	}
	NormalValue(vfaces, 0);
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.lightQuality.dark_spot;
		vfaces[i].quality.lightQuality.dark_spot = temp;
	}

	//归一化equalization
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.lightQuality.equalization;
		vfaces[i].quality.lightQuality.equalization = temp;
	}
	NormalValue(vfaces, 0);
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.lightQuality.equalization;
		vfaces[i].quality.lightQuality.equalization = temp;
	}

	//归一化dark_spot
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.lightQuality.middle_pixel;
		vfaces[i].quality.lightQuality.middle_pixel = temp;
	}
	NormalValue(vfaces, 0);
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.lightQuality.middle_pixel;
		vfaces[i].quality.lightQuality.middle_pixel = temp;
	}

	//归一化symmetryhist
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.symmetryQuality.hist;
		vfaces[i].quality.symmetryQuality.hist = temp;
	}
	NormalValue(vfaces, 0);
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.symmetryQuality.hist;
		vfaces[i].quality.symmetryQuality.hist = temp;
	}

	//归一化symmetrylbph
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.symmetryQuality.lbph;
		vfaces[i].quality.symmetryQuality.lbph = temp;
	}
	NormalValue(vfaces, 0);
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.symmetryQuality.lbph;
		vfaces[i].quality.symmetryQuality.lbph = temp;
	}

	//归一化symmetrygabor
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.symmetryQuality.gabor;
		vfaces[i].quality.symmetryQuality.gabor = temp;
	}
	NormalValue(vfaces, 0);
	for (size_t i=0; i<vfaces.size(); i++)
	{
		double temp = vfaces[i].value;
		vfaces[i].value = vfaces[i].quality.symmetryQuality.gabor;
		vfaces[i].quality.symmetryQuality.gabor = temp;
	}
	return 0;

}
