//cache.cpp
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

l1i_cache_set cache_l1d[64];

//Returns 1 if hit, 0 if miss);
int l1i_check(unsigned long long address)
{
	unsigned long long offset = get_offset(address);
	unsigned long long set_index = get_set_index(address);
	unsigned long long tag = get_tag(address);

	//Valid=1 invalid=0 for each way
	int isValid[8];
	int y;
	for(y=0;y<8;y++)
	{
		if((cache_l1i[set_index].set[y]).valid==1)
		{
			isValid[y]=1;
			unsigned long long cache_tag = (cache_l1i[set_index].set[y]).tag;
			if(tag == cache_tag)
			{
				//HIT
				cout << "L1-d HIT" << endl; 
				return 1;
			}
			else
			{	
				cout << "L1-d MISS" << endl; 
				return 0;	
			}
		}
	}
	cout << "L1-d MISS" << endl;
	return 0;
}

void init_l1i_cache()
{
	int x;
	for(x=0;x<64;x++)
	{
		(cache_l1i[x].set[0]).valid=0;
		(cache_l1i[x].set[1]).valid=0;
		(cache_l1i[x].set[2]).valid=0;
		(cache_l1i[x].set[3]).valid=0;
		(cache_l1i[x].set[4]).valid=0;
		(cache_l1i[x].set[5]).valid=0;
		(cache_l1i[x].set[6]).valid=0;
		(cache_l1i[x].set[7]).valid=0;
	}
}
