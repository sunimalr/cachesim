//L1-i cache
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

l1i_cache_set cache_l1i[64];
int lru_bits_l1i[7];

//Returns 1 if hit, 0 if miss);
int l1i_check(unsigned long long address)
{
	unsigned long long offset = get_offset(address);
	unsigned long long set_index = get_set_index(address);
	unsigned long long tag = get_tag(address);

	//Valid=1 invalid=0 for each way
	int isInvalid=-1;
	int y;

	//Check all sets
	for(y=0;y<8;y++)
	{
		if((cache_l1i[set_index].set[y]).valid==1)
		{
			
			unsigned long long cache_tag = (cache_l1i[set_index].set[y]).tag;
			if(tag == cache_tag)
			{
				//HIT
				cout << "L1-i HIT" << endl; 
				return 1;
			}
			else
			{	
				cout << "Tag mismatch" << endl; 
				//return 0;	
			}
		}
		else
		{
			//Keeps track of an invalid block
			isInvalid=y;
		}
	}
	cout << "L1-i MISS" << endl;
	//get from L2 cache
	//select which way to put data

	if(isInvalid != -1)//If there is an invalid block replace it
	{
		(cache_l1i[set_index].set[isInvalid]).valid=1;
		(cache_l1i[set_index].set[isInvalid]).tag=tag;
	}
	else//else use cache replacement policy and find out which block to replace
	{
		int way=get_8_way_lru_position(lru_bits_l1i);
		(cache_l1i[set_index].set[way]).valid=1;
		(cache_l1i[set_index].set[way]).tag=tag;
		cout<<"replaced with LRU"<<endl;
	}
	//put data

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

	for (x = 0; x < 7; x++)
	{
		lru_bits_l1i[x]=0;
	}
}
