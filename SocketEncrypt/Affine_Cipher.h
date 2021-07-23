#pragma once
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string>

using namespace std;
string encode(string plaintext, string key);
int getGcd(int value1, int value2);
void setCoprime(int coprime[], int n);
int get_a(int coprime[], int a, int n);
string decode(string ciphertext, string key);

//int main()
//{
//
//	//str存储明文
//	string str;
//	string key;
//	cout << "输入key的值\n";
//	cin >> key;
//
//	cout << "输入str的内容\n";
//	cin >> str;
//
//	//输出明文
//	cout << "明文:" + str + "\n";
//	//加密
//	string s1 = encode(str, key);
//
//	//检验是否加密成功
//	cout << s1 + "\n";
//
//	//解密
//	string s2 = decode(s1, key);
//
//	//检验是否解密成功
//	cout << s2 + "\n";
//
//	return 0;
//}