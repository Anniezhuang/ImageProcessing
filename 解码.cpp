#include<iostream>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include"HXLBMPFILE.h"
using namespace std;

template<typename TypeIterator>
int* Decompress(TypeIterator result,const char firename[])
{
	map<int, string>inv_dictionary;
	int Dictsize = 256;
	string S,entry;
	int res[65535];//输出的灰度值
	short r[65535];//暂存文件参数
	int i, j;
	FILE *f;
	
	//初始化词典
	for (i = 0; i<Dictsize; i++)
		inv_dictionary[i] = string(1, i);
	Dictsize--;

	//将文件读到result
	fopen_s(&f, firename, "rb+");
	i = 0;
	while (fscanf_s(f, "%d", &r[i]) != EOF)
	{
		result.push_back(r[i]);
		printf("%d\n", r[i]);
		i++;
	}

	//解码
	i = 0;
	for (TypeIterator::iterator it = result.begin(); it != result.end(); it++)
	{
		int k = *it;
		if (inv_dictionary.count(k))//码字是否在字典中
			entry = inv_dictionary[k];
		else if (k == Dictsize)
			entry = S + S[0];
		else
			throw "Bad compression code";

		for (int j = 0; j<entry.length(); j++)
		{
			res[i] = int(entry.at(j));
			i++;
		}

		inv_dictionary[Dictsize++] = S + entry[0];//把新的字符串加入到字典中
		S = entry;

	}
	return res;
}


int main()
{
	typedef vector<short> TypeIterator;
	TypeIterator compress_res;
	HXLBMPFILE bmpfile, bf;//bf解码后的图像
	int i, j, k = 0;

	if (!bmpfile.LoadBMPFILE("b.bmp")) return 1;

	//初始化解码后的图像
	bf.imagew = bmpfile.imagew;
	bf.imageh = bmpfile.imageh;
	bf.iYRGBnum = bmpfile.iYRGBnum;
	if (!bf.AllocateMem()) return 1;
	
	//解码
	int *decompress = Decompress(compress_res,"b");//第二个参数为文件名

	for (i = 0; i <= bmpfile.imageh - 1; i++)
		for (j = 0; j <= bmpfile.imagew - 1; j++)
		{
			bf.pDataAt(i)[j] = decompress[k++];
		}
	bf.SaveBMPFILE("解码后的图象.bmp");

	return 0;
}