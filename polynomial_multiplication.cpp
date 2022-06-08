#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

class poly{
private:
	int* coef;
	int length;
public:
	poly();
	poly(int* c, int l);
	~poly();
	poly operator=(const poly& p);
	poly(const poly& p);
	friend istream& operator>>(istream& in, poly& p);	
	friend ostream& operator<<(ostream& out, poly p);	
	poly operator*(poly p);
};

int main()
{
	poly p1, p2;
	cout << "What are the coefficients of first polynomial (lowest power first)? ";
	cin >> p1;

	cout << "What are the coefficients of second polynomial (lowest power first)? ";
	cin >> p2;
	
	cout << p1 << endl; 
	cout << "... times... " << endl;
	cout << p2 << endl; 
	cout << "------------\n";
	
	poly p3 = p1 * p2; 
	cout << p3 << endl; 
	
	return 0;
}

poly::poly()
{
	coef = new int[1];
	coef[0] = 0;
	length = 1;
}

poly::poly(int* c, int l)
{
	length = l;
	coef = new int[l];
	for(int i=0; i < l; i++)
	{
		coef[i] = c[i];
	}
}
poly poly::operator=(const poly& p)
{
	length = p.length;
	delete [] coef;
	coef = new int[length];
	for(int i=0; i < length; i++)
	{
		coef[i] = p.coef[i];
	}
	
	poly result(p.coef, p.length);
	return result;
}

poly::poly(const poly& p)
{
	coef = new int[p.length];
	//*this = p; // this line was causing errors for some people
	length = p.length;
	for(int i=0; i < length; i++)
	{
		coef[i] = p.coef[i];
	}
}

poly::~poly()
{
	delete [] coef;
}

istream& operator>>(istream& in, poly& p)	
{
	string s;
	getline(in, s);
	
	int numberCount = 0;
	bool onNumeric = false;
	
	// first loop counts amount of numbers
	for(int i=0; i < static_cast<int>(s.length()); i++)
	{
		if(isdigit(s[i]) && !onNumeric)
		{
			numberCount++;
			onNumeric=true;
		}
		if(!isdigit(s[i]) && onNumeric)
		{
			onNumeric=false;
		}
	}
	
	// delete old poly & replace with correct size
	delete [] p.coef;
	p.length = numberCount;

	if(numberCount == 0) // case if nothing entered (don't want create 0 length array...)
	{
		p.coef = new int[1];
		p.coef[0] = 0;
		return in;
	}
	
	p.coef = new int[numberCount];
	
	// loop through again to fill array (similar logic to last loop)
	onNumeric = false;
	int previousIndex = 0; // used to find negative signs
	int lastIndex=0;
	int coefIndex=0;
	for(int i=0; i < static_cast<int>(s.length()); i++)
	{
		if(isdigit(s[i]) && !onNumeric)
		{
			previousIndex=lastIndex;
			lastIndex=i;
			onNumeric = true;
		}
		if(!isdigit(s[i]) && onNumeric)
		{
			string currentNumber = s.substr(lastIndex, i-lastIndex);
			p.coef[coefIndex] = atoi(currentNumber.c_str());
			
			string before = s.substr(previousIndex, lastIndex-previousIndex);
			if(static_cast<int>(before.find('-')) != -1)
			{
				p.coef[coefIndex] *= -1;
			}
			
			coefIndex++;
			onNumeric = false;
		}
	}
	
	// case if last character is part of the digit
	if(onNumeric)
	{
		string currentNumber = s.substr(lastIndex);
		p.coef[coefIndex] = atoi(currentNumber.c_str());
		
		string before = s.substr(previousIndex, lastIndex-previousIndex);
		if(static_cast<int>(before.find('-')) != -1)
		{
			p.coef[coefIndex] *= -1;
		}

	}	
	
	return in;
}

ostream& operator<<(ostream& out, poly p)
{
	int l = p.length;
	for(int i=0; i < l; i++)
	{
		if (i==0)
		{
			if (l==1)
			{
				out<<"("<<p.coef[i]<<")";
			}
			else
			{
				out<<"("<<p.coef[i]<<")"<<" + ";
			}
		}
		else if (i==l-1)
		{
			out<<"("<<p.coef[i]<<")"<<"x^"<<i;
		}
		else
		{
			out<<"("<<p.coef[i]<<")"<<"x^"<<i<<" + ";
		}
	}
	return out;
}

poly poly::operator*(poly p)
{
	poly p3;
	p3.length=(length)+(p.length)-1;
	delete [] p3.coef;
	
	p3.coef = new int[p3.length];
	for (int g=0; g<p3.length; g++)
	{
		p3.coef[g]=0;
	}
	for (int i=0; i<length; i++)
	{
		for (int j=0; j<p.length; j++)
		{
			p3.coef[(i+j)]+=coef[i]*p.coef[j];
			//cout<<"index:"<<i+j<<endl;
			//cout<<"coef: "<<p3.coef[(i+j)]<<endl;
		}
		//cout<<"inside loop"<<endl;
	}
	//cout<<"leaving loop"<<endl;
	return p3;
}

