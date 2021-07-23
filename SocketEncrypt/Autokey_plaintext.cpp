#include "AutoKey_plaintext.h"

char(*vigenereTable(char(&list)[26][26]))[26]
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
string AtPlain_encode(string plaintext, string key) {
    string ciphertext = plaintext;
    int i, m, n;
    int flag = 0;
    char(*list)[26];
    string rule(50, '\0');
    char list1[26][26] = { 0 };
    list = vigenereTable(list1);
    for (i = 0;i < key.length();i++) {
        rule[i] = key[i];
    }
    rule[i] = '\0';
    for (i = 0;i < plaintext.length() - key.length();i++) {
        rule[i + key.length()] = plaintext[i];
    }
    rule[i + key.length()] = '\0';
    for (i = 0;i < plaintext.length();i++) {
        m = rule[i] - 'a';
        n = plaintext[i] - 'a';
        ciphertext[i] = list[n][m];
    }
    return ciphertext;
}
string AtPlain_decode(string ciphertext, string key) {
    int i, j, m, n;
    char(*list)[26];
    string pl(50, '\0');
    string rule(50, '\0');
    char list1[26][26] = { 0 };
    list = vigenereTable(list1);
    for (i = 0;i < key.length();i++) {
        rule[i] = key[i];
    }
    rule[i] = '\0';
    for (i = 0;i < ciphertext.length();i++) {
        m = rule[i] - 'a';
        for (j = 0;j < 26;j++) {
            if (list[j][m] == ciphertext[i])
                break;
        }
        n = 'a' + j;
        pl[i] = n;
        if (i < ciphertext.length() - key.length()) {
            rule[i + key.length()] = n;
            rule[i + key.length() + 1] = '\0';
        }
    }
    pl[i] = '\0';
    return pl;
}
