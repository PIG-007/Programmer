// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include <memory.h>

// TODO: 在此处引用程序需要的其他头文件
struct Point{//表示平面上的一个点
	int X;
	int Y;
	Point(int x=0,int y=0)//构造函数
	{
		this->X=x;
		this->Y=y;
	}
	Point(const Point& p)//拷贝构造函数
	{
		this->X=p.X;
		this->Y=p.Y;
	}
	bool operator==(const Point& p)const//判断两点是否相等
	{
		return this->X==p.X&&this->Y==p.Y;
	}
	bool operator!=(const Point& p)const//判断两点是否不等
	{
		return this->X!=p.X||this->Y!=p.Y;
	}
	friend std::ostream& operator<<(std::ostream &out,const Point& p)//重载流输出运算符
	{
		if (p.isInfinity())
		{
			out<<"(∞,∞)";
		}
		else
		{
			out<<'('<<p.X<<','<<p.Y<<')';
		}
		return out;
	}
	bool isZero()const//判断是否是原点
	{
		return this->X==0&&this->Y==0;
	}
	bool isInfinity()const//判断是无穷远点
	{
		return this->X==INT_MAX||this->Y==INT_MAX;
	}
	static const Point Infinity()//无穷远点
	{
		static const Point p(INT_MAX,INT_MAX);
		return p;
	}
};
struct Generator{//表示一个生成元 = value和order
	Point Value;//点P
	int Order;//点P的阶，即满足nP=O的最小正整数n
	Generator(){}
	Generator(const Point& p,int o)
	{
		this->Value.X=p.X;
		this->Value.Y=p.Y;
		this->Order=o;
	}
	bool operator==(const Generator& g)const//判断两个生成元是否相等
	{
		return this->Value==g.Value&&this->Order==g.Order;
	}
	bool operator!=(const Generator& g)const//判断两个生成元是否不相等
	{
		return !(*this==g);
	}
	friend std::ostream& operator<<(std::ostream &out,const Generator& g)
	{
		out<<g.Value<<"=>"<<g.Order;
		return out;
	}
};
struct Cipher{//表示一个字符经过ECC加密后所对应的密文
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
struct Dictionary{//表示由字符串构成的字符集
private:
	unsigned char mCharValueSet[256];
	int mHash[256];
	int Count;
public:
	Dictionary(void)//初始化一个空字典
	{
		memset(mCharValueSet,0,sizeof(mCharValueSet));
		memset(mHash,0,sizeof(mHash));
		this->Count=0;
	}
	Dictionary(const char *str)//由给定的字符串初始化一个字典
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
	int getIndex(unsigned char ch)const//获取某个字符在字典中的下标
	{
		//return std::lower_bound(mCharValueSet,mCharValueSet+Count,ch)-mCharValueSet;//二分查找
		return mHash[ch];//哈希查找
	}
	unsigned char getChar(int i)const//获取指定索引处的字符
	{
		return mCharValueSet[i];
	}
};
