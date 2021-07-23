//
// Created by 007 on 2021/7/2.
//

#include<string>
using namespace std;

#ifndef AES_LZX_H
#define AES_LZX_H


//AES Module
//init aes
char** aesInitMatrix(string block);

//伽罗华域求解
int GFMul2(int s);
int GFMul3(int s);
int GFMul4(int s);
int GFMul8(int s);
int GFMul9(int s);
int GFMul11(int s);
int GFMul12(int s);
int GFMul13(int s);
int GFMul14(int s);
int GFMul(int n, int s);
void mixColumns(char** textBlock,int flag);

//circul module
void keyAdd(char** textBlock, char** subKeyBlock);
void bySBOXText(char** textBlock, int flag);
void byShiftRowsText(char** textBlock, int* shiftRule);

//key function
void byLeftShifRowsKey(char* tempKey);
void bySBOXKey(char* tempKey);
char** genKey(char** originalKey);
char** getSubKeyBlock(char** key, int roundAmount);

//circulation function
void enCryptCircuStruct(char** plaintextBlock, char** key);
void deCryptCircuStruct(char** cipherTextBlock, char** key);

//hex function
string aesHexToStrChar(string strCiphertext);

//dataBase=16(hex)  dataBase=else(string) aes-128
string aesEncrypt(string strPlaintextBlock, int dataBase, string strOriginalKey = "PIGOOTNB");
string aesDecrypt(string strCiphertextBlock, int dataBase, string strOriginalKey = "PIGOOTNB");


//multi
string multEncrypt(string plaintext,string strOriginalKey = "PIGOOT");
string multDecrypt(string ciphertext,string strOriginalKey = "PIGOOT");

//permu
string permuEncrypt(string plaintext,string strOriginalKey = "PIGOOT");
string permuDecrypt(string ciphertext,string strOriginalKey = "PIGOOT");


#endif //AES_LZX_H
