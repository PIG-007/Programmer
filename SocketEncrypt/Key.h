#pragma once
#include "StdAfx.h"

class Key
{
public:
	Key(void);
	Key(const Generator& g,int x,const Point& Q);//生成元g,私钥x，公钥Q
	virtual ~Key(void);
	const Point& getPublicKey()const;
	const int getPrivateKey()const;
	const Generator& getGenerator()const;
protected:
	Generator m_g;//生成元
	Point m_Q;//公钥
	int m_x;//私钥
};

