#include"INT.h"
INT INT::operator +(INT a) const
{		
	int lt = max(len,a.len);
	int num = lt;
	int carry  = 0;
	int res[1000];
	for (int i=0;i<lt;i++)
	{
		carry = (a.opt[i]+opt[i]+carry)/10;
		res[i] = (a.opt[i]+opt[i]+carry)%10;		
	}

	if (carry>0)
	{
		res[lt] = carry;
		++lt;
	}
	}
	return INT(res,lt);
}
		
INT INT::operator -(INT a)
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
		while (res[lt-1] == 0)
		{
			--lt;
		}
		
		return INT(res,lt,true);

	}	
	else
	{
		return a-*this;			
	}
	
}

INT INT::operator *(INT a);

INT INT::operator /(INT a);

INT INT::operator +(int a);

INT INT::operator -(int a);

INT INT::operator *(int a);

INT INT::operator /(int a);


