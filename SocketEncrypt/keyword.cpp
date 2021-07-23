#include "keyword.h"
#include<stdlib.h>
#include<memory.h>
using namespace std;

string KeywordEncrypt(string plain,string key)
{
	string result;
	char list[26];//���ձ�
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

void genWordList(string key, char* list)//���ɶ�Ӧ�� listĬ��26λ
{
	int i=0,j = 0;//i����key j��¼λ��
	bool ori[26];//���
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
