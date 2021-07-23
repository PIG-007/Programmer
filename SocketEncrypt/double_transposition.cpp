#include"double_transprotion.h"

string double_transEncrypt(string plain, string key1, string key2)//key1��һ�μ��� key2�ڶ��μ���
{
    string temp;
    temp = encrypt_once(plain, key1);
    temp = encrypt_once(temp, key2);
    return temp;
}

string double_transDecrypt(string cipher, string key1, string key2)
{
    string temp;
    temp = decrypt_once(cipher, key2);
    temp = decrypt_once(temp, key1);
    return temp;
}

string encrypt_once(string plain, string key)
{
    string cipher;
    int i, j = 0;
    int m;
    int l = key.length();
    int* num = (int*)malloc(l * sizeof(int));//����
    int* order = (int*)malloc(l * sizeof(int));//��˳��ȡ
    for (i = 0; i < l; i++)
    {
        num[i] = 0;
        for (j = 0; j < l; j++)
        {
            if (key[j] <= key[i])
                num[i] = num[i] + 1;
            if (key[j] == key[i] && j > i)
                num[i] = num[i] - 1;
        }
    }//���� 
    for (i = 0; i < l; i++)
    {
        order[num[i] - 1] = i;
    }//˳��
    int k = plain.length() % l;
    if (k == 0)
        m = plain.length() / l;
    else m = plain.length() / l + 1;
    if (k != 0)
    {
        bool f = false;
        for (int p = 0; p < k; p++)
        {
            if (plain[(m - 1) * l + p] != 'x')
                f = true;
        }
        if (f)//���һ�в�ȫΪx ���� ȫΪx��ɾ�����һ��
        {
            for (int k = plain.length() % l; k < l; k++)
            {

                plain += "x";
            }//ȱλ��x
        }
        else {
            m--;
            plain.erase(m * l, k);
        }

    }
    i, j = 0;
    for (j = 0; j < l; j++)
    {
        for (i = 0; i < m; i++)

        {
            cipher += plain[i * l + order[j]];
        }
    }
    return cipher;
}

string decrypt_once(string cipher, string key )
{
    char* plain = (char*)malloc(cipher.length() * sizeof(char));
    int i, j = 0;
    int m;
    int l = key.length();
    int* num = (int*)malloc(l * sizeof(int));//����
    int* order = (int*)malloc(l * sizeof(int));//��˳��ȡ
    for (i = 0; i < l; i++)
    {
        num[i] = 0;
        for (j = 0; j < l; j++)
        {
            if (key[j] <= key[i])
                num[i] = num[i] + 1;
            if (key[j] == key[i] && j > i)
                num[i] = num[i] - 1;
        }
    }//����
    for (i = 0; i < l; i++)
    {
        order[num[i] - 1] = i;
    }//˳��
    int k = cipher.length() % l;
    if (k == 0)
        m = cipher.length() / l;
    else m = cipher.length() / l + 1;
    if (k != 0)
    {
        bool f = false;
        for (int p = 0; p < k; p++)
        {
            if (cipher[(m - 1) * l + p] != 'x')
                f = true;
        }
        if (f)//���һ�в�ȫΪx ���� ȫΪx��ɾ�����һ��
        {
            for (int k = cipher.length() % l; k < l; k++)
            {

                cipher += "x";
            }//ȱλ��x
        }
        else {
            m--;
            cipher.erase(m * l, k);
        }
        
    }
    //��������
    int count = 0;
    for (j = 0; j < l; j++)
    {
        for (i = 0; i < m; i++)
        {
            plain[i * l + order[j]] = cipher[count++];
        }
    }
    string result;
    count = 0;
    while (count < cipher.length())
    {
        result += plain[count++];
    }
    return result;
}