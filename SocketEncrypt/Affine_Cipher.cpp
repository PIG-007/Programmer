#include "Affine_Cipher.h"


//加密算法
string encode(string plaintext, string key)
{
	int i = 0;
	int n = key.length();
	string key1, key2;
	while ('#'!=key[i]) {
		key1 += key[i];
		i++;
	}
	i++;
	while (i<n) {
		key2 += key[i];
		i++;
	}
	int a, b;
	a = stoi(key1);
	b = stoi(key2);
	int m = plaintext.length();
	//核心算法
	for (i = 0;i < m;i++) {
		plaintext[i] -= 'a';
		plaintext[i] = (a * plaintext[i] + b) % 26;
		plaintext[i] += 'a';
	}
	return plaintext;
}
//获取value1和value2的最大公约数
int getGcd(int value1, int value2)
{
	int gcd = 0; //最大公约数
	int divisor = 0; //余数

	do	//辗转相除法
	{
		divisor = value1 % value2;

		gcd = value2;

		value1 = value2;
		value2 = divisor;

	} while (divisor);

	return gcd;
}
//设置数组coprime存放与n互素的元素
void setCoprime(int coprime[], int n)
{
	int i = 1;

	for (; i < n; i++)
		if (1 == getGcd(n, i))//判断是否n,i是否互素
			*(coprime++) = i; //将i存入coprime中
}

//在数组coprime中寻找a的模n可逆元
int get_a(int coprime[], int a, int n)
{
	int i = 0;

	for (; coprime[i] != 0; i++)
		if (1 == (a * coprime[i]) % n)
			return coprime[i];

	return 0;
}
//解密算法
string decode(string ciphertext,string key)
{
	int i = 0;
	int n = ciphertext.length();
	string key1, key2;
	while (key[i] != '#') {
		key1 += key[i];
		i++;
	}
	i++;
	for (i;i < n;i++) {
		key2 += key[i];
	}
	int a, b;
	a = stoi(key1);
	b = stoi(key2);


	int coprime[32] = { 0 }; //存放小于n并且与n互素的元素
	int _a = 0; //存放a的模n可逆元


	for (i=0; i < 32; i++)  //将数组元素赋为0
		coprime[i] = 0;

	setCoprime(coprime, 26);//设置数组coprime存放与n互素的元素

	_a = get_a(coprime, a, 26);//在coprime中寻找a的逆元_a
	int m = ciphertext.length();
	for (i = 0;i < m;i++) {
		ciphertext[i] -= 'a';
		ciphertext[i] = (_a * (ciphertext[i] - b + 26)) % 26;
		ciphertext[i] += 'a';
	}
	return ciphertext;
}

