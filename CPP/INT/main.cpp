#include<iostream>
#include<string>
using namespace std;
#include"INT.h"


int main()
{
	string s1,s2;
	
	cin >> s1 >> s2;

	INT t1(s1),t2(s2);

	INT t3 = t1 + t2;

	t3.display();
	
	INT t4 = t1 - t2;

	t4.display();
	
	INT t5 = t1*t2;

	t5.display();
	
	INT t6 = t1/t2;

	t6.display();
	return 0;
}
