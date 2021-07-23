#pragma once
#include<iostream>
#include <string>
#include<stdlib.h>
#include<memory.h>
using namespace std;
void genWordList(string key, char* list);
string KeywordEncrypt(string plain, string key);
string KeywordDecrypt(string cipher, string key);
