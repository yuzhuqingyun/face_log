#include "stdafx.h"
#include "quality_lib.h"

int NormalValue(vector<Face>& vfaces, int flag=0);	//��Face��Valueֵ��һ��



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
// ��������: vector
// ����˵��: //��Face��Valueֵ��һ��
//				0:sample/Max; 1:1- samply/Max; 
// ��    ��: Juliwei
// �������: 2014/08/31
// �� �� ֵ: int
// ��    ��:vector<Face> & vfaces
// ��    ��:int flag
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
	//��һ��dark_spot
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

	//��һ��equalization
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

	//��һ��dark_spot
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

	//��һ��symmetryhist
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

	//��һ��symmetrylbph
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

	//��һ��symmetrygabor
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
