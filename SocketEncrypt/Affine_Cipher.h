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
//	//str�洢����
//	string str;
//	string key;
//	cout << "����key��ֵ\n";
//	cin >> key;
//
//	cout << "����str������\n";
//	cin >> str;
//
//	//�������
//	cout << "����:" + str + "\n";
//	//����
//	string s1 = encode(str, key);
//
//	//�����Ƿ���ܳɹ�
//	cout << s1 + "\n";
//
//	//����
//	string s2 = decode(s1, key);
//
//	//�����Ƿ���ܳɹ�
//	cout << s2 + "\n";
//
//	return 0;
//}