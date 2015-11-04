//L1-d cache
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

cache_set cache_l1d[64];
int lru_bits_l1d[7];

//Returns 1 if hit, 0 if miss);
int l1d_check(unsigned long long address, int type)
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
		if((cache_l1d[set_index].set[y]).valid==1)
		{
			
			unsigned long long cache_tag = (cache_l1d[set_index].set[y]).tag;
			if(tag == cache_tag)
			{
				
					//Read hit 
				#ifdef BPLRU
					(cache_l1d[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_l1d[set_index].set);
				#endif

				if(type==LOAD)
				{
					//Read hit
					cout << "L1-D READ HIT" << endl;
					return 1;
				}
				if(type==STORE)
				{
					//write hit
					cout << "L1-D WRITE HIT" << endl;
				}

				switch(y)
				{
					case 0 : 
						flip_bits(lru_bits_l1d,0,1,3);
						break;
					case 1 : 
						flip_bits(lru_bits_l1d,0,1,3);
						break;
					case 2 : 
						flip_bits(lru_bits_l1d,0,1,4);
						break;
					case 3 : 
						flip_bits(lru_bits_l1d,0,1,4);
						break;
					case 4 : 
						flip_bits(lru_bits_l1d,0,2,5);
						break;
					case 5 : 
						flip_bits(lru_bits_l1d,0,2,5);
						break;
					case 6 : 
						flip_bits(lru_bits_l1d,0,2,6);
						break;
					case 7 : 
						flip_bits(lru_bits_l1d,0,2,6);
						break;
				}
				
				#ifdef BPLRU
					(cache_l1d[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_l1d[set_index].set);
				#endif

				
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
	
	if (type==STORE)
	{
		//Load from memory. Update the cache. Update memory.
		cout << "L1-D WRITE MISS" << endl;
	}
	else if((type==LOAD)||(type==FETCH))
	{
		cout << "L1-D READ MISS" << endl;	
	}
	//This is a write back cache. And also write allocate.
	//get from L2 cache : TODO
	l2_check(address, type);

	//select which way to put data
	if(isInvalid != -1)//If there is an invalid block replace it
	{
		(cache_l1d[set_index].set[isInvalid]).valid=1;
		(cache_l1d[set_index].set[isInvalid]).tag=tag;
	}
	else//else use cache replacement policy and find out which block to replace
	{
		int way;
		#ifdef BPLRU
			way=get_8_way_bit_lru_position(cache_l1d[set_index].set);
			//TODO: write cache_l1d[set_index].set[way]) data to L2 cache. 
			(cache_l1d[set_index].set[way]).valid=1;
			(cache_l1d[set_index].set[way]).tag=tag;
			cout<<"L1-D: replaced with BIT PLRU"<<endl;
		#else 
			way=get_8_way_tree_lru_position(lru_bits_l1d);
			//TODO: write cache_l1d[set_index].set[way]) data to L2 cache.
			(cache_l1d[set_index].set[way]).valid=1;
			(cache_l1d[set_index].set[way]).tag=tag;
			cout<<"L1-D: replaced with TREE PLRU"<<endl;
		#endif		
	}
	

	return 0;
}

void init_l1d_cache()
{
	int x;
	for(x=0;x<64;x++)
	{
		(cache_l1d[x].set[0]).valid=0;
		(cache_l1d[x].set[1]).valid=0;
		(cache_l1d[x].set[2]).valid=0;
		(cache_l1d[x].set[3]).valid=0;
		(cache_l1d[x].set[4]).valid=0;
		(cache_l1d[x].set[5]).valid=0;
		(cache_l1d[x].set[6]).valid=0;
		(cache_l1d[x].set[7]).valid=0;
		
		(cache_l1d[x].set[0]).mru_bit=0;
		(cache_l1d[x].set[1]).mru_bit=0;
		(cache_l1d[x].set[2]).mru_bit=0;
		(cache_l1d[x].set[3]).mru_bit=0;
		(cache_l1d[x].set[4]).mru_bit=0;
		(cache_l1d[x].set[5]).mru_bit=0;
		(cache_l1d[x].set[6]).mru_bit=0;
		(cache_l1d[x].set[7]).mru_bit=0;
	}

	for (x = 0; x < 7; x++)
	{
		lru_bits_l1d[x]=0;
	}
}
