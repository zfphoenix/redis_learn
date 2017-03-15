//
#include <iostream>
#include <list>
#include <time.h>
#include <stdlib.h>

struct Comp
{
	bool operator()(int a, int b)
	{
		return a > b;
	}
};

std::list<int> ls;
typedef std::list<int>::iterator LItr;

void insert(int a)
{
	Comp comp;
	for(LItr itr = ls.begin(); itr != ls.end(); ++itr)
	{
		if (*itr == a)
		{
			ls.erase(itr);
			break;
		}
	}
	if (ls.empty() || comp(a, ls.front()))
	{
		ls.push_front(a);
	} 
	else if (comp(ls.back(), a))
	{
		ls.push_back(a);
	}
	else
	{
		LItr itr = ls.begin();
		for ( ;itr != ls.end(); ++itr)
		{
			if(comp(a, *itr))	
			{
				ls.insert(itr, a);
				break;
			}
		}
		if (itr == ls.end())
		{
			ls.push_back(a);
		}
	}
	if (ls.size() > 10)
	{
		ls.pop_back();
	}

}

int GenRand()
{
	return rand()%1000;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int i = 0;
	while (i < 10000)
	{
		insert(GenRand());
		++i;
	}
	for (LItr itr = ls.begin(); itr != ls.end(); ++itr)
	{
		std::cout << *itr << std::endl;
	}
	return 0;
}

