#pragma once
#include <iostream>
#include <string>
using namespace std;

char(*vigenereTable(char(&list)[26][26]))[26];
string AtCipher_encode(string plaintext, string key);
string AtCipher_decode(string ciphertext, string key);

//int main()
//{
//    string ciphertext = encode("autokeycipher", "flag");
//    cout << ciphertext + "\n";
//    string plaintext = decode("fftupjrwxyyao", "flag");
//    cout << plaintext;
//}
