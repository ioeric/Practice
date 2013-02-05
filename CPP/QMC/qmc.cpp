/*
Quine-McCluskey algorithm written in CPP by Eric Liu
(http://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm)
===================================================================================
Example:
To simplify function f(A,B,C,D) = A'BC'D' + AB'C'D' + AB'CD' + AB'CD + ABC'D' + ABCD 
				= Sum of Products (4, 8, 10, 11, 12, 15)
		With don't care of AB'C'D, ABCD' which are d(9, 11)

The simplified function is f(A,B,C,D) = BC'D' + AC'D' + ACD + AB'
===================================================================================
Run
===================================================================================
./qmc
Inputse input number of variable: 4
Please input the number of products: 
6
Please input combinations: 
4 8 10 11 12 15
Please input the number of don't care combination: 2
Please input don't care combinations: 
9 11
BC'D'  AC'D'  ACD  AB'  
the number of variables:4 
===================================================================================
*/


#include<iostream>
#include<string>
#include<cstring>
#include<sstream>
#include<map>
#include<fstream>
#include<cmath>
#include<stdlib.h>
using namespace std;


int count_res = 0;
map<string,int> mapp;
string res[1000][2];
int weight[1000];
string chosen[1000];
int value[1000];
bool mark[20][20][100];
int leng;
int count_one[200];	
string m[20][20][100][2];//处理次数，1的个数，其中的第几个，0：名字 1：串
int count[20][20]; //第i次处理，1的个数为j的串的个数
int max_pos = 0;
int num;
bool only_one_digit(string s1,string s2,int& pos);
string combine(string s1,string s2,int pos);
int mark_res[10000];
bool mark_row[100];
int leave,leave_row;
void handle(int pos)
{
	//之后可用位运算优化
	bool flag=false;
	for (int i=0;i<=leng;i++)  //generate the next row and mark the string used this time
		for (int j=0;j<count[pos][i];j++)
		{
			for (int t=i+1;t<=leng;t++)
			for (int k=0;k<count[pos][t];k++)
			{
				int digit_pos;
				if (    only_one_digit( m[pos][i][j][1] , m[pos][t][k][1] , digit_pos ) )
				{
					flag = true;
					mark[pos][i][j] = false;
					mark[pos][t][k] = false;
					int p = count[pos+1][i]++;
					m[pos+1][i][p][1] = combine(m[pos][i][j][1], m[pos][t][k][1], digit_pos);
					m[pos+1][i][p][0] = m[pos][i][j][0] +m[pos][t][k][0]+' ';
				}
			}
		}

	if (flag)
		handle(pos+1);
	else
	{	
		max_pos = pos;
		return;
	}
		
		
}

int count_weight(string s)
{
	int count_v = 0;
	for (int i=0;i<s.length();i++)
	if (s[i] == '1' || s[i]=='0')
	{
		++count_v;
	}
	return count_v;
}

int value_of(string str)
{
	int tmp = 0;
	int st=0,ed;
	string s;
	for (int i=0;i<str.length();i++)
	if (str[i] == ' ')
	{
		ed = i;
		s = str.substr(st,ed-st);
		tmp = tmp | mapp[ s ];
		st = i+1;
	}
	return tmp;

}
	
bool judge_chosen(int st,int now,string str)
{
	bool flag = true;
	for (int i=st;i<now;i++)
		if (res[i][1] == str)
			flag = false;
	
	return flag;
}

void show(string s)
{
	for (int i=0;i<s.length();i++)
	if (s[i] != '-')
	{
		if (s[i] == '1')
			cout << (char)('A'+i);
		else
			cout << (char)('A'+i) << "'";
	}

}


void step1();
void step2(int *,int);
void step3();
void step4();
void step5();

void essential_prime()
{
	memset(weight,0,sizeof(int)*1000);
	memset(weight,0,sizeof(value));
	//Choose all combinaions mark with false into res arrayi
	
	for (int pos=0;pos<=max_pos;pos++)
		for (int i=0;i<=leng;i++)
		{
			int count_st = count_res+1;	
			for (int j=0;j<count[pos][i];j++)
			if (mark[pos][i][j] == true && judge_chosen(  count_st,count_res+1,m[pos][i][j][1]  ) )
			{
				++count_res;
				res[count_res][0] = m[pos][i][j][0];
				res[count_res][1] = m[pos][i][j][1];
				weight[count_res] = count_weight(res[count_res][1]);
				value[count_res] = value_of(res[count_res][0]);
			}
		}
/*	for (int i=1;i<=count_res;i++)     //////////TEST
	{
		cout << res[i][0] << '\t' << res[i][1] << '\t';
		cout <<value[i]<< '\t' <<weight[i] <<endl;
		
		
	}
*/
	memset(mark_res,0,sizeof(mark_res));
	memset(mark_row,true,sizeof(mark_row));
	leave = count_res;
	leave_row = num;
	step1();
	for (int i=1;i<=count_res;i++)
	if (mark_res[i] > 0)
	{
		show(res[i][1]);
		if (i == count_res) break;
		cout << " + ";
	}
	cout << endl;
}

