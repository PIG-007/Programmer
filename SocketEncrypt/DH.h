#pragma once
#include<iostream>
#include<string>
#include"RSA.h"//使用RSA生成素数算法
using namespace std;

void genertateDHKey(long& P, long& G, int& A, int& B);
long generator(long p);//生成元
void switchKey(long& P, long& G, int& A,
	int& B, long& g_a, long& g_b, long& key_a, long& key_b);//交换
	
