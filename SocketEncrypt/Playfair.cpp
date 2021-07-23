#include <iostream>
#include<string>
#include "Playfair.h"
using namespace std;

char playfair_table[5][5];

void CreateTable(string in_key)
{
    int i = 0, j;
    int hash[26] = { 0 };

    char key[26] = { 0 };
    int key_len = in_key.length();
    for (int a = 0; a < key_len; a++)
    {
        key[a] = in_key[a];
    }

    while (1)
    {
        if (i == key_len)
            break;
        //转换成大写
        if (key[i] >= 'a' && key[i] <= 'z')
        {
            key[i] += 'A' - 'a';
        }
        //去除非法字符
        if (!(key[i] <= 'Z' && key[i] >= 'A'))
        {
            for (j = i; j < key_len - 1; ++j)
                key[j] = key[j + 1];
            key_len--;
            continue;
        }
        if (key[i] == 'J')
        {
            //I和J视为相同
            key[i] = 'I';
        }
        if (hash[key[i] - 'A'] == 0)
        {
            hash[key[i] - 'A'] = 1;
        }
        else 
        {
            for (j = i; j < key_len - 1; ++j)
                key[j] = key[j + 1];
            key_len--;
            continue;
        }
        i++;
    }

    if (key_len < 25)
    {
        for (i = 0; i < 26; i++)
        {
            if (key_len == 25)
            {
                break;
            }
            if (i + 'A' == 'J')
                continue;
            if (hash[i] == 0)
            {
                hash[i] = 1;
                key[key_len] = i + 'A';
                key_len++;
            }
        }
    }
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
            playfair_table[i][j] = key[5 * i + j];
    }

}

char GetKey(int x, int y, char type)
{
    if (x < 0)
        x += 5;
    if (y < 0)
        y += 5;
    if (type < 'a')
    {
        return playfair_table[x % 5][y % 5];
    }
    else
    {
        return (playfair_table[x % 5][y % 5] + 'a' - 'A');
    }
}

void Getposition(char c, int* x, int* y)
{
    int i, j;
    if (c >= 'a')
    {
        c += -'a' + 'A';
    }
    for (i = 0; i < 5; ++i)
    {
        for (j = 0; j < 5; ++j)
        {
            if (playfair_table[i][j] == c)
            {
                *x = i; *y = j;
                return;
            }
        }
    }
}

string Playfair_Encryption(string plaintext, string ciphertext)
{
    int a;
    int length = plaintext.length();
    char input[100] ={0};
    char output[100] = { 0 };
    for (a = 0; a < length; a++)
    {
        input[a] = plaintext[a];
    }

    int i = 0, j = -1, find;
    int ax, ay, bx, by;
    int pair[2] = { -1,-1 };

    for(i=0;i<length;i++)
    {
    	if(input[i]>='A'&&input[i]<='Z')
    	{
    		input[i]-='A'-'a';
    	}
    }
    while (1)
    {
        char between[1000] = { 0 };
        int betweenlength = -1;
        find = 0;
        for (i = pair[1] + 1; i < length; i++)
        {
            if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                pair[0] = i;
                pair[1] = i + 1;
                find = 1;
                break;
            }
            else
            {
                output[++j] = input[i];
            }
        }
        if (find == 0)
        {
            break;
        }
        
        find = 0;
        for (i = pair[1]; i < length; i++)
        {
            if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                pair[1] = i;
                find = 1;
                break;
            }
            else
            {
                between[++betweenlength] = input[i];
            }
        }
        if (find == 0)
        {
            input[length] = 'Q';
            pair[1] = length;
            length++;
        }
        
        if (input[pair[1]] == input[pair[0]])
        {
            if (input[pair[0]] != 'X')
            {
                  
                for (i = length; i >= pair[1]; i--)
                {
                    input[i + 1] = input[i];
                }
                length++;
                input[pair[1]] = 'x';
            }
        }
        Getposition(input[pair[0]], &ax, &ay);
        Getposition(input[pair[1]], &bx, &by);
        
        if (ax == bx)
        {
            output[++j] = GetKey(ax, ay + 1, input[pair[0]]);
            for (i = 0; i <= betweenlength; i++)
            {
                output[++j] = between[i];
            }
            output[++j] = GetKey(bx, by + 1, input[pair[1]]);
        }
         
        else if (ay == by)
        {
            output[++j] = GetKey(ax + 1, ay, input[pair[0]]);
            for (i = 0; i <= betweenlength; i++)
            {
                output[++j] = between[i];
            }
            output[++j] = GetKey(bx + 1, by, input[pair[1]]);
        }
         
        else
        {
            output[++j] = GetKey(ax, by, input[pair[0]]);
            for (i = 0; i <= betweenlength; i++)
            {
                output[++j] = between[i];
            }
            output[++j] = GetKey(bx, ay, input[pair[1]]);
        }
    }
        ciphertext = output;

    return ciphertext;
}

