#pragma once
#include <iostream>;
#include <string>
#include <memory.h>
using namespace std;

int* up2low(string& plain);//标记大写和空格
void low2up(string& plain, int* flag);//将大写和空格恢复