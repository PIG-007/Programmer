#pragma once
#include<iostream>
#include<string>
#include"RSA.h"//ʹ��RSA���������㷨
using namespace std;

void genertateDHKey(long& P, long& G, int& A, int& B);
long generator(long p);//����Ԫ
void switchKey(long& P, long& G, int& A,
	int& B, long& g_a, long& g_b, long& key_a, long& key_b);//����
	
