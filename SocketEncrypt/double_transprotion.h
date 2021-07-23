#pragma once
#include<iostream>
#include<string>
using namespace std;

string double_transEncrypt(string plain, string key1, string key2);
string double_transDecrypt(string cipher, string key1, string key2);
string encrypt_once(string plain, string key);
string decrypt_once(string cipher, string key);