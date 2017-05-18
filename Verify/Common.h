#pragma once
#include "stdafx.h"
#include "Common.h"
int GetRandomNum(int iMin, int iMax)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_int_distribution<>dis(iMin, iMax);
	return dis(mt);

}
std::string GetRandomString(int num)
{
	if (num > 4095) num = 4095;
	char buff[4096];
	for (int i = 0; i < num; ++i)
	{
		buff[i] = GetRandomNum('A', 'z');
		if(buff[i]>90 && buff[i] < 97){
			buff[i] = '_';
		}
	}
	buff[num] = 0;
	return buff;
}