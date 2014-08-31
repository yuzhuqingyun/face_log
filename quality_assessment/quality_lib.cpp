#include "stdafx.h"
#include "quality_lib.h"



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
int Normal(vector<Face>& vfaces, int flag)
{
	if (vfaces.empty())
	{
		return 0;
	}
	Face maxFace = *max_element(vfaces.begin(), vfaces.end(), MaxFaceValue);
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
			break;
		}
	}
}