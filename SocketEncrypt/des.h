#pragma once
#include <iostream>
#include<fstream>
#include<string>
#include <sstream>
#include <stdio.h>

using namespace std;

string hexToStrChar(string strCiphertext);
string strBinToHex(string ciphertext);
string strToBinStr(string key);
void loop_move_left(char* p, int i);
void generate_Key(string key, char** ptr_key);
string block_code(char** key_16, string block_origintext, int flag_code);
string DES_Encode(string key, string plaintext);
string DES_Decode(string key, string ciphertext);
