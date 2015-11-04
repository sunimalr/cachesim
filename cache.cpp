#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"

using namespace std;

unsigned long long get_offset(unsigned long long address)
{
	unsigned long long offset;
	offset = (address & 0x3E);
	cout<<"offset : "<<hex<<offset<<endl;
	return offset;
}

unsigned long long get_set_index(unsigned long long address)
{
	//64 lines long cache
	unsigned long long temp,set_index;
	temp = address >> 6;
	set_index = (temp & 0x3E);
	
	cout<<"set_index : "<<hex<<set_index<<endl;
	return set_index;
}

unsigned long long get_tag(unsigned long long address)
{
	unsigned long long tag;
	tag = (address >> 12);
	cout<<"tag : "<<hex<<tag<<endl;
	return tag;
}

unsigned long long get_l2_set_index(unsigned long long address)
{
	//64 lines long cache
	unsigned long long temp,set_index;
	temp = address >> 6;
	set_index = (temp & 0x1EE);
	
	cout<<"l2_set_index : "<<hex<<set_index<<endl;
	return set_index;
}

unsigned long long get_l2_tag(unsigned long long address)
{
	unsigned long long tag;
	tag = (address >> 15);
	cout<<"l2_tag : "<<hex<<tag<<endl;
	return tag;
}

unsigned long long get_victim_cache_tag(unsigned long long address)
{
	unsigned long long tag;
	tag = (address >> 6);
	cout<<"victim cache tag : "<<hex<<tag<<endl;
	return tag;
}