string Playfair_Decryption(string plaintext, string ciphertext)
{
    int a;
    int length = ciphertext.length();
    char input[100] = { 0 };
    char output[100] = { 0 };
    for (a = 0; a < length; a++)
    {
        input[a] = ciphertext[a];
    }

    int i = 0, j = -1, find;
    int ax, ay, bx, by;
    int pair[2] = { -1,-1 };
    while (1)
    {
        char between[1000] = { 0 };
        int betweenlength = -1;
        find = 0;
        
        for (i = pair[1] + 1; i < length; i++)
        {
            if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                pair[0] = i;
                pair[1] = i + 1;
                find = 1;
                break;
            }
            else
            {
                output[++j] = input[i];
            }
        }
        if (find == 0)
        {
            break;
        }
        
        find = 0;
        for (i = pair[1]; i < length; i++)
        {
            if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
            {
                pair[1] = i;
                find = 1;
                break;
            }
            else
            {
                between[++betweenlength] = input[i];
            }
        }
        if (find == 0)
        {
            input[length] = 'Q';
            pair[1] = length;
            length++;
        }
        if(input[pair[1]]==input[pair[0]])
        {
        	if(input[pair[0]]!='x')
        	{
        		   
        		for(i=length;i>=pair[1];i--)
        		{
        			input[i+1]=input[i];
        		}
        		length++;
        		input[pair[1]]='x';
        	}
        }
        Getposition(input[pair[0]], &ax, &ay);
        Getposition(input[pair[1]], &bx, &by);
        
        if (ax == bx)
        {
            output[++j] = GetKey(ax, ay - 1, input[pair[0]]);
            for (i = 0; i <= betweenlength; i++)
            {
                output[++j] = between[i];
            }
            output[++j] = GetKey(bx, by - 1, input[pair[1]]);
        }
         
        else if (ay == by)
        {
            output[++j] = GetKey(ax - 1, ay, input[pair[0]]);
            for (i = 0; i <= betweenlength; i++)
            {
                output[++j] = between[i];
            }
            output[++j] = GetKey(bx - 1, by, input[pair[1]]);
        }
        
        else
        {
            output[++j] = GetKey(ax, by, input[pair[0]]);
            for (i = 0; i <= betweenlength; i++)
            {
                output[++j] = between[i];
            }
            output[++j] = GetKey(bx, ay, input[pair[1]]);
        }
    }
    for (i = 1; output[i] != '\0'; i++)
    {
        if (output[i] == 'X')
        {
            int m = 0, s = -1, e = -1;
            for (m = i - 1; m >= 0; m--)
            {
                if ((output[m] >= 'a' && output[m] <= 'z') || (output[m] >= 'A' && output[m] <= 'Z'))
                {
                    s = m;
                    break;
                }
            }
            for (m = i + 1; output[m] != '\0'; m++)
            {
                if ((output[m] >= 'a' && output[m] <= 'z') || (output[m] >= 'A' && output[m] <= 'Z'))
                {
                    e = m;
                    break;
                }
            }
            if (s != -1 && e != -1 && (output[s] == output[e] || output[s] - output[e] == 'A' - 'a' || output[s] - output[e] == 'a' - 'A'))
            {
                for (m = i; output[m] != '\0'; m++)
                {
                    output[m] = output[m + 1];
                }
            }
        }
    }
    if (output[i - 1] == 'Q')
    {
        output[i - 1] = '\0';
    }

    plaintext = output;

    return plaintext;
}

