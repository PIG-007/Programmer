#pragma once
#include <iostream>
#include <bitset>
#include<fstream>
using namespace std;
string RC4_encrypt(char data[], char key[]);
string RC4_decrypt(char data[], char key[]);

//int main() {
//	char key[] = { "hello world" };
//	char data[] = { "����ʱ��ʮ�˵�ʵʩ��ը!����ʱ��ʮ�˵�ʵʩ��ը!����ʱ��ʮ�˵�ʵʩ��ը!" };
//	encrypt(data, key);
//	decrypt(data, key);
//}
