#pragma once
#include <iostream>
#include <string>
using namespace std;

char(*vigenereTable(char(&list)[26][26]))[26];
string AtPlain_encode(string plaintext, string key);
string AtPlain_decode(string ciphertext, string key);

//int main()
//{
//    string ciphertext = encode("autokeyplaintext", "flag");
//    cout << ciphertext + "\n";
//    string plaintext = decode("fftukyrdvegceefg", "flag");
//    cout << plaintext;
//}
