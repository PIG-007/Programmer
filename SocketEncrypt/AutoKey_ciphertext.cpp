#include "AutoKey_ciphertext.h"

char(*Cipher_vigenere_Table(char(&list)[26][26]))[26]
{
    int i,j,k;
    char ch;
    for (i = 0;i < 26;i++) {
        for (j = 0;j < 26;j++) {
            k = 'a' + (i + j) % 26;
            ch = k;
            list[i][j] = ch;
        }
    }
    return list;
}
string AtCipher_encode(string plaintext, string key)
{
    string ciphertext=plaintext;
    int i, j,m, n;
    char(*list)[26];
    string rule(50, '\0');
    char list1[26][26] = { 0 };
    list = Cipher_vigenere_Table(list1);
    j = 0;
    for (i = 0;i < key.length();i++) {
        j = j % key.length();
        rule[i] = key[j];
        j++;
    }
    rule[i] = '\0';
    for (i = 0;i < plaintext.length() - key.length();i++) {
        m = rule[i] - 'a';
        n = plaintext[i] - 'a';
        rule[i + key.length()] = list[n][m];
    }
    rule[i + key.length()] = '\0';
    for (i = 0;i < plaintext.length();i++) {
        m = rule[i] - 'a';
        n = plaintext[i] - 'a';
        ciphertext[i]= list[n][m];
    }
    return ciphertext;
}
string AtCipher_decode(string ciphertext, string key) {
    int i,j,m, n;
    char(*list)[26];
    string plaintext(50, '\0');
    string rule(50, '\0');
    char list1[26][26] = { 0 };
    list = Cipher_vigenere_Table(list1);
    j = 0;
    for (i = 0;i < key.length();i++) {
        j = j % key.length();
        rule[i] = key[j];
        j++;
    }
    rule[i] = '\0';
    for (i = 0;i < ciphertext.length() - key.length();i++) {
        rule[i + key.length()] = ciphertext[i];
    }
    rule[i + key.length()] = '\0';
    for (i = 0;i < ciphertext.length();i++) {
        m = rule[i] - 'a';
        for (j = 0;j < 26;j++) {
            if (list[j][m] == ciphertext[i])
                break;
        }
        n = 'a' + j;
        plaintext[i] = n;
    }
    return plaintext;
}
