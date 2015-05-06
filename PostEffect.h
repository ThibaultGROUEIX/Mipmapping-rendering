#ifndef POSTEFFECT_H
#define POSTEFFECT_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>

class PostEffect
{
public:
	static float MeanSquareDistance(const std::vector<unsigned char> & firstData, const std::vector<unsigned char> & secondData);
	static void GetTotalMSE();
	static void add(const std::vector<unsigned char> & firstData);
	static void remove(int i);

public:
	static std::vector<std::vector<unsigned char> > datas;
};


#endif