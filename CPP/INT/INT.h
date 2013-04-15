// OOP practice
// INT class:
// 	Big number calculation

#include<cstring>
const int NUM = 1000;

class INT
{
	int opt[NUM];
	int len;
	bool positive;
	public:
		INT()
		{
			memset( opt,0,sizeof(opt) );
			len = 1;
			positive = true;
		}

		INT(string str)
		{
		    	memset(opt,0,sizeof(opt));	
			len = str.length();
			if (str[0]!='-')
			{
				positive = true;
				for (int i=0;i<len;i++)
				{
					opt[i] = str[len-i-1] - '0';

				}
			}
			else
			{
				positive = false;
				for (int i=1;i<len;i++)
				{
					opt[i-1] = str[len-i] - '0';
				}
				len = len-1;
			}
			
		}

		INT(int n)
		{
			memset(opt,0,sizeof(opt));
			if (n>0)
				positive = true;
			else
				{
					positive = false;
					n = -n;
				}
			len = 0;
			while (n!=0)
			{
				opt[len] = n%10;
				n = n/10;
				++len;
			}
			
		}
		
		INT(int a[],int lt)
		{
			memset(opt,0,sizeof(opt));
			positive = true;
			for (int i=0;i<lt;i++)
				opt[i] = a[i];
			len = lt;	
		}

		INT(int a[],int lt,bool mark)
		{
 			memset(opt,0,sizeof(opt));
			positive = mark;

			for (int i=0;i<lt;i++)
			opt[i] = a[i];
			len = lt;	
		
		}

		INT(INT const& tmp)
		{
			memset(opt,0,sizeof(opt));
			positive = tmp.positive;
			len = tmp.len;
			for (int i=0;i<len;i++)
			{
				opt[i] = tmp.opt[i];
			}
		}
	
		INT(INT const& tmp,bool mark)
		{
			memset(opt,0,sizeof(opt));
			positive = mark;
			len = tmp.len;
			for (int i=0;i<len;i++)
			{
				opt[i] = tmp.opt[i];
			}
		}

		void display()
		{
			if (len == 0) 
			{
				cout << '0'<< endl;
				return;
			}
			if (positive == false)
				cout << '-';
			for (int i=len-1;i>=0;i--)
			{
				cout << opt[i];
			}
			cout << endl;
		}

		bool operator >=(const INT& a)const
		{
			if (len>a.len)
				return true;
			else
			if (len<a.len)
				return false;
			else
			{
				for (int i=len-1;i>=0;i--)
				if (opt[i]<a.opt[i])
				{
					return false;
				}
				else
				if(opt[i]>a.opt[i])
					return true;
	
				return true;
			}	
				
		}

		INT& operator =(const INT& a)
		{
			memset(opt,0,sizeof(opt));
			len = a.len;
			positive = a.positive;
			for (int i=0;i<len;i++)
				opt[i] = a.opt[i];
			return *this;
		}

		INT operator +(INT a)const;
		
		INT operator -(INT a)const;

		INT operator *(INT a)const;

		INT operator /(INT a)const;

		INT operator +(int a)const;

		INT operator -(int a)const;

		INT operator *(int a)const;

		INT operator /(int a)const;

		friend INT operator +(int n,INT a);

		friend INT operator -(int n,INT a);

		friend INT operator *(int n,INT a);

		friend INT operator /(int n,INT a);
};

INT operator +(int n,INT a)
{
	
	return a*n;	
}

INT operator -(int n,INT a) 
{
	INT tmp(n);
	return tmp-a;
}	

INT operator *(int n,INT a) 
{
	INT tmp(n);
	return tmp-a;
}	