string bin_to_int_str(string b);
int num_of_one(string s);
/////////////////////////////////////////////////////////////////////////////MAIN is here!!!
int main()
{
	string m0[200][2];//最初的输入
	
	memset(mark,1,sizeof(mark));
	memset(count_one,0,sizeof(int)*200);
	memset(count,0,sizeof(int)*20*20);
	//ifstream data("data.in");
	
	cout << "<Please input number of variable: > ";
	cin >> leng;
	cout << "<Please input the number of products: > ";
	cin >> num;
	if (num > 0)
		cout << "<Please input combinations: >" << endl; 
	int value_bin = 1;
	int temp_in;
	for (int i=0;i<num;i++)
	{
		
		cin >> m0[i][0];
		m0[i][1] = bin_to_int_str(m0[i][0]); //Here is a function
		//cout << m0[i][1]<< endl;
		mapp[ m0[i][0] ] = value_bin;
		value_bin = value_bin << 1;
		count_one[i] = num_of_one(m0[i][1]);
	}

	cout << "<Please input the number of don't care combination: > ";
	int num2;
	cin >> num2;
	if (num2 > 0)
		cout << "Please input don't care combinations: " << endl;
	for (int i=0;i<num2;i++)
	{
		cin >> m0[num+i][0];
		m0[num+i][1] = bin_to_int_str(m0[num+i][0]);
		
		count_one[num+i] = num_of_one(m0[num+i][1]);
	}
	// Sorted by the number of 1 digits and the value of  minterm 
	
	for (int i=0;i<num+num2-1;i++)
	{	
		int min = i;
		for (int j=i+1;j<num+num2;j++)
		{
			if (count_one[j]<count_one[min] || count_one[j]==count_one[min] && m0[j][0]<m0[min][0])
				min= j;

			
		}
		if (i!=min)
		{
			string tmp = m0[i][0];
			m0[i][0] = m0[min][0];
			m0[min][0] = tmp;
			tmp = m0[i][1];
			m0[i][1] = m0[min][1];
			m0[min][1] = tmp;
			int temp = count_one[i];
			count_one[i] = count_one[min];
			count_one[min] = temp;
		}
	}
	for (int i=0;i<num+num2;i++)  //generate the original table
	{
		int one_num = count_one[i];
		int j = count[0][ one_num ]++;
		m[0][ one_num ][ j ][0] = m0[i][0]+' ';
		m[0][ one_num ][j][1] = m0[i][1];
	}

	handle(0);   //generate prime
	essential_prime();//choose essential
	//getchar();getchar();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool only_one_digit(string s1,string s2,int& pos)
{
	int count_it = 0;
	for (int i=0;i<leng;i++)
	if (s1[i]!=s2[i])
	{
		++count_it;
		pos = i;
	}
	if (count_it == 1)
		return true;
	else
		return false;
}
string combine(string s1,string s2,int pos)
{
	s1[pos] = '-';
	string ss = s1;
	return s1;

}
string bin_to_int_str(string b)
{
	int m=0;
	for (int i=0;i<b.length();i++)
	{	
		m = m*10+(int)(b[i]-'0');
	}
	string ss;
	for (int i=leng-1;i>=0;--i)
	{
		int temp = 1 << i;
		if ((temp&m)==temp)
			ss = ss + "1";
		else
			ss = ss + "0";
		
	}
	//cout << ss<< endl;
	return ss;

}
int num_of_one(string s)
{
	int tmp = 0;
	for (int i=0;i<s.length();i++)
	if (s[i] == '1' )
	{
		tmp++;
	}
	return tmp;
}

bool is_loop()
{
	bool ess_num = true;
	int pos = 1;
	
	for (int i=0;i<num;i++)
	if (mark_row[i])
	{
		int tmp = pos << i;
		int cover_time = 0;
		
		for (int i=1;i<=count_res;++i)
		if (mark_res[i] == 0)
		if (cover_time <=1)
		{
			if (  (tmp & value[i]) == tmp )
			{
				++cover_time;
				
			}
		
		}
		
		if ( cover_time == 1 )
		{
			ess_num = false;
			break;
		}
	}
	return ess_num;
}
void step1()
{
	int pos = 1;
	int *vec = new int[100];
	int count_chosen = 0;
	for (int i=0;i<num;i++)
	if (mark_row[i])
	{
		int tmp = pos << i;
		int cover_time = 0;
		int ess;
		for (int i=1;i<=count_res;++i)
		if (mark_res[i] == 0)
		if (cover_time<=1)
		{
			if (  (tmp & value[i]) == tmp )
			{
				++cover_time;
				ess = i;
				
			}
		
		}
		
		if ( cover_time == 1 )
		{
			++mark_res[ess];
			vec[ count_chosen ] = ess;
			++count_chosen;
			--leave;
		}
	}
	if (leave > 0)
 		step2(vec,count_chosen);	
	
}
void step2(int *vec,int count_chosen)
{
	int the_sum;
	for (int i=0;i<count_chosen;i++)
	{
		the_sum = the_sum | value[ vec[i] ];
	
	}

	for (int i=1;i<=count_res;++i)
	if (mark_res[i]==0)
	{
		bool ff = true;
		for (int j=0;j<num;j++)
		if (mark_row[j])
		{
			int temp = 1 << j;

			if ((temp&value[i]==temp)  &&   (temp&the_sum)!=temp)
			{
					ff = false;
					break;
			}
			
			
		}
		if (ff)
		{
			--mark_res[i];
			--leave;
		}

	}

	//delete LIE
	
	for (int i=0;i<num;i++)
	if (mark_row[i])
	{
		int temp = 1 << i;
		if ( (temp&the_sum)==temp  )
		{
			--leave_row;
			mark_row[i] = false;
		}
	
	}
	
	delete []vec;
	if (leave==0 || leave_row == 0)
		return;
	else
		step3();

}
void step3()
{
	if (is_loop)
	{
		step5();
	}
	else
	{
		step1();
	}

}
void step4()
{
	
}

void step5()
{
	while (is_loop() && leave>0)
	{
		for (int i=1;i<=count_res;i++)
		if (mark_res[i] == 0)
		{
			++mark_res[i];
			--leave;
			break;
		}
	}
	if (leave == 0)
		return;
	else
		step1();

}


