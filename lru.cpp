//Tree based LRU implementation
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

int get_8_way_lru_position(int *lru_bits)
{
	//block0
	if((lru_bits[0]==0)&&(lru_bits[1]==0)&&(lru_bits[3]==0))
	{
		flip_bits(lru_bits,0,1,3);
		return 0;
	}

	//block1
	if((lru_bits[0]==0)&&(lru_bits[1]==0)&&(lru_bits[3]==1))
	{
		flip_bits(lru_bits,0,1,3);
		return 1;
	}

	//block2
	if((lru_bits[0]==0)&&(lru_bits[1]==1)&&(lru_bits[4]==0))
	{
		flip_bits(lru_bits,0,1,4);
		return 2;
	}	

	//block3
	if((lru_bits[0]==0)&&(lru_bits[1]==1)&&(lru_bits[4]==1))
	{
		flip_bits(lru_bits,0,1,4);
		return 3;
	}

	//block4
	if((lru_bits[0]==1)&&(lru_bits[2]==0)&&(lru_bits[5]==0))
	{
		flip_bits(lru_bits,0,2,5);
		return 4;
	}

	//block5
	if((lru_bits[0]==1)&&(lru_bits[2]==0)&&(lru_bits[5]==1))
	{
		flip_bits(lru_bits,0,2,5);
		return 5;
	}

	//block6
	if((lru_bits[0]==1)&&(lru_bits[2]==1)&&(lru_bits[6]==0))
	{
		flip_bits(lru_bits,0,2,6);
		return 6;
	}

	//block7
	if((lru_bits[0]==1)&&(lru_bits[2]==1)&&(lru_bits[6]==1))
	{
		flip_bits(lru_bits,0,2,6);
		return 7;
	}

	return -1;//Error
}

void flip_bits(int *lru_bits, int pos1, int pos2, int pos3)
{
	int x;
	if(lru_bits[pos1]==0)
	{
		lru_bits[pos1]=1;
	}
	else
	{
		lru_bits[pos1]=0;	
	}

	if(lru_bits[pos2]==0)
	{
		lru_bits[pos2]=1;
	}
	else
	{
		lru_bits[pos2]=0;	
	}

	if(lru_bits[pos3]==0)
	{
		lru_bits[pos3]=1;
	}
	else
	{
		lru_bits[pos3]=0;	
	}
}