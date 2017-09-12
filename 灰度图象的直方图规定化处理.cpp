#include "HXLBMPFILE.h"

int main()
{
	HXLBMPFILE bmpfile; 
	int i, j;
	double x = 0,  tem2 = 0, sum = 0;
	int T[256] = { 0 }, T3[256] = {0}, T4[256] = { 0 };
	double t0[256] = { 0 }, T1[256] = { 0 }, T2[256] = { 0 }, t2[256] = { 0 }, t1[256] = { 0 }, y[256] = { 0 }, count = 0, t3[256] = { 0 };
	FILE *f1, *f2,*f3,*f4,*f5,*f6,*f7;
	HXLBMPFILE bf,bf1;//bf1���⻯���ͼ��bf�涨�����ͼ��
	
	if (!bmpfile.LoadBMPFILE("c.bmp")) return 1;

	for (i = 0; i <= bmpfile.imageh - 1; i++)
		for (j = 0; j <=bmpfile.imagew - 1; j++)
		{
			T[bmpfile.pDataAt(i)[j]]++;
			count++;
		}

	bf.imagew = bmpfile.imagew;
	bf.imageh = bmpfile.imageh;
	bf.iYRGBnum = bmpfile.iYRGBnum;
	if (!bf.AllocateMem()) return 1;

	bf1.imagew = bmpfile.imagew;
	bf1.imageh = bmpfile.imageh;
	bf1.iYRGBnum = bmpfile.iYRGBnum;
	if (!bf1.AllocateMem()) return 1;

	fopen_s(&f1, "ԭͼ��ֱ��ͼH.txt", "w");
	fopen_s(&f2, "���⻯��ת������T1.txt", "w");
	fopen_s(&f3, "ԭͼ����⻯��ֱ��ͼH1.txt", "w");
	fopen_s(&f4, "Ŀ��ֱ��ͼ.txt", "w");
	fopen_s(&f5, "Ŀ��ֱ��ͼ���⻯��ת������T2.txt", "w");
	fopen_s(&f6, "����Ŀ��ֱ��ͼ.txt", "w");
	fopen_s(&f7, "�涨����ת������T.txt", "w");

	for (i = 0; i < 256; i++)
	{
		x = double(T[i]) / count;
		fprintf_s(f1, "%.3f\n", x);//ԭͼ��ֱ��ͼH
		t1[i] = x;//ԭͼ��ֱ��ͼ����

	}

	count = 0;
	for (i = 0; i < 256; i++)
	{
		y[i] = -(i *2/ 255.0 - 1) * (i*2 / 255.0 - 1) + 1;
		count += y[i];
	}

	for (i = 0; i < 256; i++)
	{
		sum += y[i] / count;
		fprintf_s(f4, "%d\t%.6f\n", i, y[i] / count);//Ŀ��ֱ��ͼ
		fprintf_s(f5, "%d\t%.3f\n", i, sum);//Ŀ��ֱ��ͼ���⻯��ת������T2.txt
		T2[i] = sum;
	}

	//�ۻ�
	for (i = 0; i < 256; i++)
	{
		if(i==0)
			T1[0] = t1[0];
		else
			T1[i] = T1[i - 1] + t1[i];
		fprintf_s(f2, "%.3f\n", T1[i]);//���⻯��ת������T1
	}

	//��tk=int[��L-1��tk+0.5]��tkӳ�䵽[0��L-1]
	for (i = 0; i < 256; i++)
	{
		j = int(255.0 * T1[i] + 0.5);
		t0[j] += t1[i];
		T4[i] = j;//�»Ҷȼ�
	}

	//ԭͼ����⻯��ֱ��ͼH1
	for (i = 0; i < 256; i++)
	{
		fprintf_s(f3, "%d\t%.3f\n", i, t0[i]);
	}

	//���ԭͼ����⻯��ͼ��
	for (i = 0; i <= bf1.imageh - 1; i++)
	{
		for (j = 0; j <= bf1.imagew - 1; j++)
		{
			bf1.pDataAt(i)[j] = T4[bmpfile.pDataAt(i)[j]];
		}
	}
	bf1.SaveBMPFILE("ԭͼ����⻯��ͼ��.bmp");

	//����Ŀ��ֱ��ͼ
	j = 0;
	i = 0;
	while(i<256&&j<255)
	{
		if (T1[i] >= T2[j] && T1[i]<=T2[j+1])
		{
			if ((T1[i] - T2[j]) <= T2[j+1]- T1[i]) {
				T3[i] = j;
				t2[j] += t1[i];
				t3[i] = T2[j];
			}
			else
			{
				T3[i] = j+1;
				t2[j+1] += t1[i];
				t3[i] = T2[j+1];
			}
			i++;
		}
		else j++;
	}

	//�涨����ת������T
	for (i = 0; i < 256; i++)
		fprintf_s(f7, "%d\t%.3f\n", i, t3[i]);

	//����Ŀ��ֱ��ͼ
	for (i = 0; i < 256; i++)
		fprintf_s(f6, "%d\t%.3f\n", i, t2[i]);

	//����涨�����ͼ��
	for (i = 0; i <= bf.imageh - 1; i++) 
	{
		for (j = 0; j <= bf.imagew - 1; j++)
		{
			bf.pDataAt(i)[j] = T3[bmpfile.pDataAt(i)[j]];
		}
	}
	bf.SaveBMPFILE("�涨�����ͼ��.bmp");
	printf("program ends!\n");
	return 0;
}