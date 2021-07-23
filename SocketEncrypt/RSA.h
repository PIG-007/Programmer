#pragma once
#include<iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include"sstream"


using namespace std;
unsigned long generatePrime(unsigned long max, unsigned  min);
bool M_R(long long base, long num);
bool ifPrime(unsigned long p);
bool ifPrime2each(long  num1, long  num2);
int getOpposite(long int num, long int mode);
long quickExp(long a, long k, long m);

string RSAEncrypt(string plain, long publickey, long mod);
string RSADecrypt(string cipher, long privatekey, long mod);
void generateKey(long& publickey, long& privatekey,long& mod,long& pp, long& qq);

string dec2hex(long i,int width);
long hex2dec(string s, int position);
