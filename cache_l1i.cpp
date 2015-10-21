//cache.cpp
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.h"

using namespace std;
l1i_cache_set cache_l1i[64];

int l1i_fetch(unsigned long long address)
{
	get_offset(address);
	get_set_index(address);
	get_tag(address);
}

unsigned long long get_offset(unsigned long long address)
{
	unsigned long long offset;
	offset = (address & 0x3E);
	cout<<"offset : "<<hex<<offset<<endl;
	return offset;
}

unsigned long long get_set_index(unsigned long long address)
{
	//Associativity = 8
	unsigned long long temp,set_index;
	temp = (address & 0x7EC0);
	set_index = temp >> 6;
	cout<<"set_index : "<<hex<<set_index<<endl;
	return set_index;
}

unsigned long long get_tag(unsigned long long address)
{
	unsigned long long tag;
	tag = (address >> 15);
	cout<<"tag : "<<hex<<tag<<endl;
	return tag;
}