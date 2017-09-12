#include<iostream>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include"HXLBMPFILE.h"
using namespace std;

template<typename TypeIterator>
TypeIterator Compress(string str, TypeIterator result)
{
	//建词典
	map<string,short>dictionary;
	short Dictsize = 256;
	FILE *f1;

	//存储词典的文件
	fopen_s(&f1, "dict", "wb");

	//初始化0-255
	for (short i = 0; i < Dictsize; i++) 
	{
		dictionary[string(1, i)] = i;
		printf("%s:%d\n", (string(1, i)).c_str(), dictionary[string(1, i)]);
		fprintf_s(f1, "%d\t%s\n", dictionary[string(1, i)], (string(1, i)).c_str());
	}

	char z;
	string S;

	for (string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		z = *it;

		if (dictionary.count(S + z))//S+z是否在词典中
		{
			S += z;
		}
		else//S is not in dictionary D  
		{
			*result++ = dictionary[S]; //输出S到result
			dictionary[S + z] = Dictsize++; //把S+z加到词典中
			printf("%d：%s\n", dictionary[S + z], (S + z).c_str());
			fprintf_s(f1, "%d\t%s\n", dictionary[S + z], (S + z).c_str());
			S =z;
		}
	}

	if (!S.empty())
		*result++ = dictionary[S];

	return result;
}

int main()
{
	typedef vector<short> TypeIterator;
	TypeIterator compress_res;
	string S;
	HXLBMPFILE bmpfile;
	int i, j;
	FILE *f;

	//存储编码结果的文件
	fopen_s(&f, "b", "wb+");

	if (!bmpfile.LoadBMPFILE("b.bmp")) return 1;

	//提取图像的灰度值，把灰度值变成字符
	for (i = 0; i <= bmpfile.imageh - 1; i++)
		for (j = 0; j <= bmpfile.imagew - 1; j++)
		{
			S += string(1,bmpfile.pDataAt(i)[j]);
		}

	//lzw编码
	Compress(S, std::back_inserter(compress_res));

	//输出编码结果
	for (TypeIterator::iterator it = compress_res.begin(); it != compress_res.end(); it++)
	{
		cout << (*it) << endl;
		fprintf(f, "%d ", short(*it));
	}

	return 0;
}