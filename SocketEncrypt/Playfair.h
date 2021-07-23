#pragma once
#include <iostream>
#include<string>
using namespace std;

void CreateTable(string in_key);
char GetKey(int x, int y, char type);
void Getposition(char c, int* x, int* y);
string Playfair_Encryption(string plaintext, string ciphertext);
string Playfair_Decryption(string plaintext, string ciphertext);
