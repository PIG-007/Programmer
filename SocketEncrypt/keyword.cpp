#include "keyword.h"
#include<stdlib.h>
#include<memory.h>
using namespace std;

string KeywordEncrypt(string plain,string key)
{
	string result;
	char list[26];//对照表
	genWordList(key, list);
	int i = 0;
	while (i < plain.length())
	{
		result += list[ (int)plain[i] - 97 ];
		i++;
	}
	return result;
}
string KeywordDecrypt(string cipher, string key)
{
	string result;
	char list[26];
	genWordList(key, list);
	int i = 0;
	while (i < cipher.length())
	{
		for (int j = 0; j < 26; j++)
		{
			if (cipher[i] == list[j])
			{
				result += (char)(j + 97);
				break;
			}
		}
		i++;
	}
	return result;
}

void genWordList(string key, char* list)//生成对应表 list默认26位
{
	int i=0,j = 0;//i遍历key j记录位置
	bool ori[26];//查表
	memset(ori,false,sizeof(ori));

	while (i < key.length())
	{
		if (!ori[(int)key[i] - 97])
		{
			list[j++] = key[i];
			ori[(int)key[i] - 97] = true;
		}
		i++;
	}
	i = 0;
	while (j < 26)
	{
		if (!ori[i])
		{
			list[j++] = (char)(i+97);
			ori[i] = true;
		}
		i++;
	}
	

}
