//stride prefetcher
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"
#include <map>

using namespace std;

struct table_entry{
	unsigned long long previous_address;
	int stride;
	int status;
	int mru_bit;
};

typedef struct table_entry table_entry;

 map<unsigned long long,table_entry> table;
 map<unsigned long long,table_entry>::iterator it;

unsigned long long get_prefetch_address(unsigned long long pc, unsigned long long add)
{
	table_entry t;
	it = table.find(pc);
  	if (it != table.end())//No entry in table
  	{
  		
  		t.previous_address=add;
  		t.stride=0;
  		t.status=INIT;
  		table[pc]=t;
  		return -1;
  	}
  	else
  	{
  		t=it->second;
  		
  		{
  			int new_stride=add-t.previous_address;
  			t.previous_address=add;
  			if((new_stride!=t.stride)&&(t.status==INIT))
  			{
  				t.stride=new_stride;
  				t.status=TRANSIENT;
  				t.mru_bit=1;
  				table[pc]=t;
  				return (add+new_stride);
  			}
  			if((new_stride!=t.stride)&&(t.status==TRANSIENT))
  			{
  				t.stride=new_stride;
  				t.status=NOPRED;
  				t.mru_bit=1;
  				table[pc]=t;
  				return -1;
  			}
  			if((new_stride!=t.stride)&&(t.status==NOPRED))
  			{
  				t.stride=new_stride;
  				t.status=NOPRED;
  				t.mru_bit=1;
  				table[pc]=t;
  				return -1;
  			}
  			if((new_stride!=t.stride)&&(t.status==STEADY))
  			{
  				t.status=INIT;
  				t.mru_bit=1;
  				table[pc]=t;
  				return (add+new_stride);
  			}
  			if((new_stride==t.stride)&&(t.status==INIT))
  			{
  				t.stride=new_stride;
  				t.status=TRANSIENT;
  				t.mru_bit=1;
  				table[pc]=t;
  				return (add+new_stride);
  			}
  			if((new_stride==t.stride)&&(t.status==TRANSIENT))
  			{
  				t.stride=new_stride;
  				t.status=STEADY;
  				t.mru_bit=1;
  				table[pc]=t;
  				return (add+new_stride);
  			}
  			if((new_stride==t.stride)&&(t.status==STEADY))
  			{
  				t.stride=new_stride;
  				t.status=STEADY;
  				t.mru_bit=1;
  				table[pc]=t;
  				return (add+new_stride);
  			}
  			if((new_stride==t.stride)&&(t.status==NOPRED))
  			{
  				t.stride=new_stride;
  				t.status=TRANSIENT;
  				t.mru_bit=1;
  				table[pc]=t;
  				return (add+new_stride);
  			}
  		}
  		
  	}
    

}