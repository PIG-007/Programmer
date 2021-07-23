#include "tool.h"

int* up2low(string& plain)//flag±Íº«¥Û–¥
{
	int len = plain.length();
	int* flag = (int*)malloc(len * sizeof(int));
	for (int i = 0, j = 0; i < len; i++, j++)
	{
		if (plain[j] >= 'A' && plain[j] <= 'Z')
		{
			flag[i] = 1;
			plain[j] -= 'A' - 'a';
		}
		else if (plain[j] == ' ')
		{
			flag[i] = 2;
			plain.erase(j, 1);
			j--;
		}
		else {
			flag[i] = 0;
		}
	}
	return flag;
}

void low2up(string& plain, int* flag)
{
	int len = sizeof(flag);
	for (int i = 0,j = 0; i < len; i++,j++)
	{
		if (flag[i] == 2)

		{
			plain.insert(j, 1, ' ');
		}
		else if (flag[i] == 1)
		{
			plain[j] += 'A' - 'a';
		}
	}
}