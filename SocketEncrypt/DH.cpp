#include "DH.h"

void genertateDHKey(long &P,long & G,int &A,int &B)
{
	P = generatePrime(200, 100);//mod
	G = generator(P);//生成元
	A = rand() % (P - 2);
	B = rand() % (P - 2);
}

void switchKey(long& P, long& G, int& A, int& B, long &g_a,long &g_b, long &key_a,long &key_b)
{
	g_a = quickExp(G, A, P);
	g_b = quickExp(G, B, P);
	key_a = quickExp(g_b, A, P);
	key_b = quickExp(g_a, B, P);
}

long generator(long p)//生成元
{
	long a = 1;
	long q = (p - 1) / 2;
	while (1)
	{
		if (quickExp(a, 2, p) != 1 && quickExp(a, q, p) != 1)
		{
			return a;
		}
		a++;
	}
}
