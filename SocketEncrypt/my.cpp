//
// Created by 007 on 2021/7/2.
//

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;


static const unsigned char sBox[] =
        { /*     0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
                0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, /*0*/
                0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0, /*1*/
                0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, /*2*/
                0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, /*3*/
                0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, /*4*/
                0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf, /*5*/
                0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8, /*6*/
                0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, /*7*/
                0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, /*8*/
                0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, /*9*/
                0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79, /*a*/
                0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08, /*b*/
                0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, /*c*/
                0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e, /*d*/
                0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, /*e*/
                0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16  /*f*/
        };

static const unsigned char invSBox[] =
        {/*      0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
                0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,/*0*/
                0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,/*1*/
                0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,/*2*/
                0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,/*3*/
                0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,/*4*/
                0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,/*5*/
                0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,/*6*/
                0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,/*7*/
                0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,/*8*/
                0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,/*9*/
                0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,/*a*/
                0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,/*b*/
                0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,/*c*/
                0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,/*d*/
                0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,/*e*/
                0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d/*f*/
        };



static const int enColumnMixMatrix[4][4] = {2, 3, 1, 1,
                                            1, 2, 3, 1,
                                            1, 1, 2, 3,
                                            3, 1, 1, 2 };

static const int deColumnMixMatrix[4][4] = {0xe, 0xb, 0xd, 0x9,
                                            0x9, 0xe, 0xb, 0xd,
                                            0xd, 0x9, 0xe, 0xb,
                                            0xb, 0xd, 0x9, 0xe };

//伽罗华域求解
int GFMul2(int s) {
    int result = s << 1;
    int a7 = result & 0x00000100;

    if(a7 != 0) {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }

    return result;
}
int GFMul3(int s) {
    return GFMul2(s) ^ s;
}
int GFMul4(int s) {
    return GFMul2(GFMul2(s));
}
int GFMul8(int s) {
    return GFMul2(GFMul4(s));
}
int GFMul9(int s) {
    return GFMul8(s) ^ s;
}
int GFMul11(int s) {
    return GFMul9(s) ^ GFMul2(s);
}
int GFMul12(int s) {
    return GFMul8(s) ^ GFMul4(s);
}
int GFMul13(int s) {
    return GFMul12(s) ^ s;
}
int GFMul14(int s) {
    return GFMul12(s) ^ GFMul2(s);
}
int GFMul(int n, int s) {
    int result;

    if(n == 1)
        result = s;
    else if(n == 2)
        result = GFMul2(s);
    else if(n == 3)
        result = GFMul3(s);
    else if(n == 0x9)
        result = GFMul9(s);
    else if(n == 0xb)//11
        result = GFMul11(s);
    else if(n == 0xd)//13
        result = GFMul13(s);
    else if(n == 0xe)//14
        result = GFMul14(s);

    return result;
}
void mixColumns(char** textBlock,int flag) {

    int mixMatrix[4][4];
    if(flag == 1)
    {
        memcpy(mixMatrix,enColumnMixMatrix,sizeof(int)*16);
    } else
    {
        memcpy(mixMatrix,deColumnMixMatrix,sizeof(int)*16);
    }

    int array[4][4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            array[i][j] = textBlock[i][j];
        }
    }

    int tempArray[4][4];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            array[i][j] = GFMul(mixMatrix[i][0], tempArray[0][j]) ^ GFMul(mixMatrix[i][1], tempArray[1][j])
                          ^ GFMul(mixMatrix[i][2], tempArray[2][j]) ^ GFMul(mixMatrix[i][3], tempArray[3][j]);
        }
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            textBlock[i][j] = array[i][j];
        }
    }
}

//yes
char** aesInitMatrix(string block)
{
    char** blockMatrix = new char*[4];
    for (int i = 0; i < 4; ++i) {
        blockMatrix[i] = new char[4];
        for (int j = 0; j < 4; ++j) {
            blockMatrix[i][j] = block[i + 4 * j]&0xff;
        }
    }
    return blockMatrix;
}

//yes
void keyAdd(char** textBlock, char** subKeyBlock)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            textBlock[i][j] = textBlock[i][j] xor subKeyBlock[i][j];
        }
    }
}
void bySBOXText(char** textBlock, int flag)
{
    int row,column;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            row = (textBlock[i][j] >> 4) & 0xf;
            column = textBlock[i][j] & 0xf;
            if(flag == 1) //Encrypt
            {
                textBlock[i][j] = sBox[row * 16 + column];
            } else
            {
                textBlock[i][j] = invSBox[row * 16 + column];
            }
        }
    }
}
void byShiftRowsText(char** textBlock, int* shiftRule)
{
    char* temp = new char[4];
    for (int i = 0; i < 4; ++i) {
        memcpy(temp, textBlock[i], 4);
        for (int j = 0; j < 4; ++j) {
            textBlock[i][j] = temp[(j + shiftRule[i]) % 4];
        }
    }
    delete[] temp;
    temp = nullptr;
}

