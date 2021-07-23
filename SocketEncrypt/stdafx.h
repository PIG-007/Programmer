// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include <memory.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
struct Point{//��ʾƽ���ϵ�һ����
	int X;
	int Y;
	Point(int x=0,int y=0)//���캯��
	{
		this->X=x;
		this->Y=y;
	}
	Point(const Point& p)//�������캯��
	{
		this->X=p.X;
		this->Y=p.Y;
	}
	bool operator==(const Point& p)const//�ж������Ƿ����
	{
		return this->X==p.X&&this->Y==p.Y;
	}
	bool operator!=(const Point& p)const//�ж������Ƿ񲻵�
	{
		return this->X!=p.X||this->Y!=p.Y;
	}
	friend std::ostream& operator<<(std::ostream &out,const Point& p)//��������������
	{
		if (p.isInfinity())
		{
			out<<"(��,��)";
		}
		else
		{
			out<<'('<<p.X<<','<<p.Y<<')';
		}
		return out;
	}
	bool isZero()const//�ж��Ƿ���ԭ��
	{
		return this->X==0&&this->Y==0;
	}
	bool isInfinity()const//�ж�������Զ��
	{
		return this->X==INT_MAX||this->Y==INT_MAX;
	}
	static const Point Infinity()//����Զ��
	{
		static const Point p(INT_MAX,INT_MAX);
		return p;
	}
};
struct Generator{//��ʾһ������Ԫ = value��order
	Point Value;//��P
	int Order;//��P�Ľף�������nP=O����С������n
	Generator(){}
	Generator(const Point& p,int o)
	{
		this->Value.X=p.X;
		this->Value.Y=p.Y;
		this->Order=o;
	}
	bool operator==(const Generator& g)const//�ж���������Ԫ�Ƿ����
	{
		return this->Value==g.Value&&this->Order==g.Order;
	}
	bool operator!=(const Generator& g)const//�ж���������Ԫ�Ƿ����
	{
		return !(*this==g);
	}
	friend std::ostream& operator<<(std::ostream &out,const Generator& g)
	{
		out<<g.Value<<"=>"<<g.Order;
		return out;
	}
};
struct Cipher{//��ʾһ���ַ�����ECC���ܺ�����Ӧ������
	Point C1;//C1=kP
	Point C2;//C2=Pm+kQ
	Cipher(const Point& c1,const Point& c2)
	{
		this->C1.X=c1.X;
		this->C1.Y=c1.Y;
		this->C2.X=c2.X;
		this->C2.Y=c2.Y;
	}
	friend std::ostream& operator<<(std::ostream &out,const Cipher& c)
	{
		out<<'['<<c.C1<<' '<<c.C2<<']';
		return out;
	}
};
struct Dictionary{//��ʾ���ַ������ɵ��ַ���
private:
	unsigned char mCharValueSet[256];
	int mHash[256];
	int Count;
public:
	Dictionary(void)//��ʼ��һ�����ֵ�
	{
		memset(mCharValueSet,0,sizeof(mCharValueSet));
		memset(mHash,0,sizeof(mHash));
		this->Count=0;
	}
	Dictionary(const char *str)//�ɸ������ַ�����ʼ��һ���ֵ�
	{
		memset(mCharValueSet,0,sizeof(mCharValueSet));
		memset(mHash,0,sizeof(mHash));
		this->Count=0;

		unsigned char i=0;
		while(*str!='\0')
		{
			i=*str;
			mHash[i]++;
			str++;
		}
		for (int i=0;i<256;i++)
		{
			if (mHash[i]!=0)
			{
				mCharValueSet[this->Count]=i;
				mHash[i]=this->Count;
				this->Count++;
			}
		}
	}
	int getIndex(unsigned char ch)const//��ȡĳ���ַ����ֵ��е��±�
	{
		//return std::lower_bound(mCharValueSet,mCharValueSet+Count,ch)-mCharValueSet;//���ֲ���
		return mHash[ch];//��ϣ����
	}
	unsigned char getChar(int i)const//��ȡָ�����������ַ�
	{
		return mCharValueSet[i];
	}
};
