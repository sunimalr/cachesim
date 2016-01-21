//Victim cache
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

cache_set cache_victim[1];
int lru_bits_victim[7];

//Returns 1 if hit, 0 if miss);
int victim_cache_check(unsigned long long address, int type)
{
	unsigned long long offset = get_offset(address);
	unsigned long long set_index = 0;
	unsigned long long tag = get_victim_cache_tag(address);

	//Valid=1 invalid=0 for each way
	int isInvalid=-1;
	int y;


	//Check all blocks
	for(y=0;y<8;y++)
	{
		if((cache_victim[set_index].set[y]).valid==1)
		{
			
			unsigned long long cache_tag = (cache_victim[set_index].set[y]).tag;
			if(tag == cache_tag)
			{
				
					//Read hit 
				#ifdef BPLRU
					(cache_victim[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_victim[set_index].set);
				#endif

				if(type==LOAD)
				{
					//Read hit
					if(d)
					cout << "VICTIM READ HIT" << endl;
					ct_victim_hit++;
					return 1;
				}
				if(type==STORE)
				{
					//write hit
					if(d)
					cout << "VICTIM WRITE HIT" << endl;
					ct_victim_hit++;
					return 1;
				}

				flip_bits_lru(8, lru_bits_victim, y);
				
				#ifdef BPLRU
					(cache_victim[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_victim[set_index].set);
				#endif

				
				return 1;
			}
			else
			{	
				if(d)
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
	ct_victim_miss++;
	if (type==STORE)
	{
		//Load from memory. Update the cache. Update memory.
		if(d)
		cout << "VICTIM WRITE MISS" << endl;
	}
	else if((type==LOAD)||(type==FETCH))
	{
		if(d)
		cout << "VICTIM READ MISS" << endl;	
		
	}
	
	

	return 0;
}

void write_to_victim_cache(unsigned long long address)
{
	int set_index=0;
	int y;
	unsigned long long tag = get_victim_cache_tag(address);
	for(y=0;y<8;y++)
	{
		if((cache_victim[set_index].set[y]).valid==0)//If there is an invalid block replace it
		{
			(cache_victim[set_index].set[y]).valid=1;
			(cache_victim[set_index].set[y]).tag=tag;
			return;
		}
	}
		//else use cache replacement policy and find out which block to replace
		{
			int way;
			#ifdef BPLRU
				way=get_8_way_bit_lru_position(cache_victim[set_index].set);
				//write cache_victim[set_index].set[way]) data to L2 cache. 
				if((cache_victim[set_index].set[way]).dirty_bit==1)
				{
					int addr=(cache_victim[set_index].set[way]).tag;
					addr=addr << 6;
					l2_write(addr);
				}	 
				(cache_victim[set_index].set[way]).valid=1;
				(cache_victim[set_index].set[way]).tag=tag;
				if(d)
				cout<<"VICTIM: replaced with BIT PLRU"<<endl;
			#else 
				way=get_tree_lru_position(8,lru_bits_victim);
				//write cache_victim[set_index].set[way]) data to L2 cache.
				if((cache_victim[set_index].set[way]).dirty_bit==1)
				{
					int addr=(cache_victim[set_index].set[way]).tag;
					addr=addr << 6;
					l2_write(addr);
				}
				(cache_victim[set_index].set[way]).valid=1;
				(cache_victim[set_index].set[way]).tag=tag;
				if(d)
				cout<<"VICTIM: replaced with TREE PLRU"<<endl;
			#endif		
		}
		return;
}

void init_victim_cache()
{
	int x=0;
	
		(cache_victim[x].set[0]).valid=0;
		(cache_victim[x].set[1]).valid=0;
		(cache_victim[x].set[2]).valid=0;
		(cache_victim[x].set[3]).valid=0;
		(cache_victim[x].set[4]).valid=0;
		(cache_victim[x].set[5]).valid=0;
		(cache_victim[x].set[6]).valid=0;
		(cache_victim[x].set[7]).valid=0;
		
		(cache_victim[x].set[0]).mru_bit=0;
		(cache_victim[x].set[1]).mru_bit=0;
		(cache_victim[x].set[2]).mru_bit=0;
		(cache_victim[x].set[3]).mru_bit=0;
		(cache_victim[x].set[4]).mru_bit=0;
		(cache_victim[x].set[5]).mru_bit=0;
		(cache_victim[x].set[6]).mru_bit=0;
		(cache_victim[x].set[7]).mru_bit=0;
	

	for(x = 0; x < 7; x++)
	{
		lru_bits_victim[x]=0;
	}
}