INT operator /(int n,INT a) 
{
	INT tmp(n);
	return tmp/a;
}
INT INT::operator +(INT a) const
{		
	if (a.positive && positive)
	{
		int lt = max(len,a.len);
		int num = lt;
		int carry  = 0;
		int res[1000];
		for (int i=0;i<lt;i++)
		{
			res[i] = (a.opt[i]+opt[i]+carry)%10;
			carry = (a.opt[i]+opt[i]+carry)/10;
		
		}

		if (carry>0)
		{
			res[lt] = carry;
			++lt;
		}

		return INT(res,lt);
	}
	else
	if (positive && a.positive==false)
	{
		INT tmp = INT(a,true);
		return *this-tmp;
	}
	else
	if (!positive && a.positive)
	{
		INT tmp = INT(*this,true);
		return a-tmp;
	}
	else
	if (!positive && !a.positive)
	{
		INT tmp1 = INT(a,true);
		INT tmp2 = INT(*this,true);
		return INT(tmp1+tmp2,false);
	}
}
		
INT INT::operator -(INT a)const
{
	if (a.positive == false)
	{
		if (positive)
		{
			INT tmp1 = INT(a,true);
			return *this+tmp1;
		}
		else
		{
			INT tmp1 = INT(a,true);
			INT tmp2 = INT(*this,true);
			return tmp1 - tmp2;
		}
	}
	
	if (positive == false)
	{
		INT tmp1 = INT(*this,true);
		return INT(tmp1+a,false);
	}
	
	int res[1000];

	if (*this >= a)
	{
		
		int carry = 0;
		for (int i=0;i<len;i++)
		{
			if (opt[i]-carry >= a.opt[i])
			{
				res[i] = opt[i] - a.opt[i] - carry;
				carry = 0;
			}
			else
			{
				res[i] = opt[i] - a.opt[i] - carry +10;
				carry = 1;
			}
		}
		int lt = len;
		while (res[lt-1] == 0 && lt>0)
		{
			--lt;
		}
		
		return INT(res,lt,true);

	}	
	else
	{
		return INT(a-*this,false);			
	}
	
}

INT INT::operator *(INT a) const
{
	INT tmp1(*this,true);
	INT tmp2(a,true);
	int res[NUM];
	memset(res,0,sizeof(res));
	for (int i=0;i<len;i++)
		for (int j=0;j<a.len;j++)
		{
			res[i+j] = res[i+j] + opt[i]*a.opt[j];
		}
	int carry = 0;
	int lt = len+a.len-1;
	for (int i=0;i<lt;i++)
	{
		int t = carry;
		carry = (res[i]+carry) / 10;
		res[i] =( t + res[i])%10;

	}
	while (carry>0)
	{
		res[lt] = carry % 10;
		carry = carry / 10;
		++lt;
	}
	if (positive && a.positive || !positive && !a.positive)
		return INT(res,lt,true);
	else
		return INT(res,lt,false);

}

INT INT::operator /(INT a) const
{
	if (positive && a.positive)
	{
		INT tmp1 = INT(*this,true);
		INT tmp2 = INT(a,true);
	
		if (tmp1>=tmp2)
		{
			bool mark = true;
			INT zero = INT(0);
			int res=0;
			while (tmp1.positive)
			{
				INT tmp = tmp1-tmp2;
				tmp1 = tmp;
	
				++res;
				
			}
			return INT(res-1);
		}
		else
		{
			return INT(0);
		}
	}
	else
	if (positive && !a.positive)
	{
		INT tmp(a,true);
		return INT(*this/tmp,false);
	}
	else
	if(a.positive)
	{
		INT tmp(*this,true);
		return INT(tmp/a,false);
	}
	else
	if (!a.positive)
	{
		INT tmp1(*this,true);
		INT tmp2(a,true);
		return tmp1/tmp2;
	}
}

INT INT::operator +(int a) const
{
	INT tmp(a);
	INT tmp_this = *this;
	return tmp_this+tmp; 
}

INT INT::operator -(int a) const
{
	INT tmp(a);
	INT tmp_this = *this;	
	return tmp_this-tmp;
}

INT INT::operator *(int a) const
{
	INT tmp(a);
	INT tmp_this = *this;
	return tmp_this*tmp;
}

INT INT::operator /(int a) const
{
	INT tmp(a);
	INT tmp_this = *this;
	return tmp_this/tmp;
}


