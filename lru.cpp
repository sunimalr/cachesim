//Tree based LRU implementation
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

int get_tree_lru_position(int way, int *lru_bits)
{
	int prev=-1;
	int curr=0;
	int lru_bits_prev;
	
	
	while((curr<(way-1)))
	{

		
		if(lru_bits[curr]==0)//go left
		{
			lru_bits_prev=lru_bits[curr];
			lru_bits[curr]=1;
			prev=curr;
			curr=(curr*2)+1;
		}
		else//go right
		{

			lru_bits_prev=lru_bits[curr];
			lru_bits[curr]=0;
			prev=curr;
			curr=(curr*2)+2;
		}
	}
	
	int line;
	if(lru_bits_prev==0)
	{
		line = (prev*2+2)-way;
	}
	else
	{
		line = (prev*2+3)-way;	
	}
	
	return line;
}

void flip_bits_lru(int way, int *lru_bits, int line)
{
	int prev=-1;
	int curr=0;
	int left=0;
	int right=way;
	int mid;
	while((curr<(way-1)))
	{
		if(d)
		cout<<"LINE : "<<line<<" BITFLIP : "<<curr<<endl;
		mid=(left+right)/2;
		if(line>=mid)//go right
		{
			left=mid;
			lru_bits[curr]=0;
			prev=curr;
			curr=(curr*2)+2;
		}
		else//go left
		{
			right=mid;
			lru_bits[curr]=1;
			prev=curr;
			curr=(curr*2)+1;
		}
	}
}


int get_8_way_bit_lru_position(cache_line_block *cache_8_block)
{
	int x;
	for(x=0;x<8;x++)
	{
		if(cache_8_block[x].mru_bit==0)
		{
			return x;
		}
	}
	return -1;//Error
}

void flip_if_all_mru_bits_are_1(cache_line_block *cb)
{
	if((cb[0].mru_bit&cb[1].mru_bit&cb[2].mru_bit&cb[3].mru_bit&cb[4].mru_bit&cb[5].mru_bit&cb[6].mru_bit&cb[7].mru_bit)==1)
	{
		cb[0].mru_bit=0;
		cb[1].mru_bit=0;
		cb[2].mru_bit=0;
		cb[3].mru_bit=0;
		cb[4].mru_bit=0;
		cb[5].mru_bit=0;
		cb[6].mru_bit=0;
		cb[7].mru_bit=0;
	}

}