//key function
void byLeftShifRowsKey(char* tempKey)
{
    char* temp = new char[4];
    memcpy(temp,tempKey,4);
    for (int i = 0; i < 4; ++i) {
        tempKey[i] = temp[(i+1)%4];
    }
    delete[] temp;
    temp = nullptr;
}
void bySBOXKey(char* tempKey)
{
    int row,column;
    for (int i = 0; i < 4; ++i) {
        row = (tempKey[i] >> 4)& 0xf;
        column = tempKey[i] & 0xf;
        tempKey[i] = sBox[row*16+column];
    }
}
char** genKey(char** originalKey)
{
    char** key = new char*[4];
    for (int i = 0; i < 4; ++i) {
        key[i] = new char[44];
        for (int j = 0; j < 4; ++j) {
            key[i][j] = originalKey[i][j];
        }
    }


    int rConst[11] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x1b,0x36};
    char* tempKey = new char[4];
    int count = 0;
    for (int i = 4; i < 44; ++i) {
        if((i % 4) == 0)
        {
            //calculate T[W(i-1)]
            for (int j = 0; j < 4; ++j) {
                tempKey[j] = key[j][i-1];
            }
            byLeftShifRowsKey(tempKey);
            bySBOXKey(tempKey);
            tempKey[0] = tempKey[0] xor rConst[count];
            count ++;

            //calculate W(i) = W(i-4) xor T[W(i-1)]
            for (int j = 0; j < 4; ++j) {
                key[j][i] = tempKey[j] xor key[j][i - 4];
            }
        } else
        {
            for (int j = 0; j < 4; ++j) {
                key[j][i] = key[j][i - 1] xor key[j][i - 4];
            }
        }
    }
    return key;
}
char** getSubKeyBlock(char** key, int roundAmount)
{
    char** subKeyBlock = new char*[4];
    for (int i = 0; i < 4; ++i) {
        subKeyBlock[i] = new char[4];
        for (int j = roundAmount*4; j < roundAmount*4+4; ++j) {
            subKeyBlock[i][j-roundAmount*4] = key[i][j];
        }
    }
    return subKeyBlock;
}

//circulation function
void enCryptCircuStruct(char** plaintextBlock, char** key)
{
    int shiftRule[4] = {0,1,2,3};
    char ** subKeyBlock;
    for (int i = 1; i < 11; ++i) {
        bySBOXText(plaintextBlock,1);
        byShiftRowsText(plaintextBlock, shiftRule);
        if(i != 10)
        {
            mixColumns(plaintextBlock,1);
        }
        subKeyBlock = getSubKeyBlock(key, i);
        keyAdd(plaintextBlock,subKeyBlock);
    }
}
void deCryptCircuStruct(char** cipherTextBlock, char** key)
{
    int shiftRule[4] = {0,3,2,1};
    char ** subKeyBlock;
    for (int i = 9; i >= 0; i --) {

        byShiftRowsText(cipherTextBlock, shiftRule);  //yes



        bySBOXText(cipherTextBlock, 0);//yes


        subKeyBlock = getSubKeyBlock(key, i);//yes



        keyAdd(cipherTextBlock,subKeyBlock);



        if(i)
        {
            mixColumns(cipherTextBlock,0);

        }
    }
}

//hex function
string aesHexToStrChar(string strCiphertext)
{
    string strTemp = "";
    string ciphertextChar = "";
    for (int i = 0; i < strCiphertext.length(); i = i+2) {
        strTemp.clear();
        strTemp += strCiphertext[i];
        strTemp += strCiphertext[i+1];
        ciphertextChar += char(stoi(strTemp,0,16)&0xff);
    }
    return ciphertextChar;
}


