#pragma once
#include "StdAfx.h"

class Key
{
public:
	Key(void);
	Key(const Generator& g,int x,const Point& Q);//����Ԫg,˽Կx����ԿQ
	virtual ~Key(void);
	const Point& getPublicKey()const;
	const int getPrivateKey()const;
	const Generator& getGenerator()const;
protected:
	Generator m_g;//����Ԫ
	Point m_Q;//��Կ
	int m_x;//˽Կ
};

