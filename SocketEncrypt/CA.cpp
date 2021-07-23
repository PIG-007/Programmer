#include <iostream>
#include<string>
#include<math.h>
#include "CA.h"
using namespace std;

string CA(string length, string prin)
{

    int len = stoi(length);
    int p = stoi(prin);
    int bp[8] = { 0 };
    int br[100] = { 0 };
    for (int a = 0; a < len; a++)
    {
        br[a] = 0;
    }
    int i = 7, j = len - 1;
    string key;

    if (p > 255 || p < 0)
    {
        //cout << "输入规则数不合规" << endl;
        return NULL;
    }

    int max = 0;
    for (int i = 0; i < len; i++)
    {
        max += pow(2, i);
    }

    int r = rand() % (max + 1);

    while (p != 0)
    {
        bp[i] = p % 2;
        i--;
        p = p / 2;
    }
    while (r != 0)
    {
        br[j] = r % 2;
        j--;
        r = r / 2;
    }

    int temp;
    for (i = 0; i < len; i++)
    {
        if (i == 0)
        {
            temp = br[i] * pow(2, 1) + br[i + 1] * pow(2, 0) + br[len - 1] * pow(2, 2);
            key += to_string(bp[temp]);
        }
        else if (i == len - 1)
        {
            temp = br[i] * pow(2, 1) + br[i - 1] * pow(2, 2) + br[0] * pow(2, 0);
            key += to_string(bp[temp]);
        }
        else if (i > 0 && i < len - 1)
        {
            temp = br[i] * pow(2, 1) + br[i - 1] * pow(2, 2) + br[i + 1] * pow(2, 0);
            key += to_string(bp[temp]);
        }
    }

    return key;
}
