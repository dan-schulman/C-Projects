#include <iostream>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

string int2str(int x);

class GroceryList
{
private:
	string* list;
	int size;
public:
	GroceryList();
	void add(string item);
	string* checkout(int& size);
	~GroceryList();
};

GroceryList::GroceryList()
{
	list = new string[201];
	for (int i=0; i<201; i++)
	{
		list[i]="";
	}
}

GroceryList::~GroceryList()
{
	delete [] list;
}

void GroceryList::add(string item)
{
	for (int i=0; i<201; i++)
	{
		if (list[i].find(item)>=0 && list[i].find(item)<list[i].length())
		{
			string h=list[i].substr(list[i].find("x ")+2);
			
			if (h==item)
			{
				string r=list[i].substr(0,list[i].find("x "));
				int number;
				if(r.length()==3)
				{
					number=(r[0]-48)*100+(r[1]-48)*10+(r[2]-48);
				}
				else if(r.length()==2)
				{
					number=(r[0]-48)*10+(r[1]-48);
				}
				else if(r.length()==1)
				{
					number=r[0]-48;
				}
				list[i]="";
				list[i]+=int2str(number+1);
				list[i]+="x ";
				list[i]+=item;
				break;
			}
		}
		else if (i==200)
		{
			int j=0;
			while(list[j]!="")
			{
				j++;
			}
			list[j]="1x ";
			list[j]+=item;
			break;
		}
	}
}

string* GroceryList::checkout(int& size)
{
	int sum=0;
	int i=0;
	while(list[i]!="")
	{
		string r=list[i].substr(0,list[i].find("x "));
		int number;
		if(r.length()==3)
		{
			number=(r[0]-48)*100+(r[1]-48)*10+(r[2]-48);
		}
		else if(r.length()==2)
		{
			number=(r[0]-48)*10+(r[1]-48);
		}
		else if(r.length()==1)
		{
			number=r[0]-48;
		}
		sum+=number;
		i++;
	}
	
	list[i]=int2str(sum);
	size=i+1;
	string* newList;
	newList= new string [i+1];
	for (int j=0; j<i+1; j++)
	{
		newList[j]=list[j];
	}

	return newList;
}

int main()
{
	GroceryList gl;
	
	string item;
	
	getline(cin, item);
	while(item != "!checkout")
	{
		gl.add(item);
		getline(cin, item);		
	}
	
	int size;
	string* receipt = gl.checkout(size); // this should initialize size
	
	cout << "\n\nReceipt:\n";
	int maxLen = 0;
	for(int i=0; i < size-1; i++) // list of items, but not total (last in array)
	{
		cout << receipt[i] << endl;		
		if(static_cast<int>(receipt[i].length()) > maxLen)
		{
			maxLen = receipt[i].length();
		}
	}
	
	for(int i=0; i < maxLen; i++)
	{
		cout << "-";
	}
	cout << endl;
	
	cout << "Total items: "<<receipt[size-1] << endl; // total number of items
	
	delete [] receipt;

	return 0;
}

string int2str(int x)
{
	string reverse = "";
	int pos = abs(x);
	while(pos > 0)
	{
		reverse += ('0'+pos%10);
		pos/=10;
	}
	
	if(reverse.length() == 0)
	{
		return "0";
	}
	
	string result = "";
	
	if(x < 0)
	{
		result += '-';
	}
	
	for(int i=0; i < static_cast<int>(reverse.length()); i++)
	{
		result += reverse[reverse.length()-1-i];
	}
	
	return result;
}
