#include "VigenereCipher.h"

string Vencrypt(string plaintext, string key)
{
    string ciphertext;
    int i, j = 0;
    int m = key.length();
    int l = plaintext.length();
    for (i = 0; i < l; i++)
    {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            if (key[j] >= 'a' && key[j] <= 'z')
                key[j] += 'A' - 'a';
            if (j == m)
            {
                j = 0;
                ciphertext += (plaintext[i] - 'A' + key[j] - 'A') % 26 + 'A';
            }
            else {
                ciphertext += (plaintext[i] - 'A' + key[j] - 'A') % 26 + 'A';
            }
            j++;
        }
        else if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            if (key[j] >= 'A' && key[j] <= 'Z')
                key[j] += 'a' - 'A';
            if (j == m) {
                j = 0;
                ciphertext += (plaintext[i] - 'a' + key[j] - 'a') % 26 + 'a';
            }
            else {
                ciphertext += (plaintext[i] - 'a' + key[j] - 'a') % 26 + 'a';
            }
            j++;
        }
        else {
            ciphertext += plaintext[i];
        }
    }
    return ciphertext;
}

string Vdecrypt(string ciphertext, string key)
{
    string plaintext;
    int i, j = 0;
    int m = key.length();
    int l = ciphertext.length();
    for (i = 0; i < l; i++)
    {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            if (key[j] >= 'a' && key[j] <= 'z')
                key[j] += 'A' - 'a';
            if (j == m) {
                j = 0;
                plaintext += (ciphertext[i] - key[j] + 26) % 26 + 'A';
            }
            else {
                plaintext += (ciphertext[i] - key[j] + 26) % 26 + 'A';
            }
            j++;
        }
        else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            if (key[j] >= 'A' && key[j] <= 'Z')
                key[j] += 'a' - 'A';
            if (j == m) {
                j = 0;
                plaintext += (ciphertext[i] - key[j] + 26) % 26 + 'a';
            }
            else {
                plaintext += (ciphertext[i] - key[j] + 26) % 26 + 'a';
            }
            j++;
        }
        else {
            plaintext += ciphertext[i];
        }
    }
    return plaintext;
}