//dataBase=16(hex)  dataBase=else(string) aes-128
string aesEncrypt(string strPlaintextBlock, int dataBase, string strOriginalKey = "PIGOOTNB") {
    char *temp = new char[3];
    string ciphertext;
    char **plaintTextBlock;
    if (dataBase == 16) {
        plaintTextBlock = aesInitMatrix(aesHexToStrChar(strPlaintextBlock));
    } else
    {
        plaintTextBlock = aesInitMatrix(strPlaintextBlock);
    }
    char** originalKey = aesInitMatrix(strOriginalKey);
    char** key = genKey(originalKey);
    char** subKeyBlock = getSubKeyBlock(key, 0);
    keyAdd(plaintTextBlock, subKeyBlock);
    enCryptCircuStruct(plaintTextBlock, key);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            _itoa_s(plaintTextBlock[j][i]&0xff,temp,3,16);
            if(temp[0] == '\x00')
            {
                ciphertext += "00";
                ciphertext += " ";
            } else
            {
                ciphertext = ciphertext + temp + " ";
            }

        }
    }
    delete[] temp;
    temp = nullptr;
    return ciphertext;
}
string aesDecrypt(string strCiphertextBlock, int dataBase, string strOriginalKey = "PIGOOTNB")
{
    char* temp = new char[3];
    string plaintext = "";
    char** cipherTextBlock;
    if (dataBase == 16) {
        cipherTextBlock = aesInitMatrix(aesHexToStrChar(strCiphertextBlock));
    } else
    {
        cipherTextBlock = aesInitMatrix(strCiphertextBlock);
    }
    char** originalKey = aesInitMatrix(strOriginalKey);
    char** key = genKey(originalKey);
    char** subKeyBlock = getSubKeyBlock(key, 10);
    keyAdd(cipherTextBlock,subKeyBlock);
    deCryptCircuStruct(cipherTextBlock,key);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            plaintext += cipherTextBlock[j][i] & 0xff;
        }
    }
    return plaintext;

}

string multEncrypt(string plaintext,string strOriginalKey = "PIGOOT")
{
    string key;
    string ciphertext;
    string multMatrix = {
            "abcde"
            "fghik"
            "lmnop"
            "qrstu"
            "vwxyz"};
    sort(strOriginalKey.begin(), strOriginalKey.end());
    unique(strOriginalKey.begin(), strOriginalKey.end());
    for (int i = 0 ; i < 5 ; i ++) {
        key += strOriginalKey[i];
    }

    int row,column;
    for (auto i : plaintext) {
        row = multMatrix.find(i)/5;
        column = multMatrix.find(i) % 5;
        ciphertext += key[row];
        ciphertext += key[column];
    }

    return ciphertext;

}
string multDecrypt(string ciphertext,string strOriginalKey = "PIGOOT")
{
    string key;
    string plaintext;
    string multMatrix = {
            "abcde"
            "fghik"
            "lmnop"
            "qrstu"
            "vwxyz"};
    sort(strOriginalKey.begin(), strOriginalKey.end());
    unique(strOriginalKey.begin(), strOriginalKey.end());
    for (int i = 0 ; i < 5 ; i ++) {
        key += strOriginalKey[i];
    }

    int row,column;
    for (int i = 0; i < ciphertext.length();i += 2) {
        row = key.find(ciphertext[i]);
        column = key.find(ciphertext[i+1]);
        plaintext += multMatrix[row*5+column];
    }
    return plaintext;


}

string permuEncrypt(string plaintext,string strOriginalKey = "PIGOOT")
{
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
    char chrKey[5];
    string ciphertext;
    unique(strOriginalKey.begin(), strOriginalKey.end());

    //get key
    int queIdx = 0;
    for(int i = 0;i <= 25;i++ )
    {
        for(int j = 0 ; j < 5 ; j++)
        {
            if(alphabet[i] == strOriginalKey[j])
            {
                chrKey[j] = char(queIdx + '0');
                queIdx = queIdx + 1;
            }
        }
    }
    string key = chrKey;


    //fill the plaintext with '~'
    while(plaintext.length()%5 != 0)
    {
        plaintext += '~';
    }

    int moduleAmount = plaintext.length()/5;
    string module;
    for (int i = 0; i < moduleAmount; ++i)
    {
        module = plaintext.substr(i*5,5);
        for(int j = 0 ; j < 5 ; j ++)
        {
            ciphertext += module[key.find(char(j+'0'))];
        }
    }
    return ciphertext;
}
string permuDecrypt(string ciphertext,string strOriginalKey = "PIGOOT")
{
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
    char chrKey[5];
    string plaintext;
    unique(strOriginalKey.begin(), strOriginalKey.end());

    //get key
    int queIdx = 0;
    for(int i = 0;i <= 25;i++ )
    {
        for(int j = 0 ; j < 5 ; j++)
        {
            if(alphabet[i] == strOriginalKey[j])
            {
                chrKey[j] = char(queIdx + '0');
                queIdx = queIdx + 1;
            }
        }
    }
    string key = chrKey;

    int moduleAmount = ciphertext.length()/5;
    string module;
    for (int i = 0; i < moduleAmount; ++i)
    {
        module = ciphertext.substr(i*5,5);
        for(int j = 0 ; j < 5 ; j ++)
        {
            plaintext += module[int(key[j]-'0')];
        }
    }


    int position = plaintext.find('~');
    while (position != plaintext.npos)
    {
        plaintext.erase(position,1);
        position = plaintext.find('~');
    }
    return plaintext;
}

