#include "RC4.h"
string RC4_encrypt(char data[], char key[]) {

   // char* temp;
    //异或加密
	unsigned char k[1024];
	unsigned char S[256];
	unsigned char T[256];
	int keylen;
	int datalen;
	int i, j = 0;
	//检测密钥长度
	for (i = 0;; i++) {
		if (key[i] == '\0') {
			keylen = i;
			break;
		}
	}
	//检测明文长度
	for (i = 0;; i++) {
		if (data[i] == '\0') {
			datalen = i;
			break;
		}
	}
	//KSA key scheduling算法初始化S
	for (i = 0; i < 256; i++) {
		S[i] = i;
	}
	i = 0;
	if (keylen >= 256) {
		for (i = 0; i < 256; i++) {
			T[i] = key[i];
		}
	}
	else {
		while (1) {
			for (j = 0; j < keylen; j++) {
				T[i++] = key[j];
				if (i >= 256)break;
			}
			if (i >= 256)break;
		}
	}
	i = 0;
	j = 0;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + T[i]) % 256;
		unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = S[i];
	}
	//PRGA伪随机生成算法，生成密钥流
	int t;
	int m = 0;
	int times = datalen;
	while (times-- > 0) {//相当于执行明文长度次，这样生成的秘钥流也是明文长度个字节
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
        unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = S[i];
		t = (S[i] + S[j]) % 256;
		k[m++] = S[t];//生成密钥流并存储
    }
   // cout << "The secret key is: ";
  //  for (i = 0; i < datalen;i++) {
   //     cout << (bitset<8>)k[i] << " ";
  //  }
  //  cout << endl;

    for (int i = 0; i < datalen;i++) {
        data[i] = data[i] ^ k[i];
    }
  //  cout << "The secret message is: ";
    //指定8位2进制输出
    string s;
    for (int i = 0; i < datalen; i++) {
       // s+=((bitset<8>)data[i]).to_string();
        s+=data[i];
      //  cout<< (bitset<8>)data[i]<<"    ";
    }
    s+="\n";
    for (int i = 0; i < datalen; i++) {
        s+=((bitset<8>)data[i]).to_string();
        //s+=data[i];
      //  cout<< (bitset<8>)data[i]<<"    ";
    }
//    cout << endl;


    return s;
}
string RC4_decrypt(char data[], char key[]) {

    char* temp;
    //异或解密
	unsigned char k[1024];
	unsigned char S[256];
	unsigned char T[256];
	int keylen;
	int datalen;
	int i, j = 0;
	//检测密钥长度
	for (i = 0;; i++) {
		if (key[i] == '\0') {
			keylen = i;
			break;
		}
	}
	//检测明文长度
	for (i = 0;; i++) {
		if (data[i] == '\0') {
			datalen = i;
			break;
		}
	}
	//KSA key scheduling算法初始化S
	for (i = 0; i < 256; i++) {
		S[i] = i;
	}
	i = 0;
	if (keylen >= 256) {
		for (i = 0; i < 256; i++) {
			T[i] = key[i];
		}
	}
	else {
		while (1) {
			for (j = 0; j < keylen; j++) {
				T[i++] = key[j];
				if (i >= 256)break;
			}
			if (i >= 256)break;
		}
	}
	i = 0;
	j = 0;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + T[i]) % 256;
		unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = S[i];
	}
	//PRGA伪随机生成算法，生成密钥流
	int t;
	int m = 0;
	int times = datalen;
	while (times-- > 0) {//相当于执行明文长度次，这样生成的秘钥流也是明文长度个字节
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = S[i];
		t = (S[i] + S[j]) % 256;
		k[m++] = S[t];//生成密钥流并存储
    }
  //  cout << "The secret key is: ";
  //  for (i = 0; i < datalen;i++) {
  //      cout << (bitset<8>)k[i] << " ";
  //  }
  //  cout << endl;

    for (int i = 0; i < datalen; i++) {
        data[i] = data[i] ^ k[i];
    }
  //  cout << "The decrypted message is: ";
    //指定8位2进制输出
    string s;
    for (int i = 0; i < datalen; i++) {
        s+=data[i];
    }
    s+=" ";
    for (int i = 0; i < datalen; i++) {
        s+=((bitset<8>)data[i]).to_string();
    }
   // cout << endl;

    return s;
}

