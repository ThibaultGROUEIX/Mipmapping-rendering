#include "PostEffect.h"

using namespace std;

std::vector<std::vector<unsigned char> > PostEffect::datas = std::vector<std::vector<unsigned char> >();

float PostEffect::MeanSquareDistance(const vector<unsigned char> & firstData, const vector<unsigned char> & secondData)
{
	float result = 0.;
	if(firstData.size()!=secondData.size())
	{
		std::cout << "data can't be compared " << std::endl;
		return -1.;
	}

	for(int unsigned i=0;i<firstData.size();i++)
	{
		result += pow(firstData[i]-secondData[i], 2);
	}
	result = sqrt(result);
	return result;
}


void PostEffect::GetTotalMSE()
{	
	cout << " size : " << datas.size() << endl;
	for(int unsigned i=0;i<datas.size();i++)
	{
		for(int unsigned j=i+1;j<datas.size();j++)
		{
			std::cout << "data " << i << " & data " << j << " : " << MeanSquareDistance(datas[i], datas[j]) << std::endl;
		}
	}
}

void PostEffect::add(const vector<unsigned char> & firstData)
{
	datas.push_back(firstData);
	cout << "added" << endl;
}

void PostEffect::remove(int i)
{	if (i > datas.size())
	{
		std::cout << "there is no such element in vector datas" << std::endl;
		return;
	}
	datas.erase(datas.begin()+i-1);
}