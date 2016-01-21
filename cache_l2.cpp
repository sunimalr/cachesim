//L1-d cache
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

cache_set cache_l2[512];
int lru_bits_l2[7];

//Returns 1 if hit, 0 if miss);
int l2_check(unsigned long long address, int type)
{
	check_infinity_cache_l2(address);
	unsigned long long offset = get_offset(address);
	unsigned long long set_index = get_l2_set_index(address);
	unsigned long long tag = get_l2_tag(address);

	//Valid=1 invalid=0 for each way
	int isInvalid=-1;
	int y;


	//Check all sets
	for(y=0;y<8;y++)
	{
		if((cache_l2[set_index].set[y]).valid==1)
		{
			
			unsigned long long cache_tag = (cache_l2[set_index].set[y]).tag;
			unsigned long long pref;
			if(tag == cache_tag)
			{
				if((type==LOAD)||(type==FETCH))
				{
					//Read hit
					if(d)
					cout << "L2 READ HIT" << endl;
					ct_l2_r_hit++;
					
					pref=get_prefetch_address(program_counter, address);
					if(pref!=-1)
					{
						l2_prefetch(pref);//Prefetches the given address	
					}
					
					return 1;
				}
				if(type==STORE)
				{
					//write hit
					if(d)
					cout << "L2 WRITE HIT" << endl;
					ct_l2_w_hit++;
					
					pref=get_prefetch_address(program_counter, address);
					if(pref!=-1)
					{
						l2_prefetch(pref);//Prefetches the given address	
					}
					
					
					//write to memory directly.
				}

				flip_bits_lru(8,lru_bits_l2,y);
				
				#ifdef BPLRU
					(cache_l2[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_l2[set_index].set);
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
	
	//This is a write through. And also write allocate.
	//get from memory

	if (type==STORE)
	{
		//Load from memory. Update the cache. Update memory.
		if(d)
		cout << "L2 WRITE MISS" << endl;
		ct_l2_w_miss++;
	}
	else if((type==LOAD)||(type==FETCH))
	{
		if(d)
		cout << "L2 READ MISS" << endl;
		ct_l2_r_miss++;	
	}

	//select which way to put data
	if(isInvalid != -1)//If there is an invalid block replace it
	{
		(cache_l2[set_index].set[isInvalid]).valid=1;
		(cache_l2[set_index].set[isInvalid]).tag=tag;
	}
	else//else use cache replacement policy and find out which block to replace
	{
		int way;
		#ifdef BPLRU
			way=get_8_way_bit_lru_position(cache_l2[set_index].set);
			//TODO: write cache_l1d[set_index].set[way]) data to L2 cache. 
			(cache_l2[set_index].set[way]).valid=1;
			(cache_l2[set_index].set[way]).tag=tag;
			if(d)
			cout<<"L2: replaced with BIT PLRU"<<endl;
		#else 
			way=get_tree_lru_position(8,lru_bits_l2);
			//TODO: write cache_l1d[set_index].set[way]) data to L2 cache.
			(cache_l2[set_index].set[way]).valid=1;
			(cache_l2[set_index].set[way]).tag=tag;
			if(d)
			cout<<"L2: replaced with TREE PLRU"<<endl;
		#endif		
	}
	

	return 0;
}

int l2_prefetch(unsigned long long address)
{
	unsigned long long offset = get_offset(address);
	unsigned long long set_index = get_l2_set_index(address);
	unsigned long long tag = get_l2_tag(address);

	//Valid=1 invalid=0 for each way
	int isInvalid=-1;
	int y;


	//Check all sets
	for(y=0;y<8;y++)
	{
		if((cache_l2[set_index].set[y]).valid==1)
		{
			
			unsigned long long cache_tag = (cache_l2[set_index].set[y]).tag;
			if(tag == cache_tag)
			{
				//Already in cache		
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
	
		//Load from memory. Update the cache. Update memory.
	

	//select which way to put data
	if(isInvalid != -1)//If there is an invalid block replace it
	{
		(cache_l2[set_index].set[isInvalid]).valid=1;
		(cache_l2[set_index].set[isInvalid]).tag=tag;
	}
	else//else use cache replacement policy and find out which block to replace
	{
		int way;
		#ifdef BPLRU
			way=get_8_way_bit_lru_position(cache_l2[set_index].set);
			 
			(cache_l2[set_index].set[way]).valid=1;
			(cache_l2[set_index].set[way]).tag=tag;
			if(d)
			cout<<"L2: replaced with BIT PLRU"<<endl;
		#else 
			way=get_tree_lru_position(8,lru_bits_l2);
			
			(cache_l2[set_index].set[way]).valid=1;
			(cache_l2[set_index].set[way]).tag=tag;
			if(d)
			cout<<"L2: replaced with TREE PLRU"<<endl;
		#endif		
	}
	

	return 0;
}

int l2_write(unsigned long long address)
{
	unsigned long long offset = get_offset(address);
	unsigned long long set_index = get_l2_set_index(address);
	unsigned long long tag = get_l2_tag(address);

	//Valid=1 invalid=0 for each way
	int isInvalid=-1;
	int y;


	//Check all sets
	for(y=0;y<8;y++)
	{
		if((cache_l2[set_index].set[y]).valid==1)
		{
			
			unsigned long long cache_tag = (cache_l2[set_index].set[y]).tag;
			if(tag == cache_tag)
			{ 
				#ifdef BPLRU
					(cache_l2[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_l2[set_index].set);
				#endif

				//Update data. Entry is already there.
				//Update memory becuase this is write through cache.

				flip_bits_lru(8,lru_bits_l2,y);
				
				#ifdef BPLRU
					(cache_l2[set_index].set[y]).mru_bit=1;
					flip_if_all_mru_bits_are_1(cache_l2[set_index].set);
				#endif

				
				return 1;
			}
			else
			{	
				if(d)
				cout << "Tag mismatch" << endl; 
				//Entry is not there
			}
		}
		else
		{
			//Keeps track of an invalid block
			isInvalid=y;
		}
	}
		
	//select which way to put data
	if(isInvalid != -1)//If there is an invalid block replace it
	{
		(cache_l2[set_index].set[isInvalid]).valid=1;
		(cache_l2[set_index].set[isInvalid]).tag=tag;
	}
	else//else use cache replacement policy and find out which block to replace
	{
		int way;
		#ifdef BPLRU
			way=get_8_way_bit_lru_position(cache_l2[set_index].set);
			//TODO: write cache_l1d[set_index].set[way]) data to L2 cache. 
			(cache_l2[set_index].set[way]).valid=1;
			(cache_l2[set_index].set[way]).tag=tag;
			if(d)
			cout<<"L2: replaced with BIT PLRU"<<endl;
		#else 
			way=get_tree_lru_position(8,lru_bits_l2);
			//TODO: write cache_l1d[set_index].set[way]) data to L2 cache.
			(cache_l2[set_index].set[way]).valid=1;
			(cache_l2[set_index].set[way]).tag=tag;
			if(d)
			cout<<"L2: replaced with TREE PLRU"<<endl;
		#endif		
	}
	
	return 0;
}

void init_l2_cache()
{
	int x;
	for(x=0;x<512;x++)
	{
		(cache_l2[x].set[0]).valid=0;
		(cache_l2[x].set[1]).valid=0;
		(cache_l2[x].set[2]).valid=0;
		(cache_l2[x].set[3]).valid=0;
		(cache_l2[x].set[4]).valid=0;
		(cache_l2[x].set[5]).valid=0;
		(cache_l2[x].set[6]).valid=0;
		(cache_l2[x].set[7]).valid=0;
		
		(cache_l2[x].set[0]).mru_bit=0;
		(cache_l2[x].set[1]).mru_bit=0;
		(cache_l2[x].set[2]).mru_bit=0;
		(cache_l2[x].set[3]).mru_bit=0;
		(cache_l2[x].set[4]).mru_bit=0;
		(cache_l2[x].set[5]).mru_bit=0;
		(cache_l2[x].set[6]).mru_bit=0;
		(cache_l2[x].set[7]).mru_bit=0;
	}

	for (x = 0; x < 7; x++)
	{
		lru_bits_l2[x]=0;
	}
}
