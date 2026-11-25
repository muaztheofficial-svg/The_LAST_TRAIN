#include "util.h"
#include<iostream>
#include<cstdlib>
using namespace std;
int clampINt(int val, int minVal, int maxVal)
{
if (val<minVal)
return minVal;
if (val>maxVal)
return maxVal;
return val;}
bool stringEquals(const char* a, const char* b)
{
if (!a || !b)
return false;
int i=0;
while (a[i]!='\0' && b[i]!='\0')
{
if (a[i]!=b[i])
return false;
i++;}
return (a[i]=='\0' && b[i]=='\0');}
void clearScreen()
{
system("clear");}
// Primitive helper utilities, clamping, and screen control

