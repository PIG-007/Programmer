#include <iostream>
#include<string>
#include "caesar.h"
using namespace std;

string Caesar_Encryption(string plaintext, string ciphertext, string key)
{
    if (key.length() != 1)
        return NULL;

    int k = stoi(key);
    int len = plaintext.length();

    for (int i = 0; i < len; i++)
    {
        ciphertext += ((plaintext[i] - 'a') + k) % 26 + 'a';

    }

    return ciphertext;
}

string Caesar_Decryption(string plaintext, string ciphertext, string key)
{
    if (key.length() != 1)
        return NULL;

    int k = stoi(key);
    int len = ciphertext.length();

    for (int i = 0; i < len; i++)
    {
        plaintext += ((ciphertext[i] - 'a') + 26 - k) % 26 + 'a';

    }

    return plaintext;
}


