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
	int res[65535];//����ĻҶ�ֵ
	short r[65535];//�ݴ��ļ�����
	int i, j;
	FILE *f;
	
	//��ʼ���ʵ�
	for (i = 0; i<Dictsize; i++)
		inv_dictionary[i] = string(1, i);
	Dictsize--;

	//���ļ�����result
	fopen_s(&f, firename, "rb+");
	i = 0;
	while (fscanf_s(f, "%d", &r[i]) != EOF)
	{
		result.push_back(r[i]);
		printf("%d\n", r[i]);
		i++;
	}

	//����
	i = 0;
	for (TypeIterator::iterator it = result.begin(); it != result.end(); it++)
	{
		int k = *it;
		if (inv_dictionary.count(k))//�����Ƿ����ֵ���
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

		inv_dictionary[Dictsize++] = S + entry[0];//���µ��ַ������뵽�ֵ���
		S = entry;

	}
	return res;
}


int main()
{
	typedef vector<short> TypeIterator;
	TypeIterator compress_res;
	HXLBMPFILE bmpfile, bf;//bf������ͼ��
	int i, j, k = 0;

	if (!bmpfile.LoadBMPFILE("b.bmp")) return 1;

	//��ʼ��������ͼ��
	bf.imagew = bmpfile.imagew;
	bf.imageh = bmpfile.imageh;
	bf.iYRGBnum = bmpfile.iYRGBnum;
	if (!bf.AllocateMem()) return 1;
	
	//����
	int *decompress = Decompress(compress_res,"b");//�ڶ�������Ϊ�ļ���

	for (i = 0; i <= bmpfile.imageh - 1; i++)
		for (j = 0; j <= bmpfile.imagew - 1; j++)
		{
			bf.pDataAt(i)[j] = decompress[k++];
		}
	bf.SaveBMPFILE("������ͼ��.bmp");

	return 0;
}