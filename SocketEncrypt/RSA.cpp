#include "RSA.h"

string RSAEncrypt(string plain, long publickey, long mod)
{
	string res;
	for (int i = 0; i < plain.length(); i++)
	{
		char p = plain[i];
		long pt = quickExp(p,publickey, mod);
		res += dec2hex(pt,4);
	}
	return res;
}

string RSADecrypt(string cipher, long privatekey, long mod)
{
	string res;
	int count = 0;
	while (count < cipher.length())
	{
		int limit = 0;
		string t;
		while (limit <4)
		{
			t += cipher[count];
			limit++;
			count++;
		}
		long n = hex2dec(t,4);
		n = quickExp(n, privatekey, mod);
		res += (char)n;
	}
	return res;
}

void generateKey(long& publickey, long& privatekey,long& mod,long& pp, long& qq)
{

	srand((unsigned)time(NULL));
	long p = generatePrime(200,10);
	long q;
	while (1)
	{
		q = generatePrime(200,10);
		if (q != p)
			break;
	}
    pp = p;
    qq = q;
	long fin = (p - 1) * (q - 1);
	long n = p * q;
	long d;//n + d=公钥
	long e;//n + e=私钥
	while (1)
	{
		e = rand()*rand()%(fin-1);
		if (ifPrime2each(e, fin) && e >p && e > q)
			break;
	}
	d = getOpposite(e, fin);
	publickey = d;
	privatekey = e;
	mod = n;
	cout << "p        :" << p << endl;
	cout << "q        :" << q << endl;
	cout << "public  d:" << d << endl;
	cout << "private e:" << e << endl;
	cout << "mod n    :" << n << endl;
}

unsigned long generatePrime(unsigned long max,unsigned  min)
{
	unsigned long p;
	srand((unsigned)time(NULL));
	while (1)
	{
		p = min + (rand() * rand()) % (max - min);
		if (ifPrime(p))
			break;
	}
	return p;
}


bool ifPrime(unsigned long p)
{
	if (((p & 1) != 0) && (p % 3 != 0) 
		&& (p > 2) && M_R(2, p) && ((p <= 7) 
		|| M_R(7, p)) && ((p <= 61) || M_R(61, p)) 
		|| (p == 2) || (p == 3))
		//检验2 3 7 61 可保证十亿级大数
		return true;
	else 
		return false;
}

bool M_R(long long base, long num)//Miller-Rabin素性检验
{
	int d = num - 1;
	int t;
	while ((d & 1) == 0)
	{
		d = (d >> 1);
	}

	if ((quickExp(base, d, num) == 1) || (quickExp(base, d, num) == num - 1)) return true;
	else
	{
		t = (num - 1) / 2;
		while (d != t)
		{
			d = (d << 1);
			if (quickExp(base, d, num) == num - 1) return true;
		}
		return false;
	}
}

long quickExp(long a, long k , long m) //a^k mod m
{
	unsigned long b = 1;//结果sum
	while (k >= 1) {			
		if (k % 2)//k为奇数 sum = sum* a^(sum-1)
			b = (a * b) % m;
		a = (a * a) % m;
		k /= 2;
	}
	return b;
}

int getOpposite( long int num,  long int mode)//求num(mod mode)的逆元
{
	// mode = num * mode/num + mode%num
	if (!ifPrime2each(num, mode))
		return 0;
	//确定互素
	int x1, x2, x3;
	int y1, y2, y3;
	int q;
	int t1, t2, t3;
	x3 = num;
	y3 = mode;
	x1 = 1; x2 = 0;
	y1 = 0; y2 = 1;
	int t;
	int l;
	l = x3;
	int k;
	k = y3;
	if (x3 < y3)
	{
		t = y3;
		y3 = x3;
		x3 = t;
		x1 = 0;
		x2 = 1;
		y1 = 1;
		y2 = 0;
	}
	while (y3 != 0)
	{
		q = x3 / y3;
		t1 = x1 - q * y1;
		t2 = x2 - q * y2;
		t3 = x3 - q * y3;
		x1 = y1;
		x2 = y2;
		x3 = y3;
		y1 = t1;
		y2 = t2;
		y3 = t3;

	}
	if (x1 < 0)
		x1 += mode;
	return x1;
}
	
bool ifPrime2each( long  num1,  long  num2)
{
	unsigned long int m, n,temp = 0;
	if (num1 == num2)
		return false;
		m = num1;
		n = num2;
		temp = m % n;
	while (temp != 0)
	{
		m = n;
		n = temp;
		temp = m % n;
	}
	if (n == 1)
		return true;
	else
		return false;
}


string dec2hex(long i,int width)
{
	stringstream ioss;     //定义字符串流
	string s_temp;         //存放转化后字符
	ioss << hex << i;      //以十六制形式输出
	ioss >> s_temp;

	if (width > s_temp.size())
	{
		string s_0(width - s_temp.size(), '0');      //位数不够则补0
		s_temp = s_0 + s_temp;                            //合并
	}

	string s = s_temp.substr(s_temp.length() - width, s_temp.length());    //取右width位
	return s;
}

long hex2dec(string s, int width)
{
	long temp;
	int totalNum = 0;
	int position = width - 1;
	int lastChar;
	for (; position >= 0; position--) {
		char c = s[position];
		int num = 0;
		// A～F转换成10-15,数字字符转数字0-9
		if (c >= 'a' && c <= 'f') {
			num = c - 'a' + 10;
		}
		else {
			num = c - '0';
		}
		if (position >= 0) {
			int exponent = width - 1 - position ; // 指数
			int exponentNum = 1; // 十六进制对应指数结果值
			for (; exponent > 0; exponent--)
			{
				exponentNum *= 16;
			}
			num *= exponentNum; // 计算当前位置对应的结果值
		}
		totalNum += num;
	}
	return totalNum;
}
