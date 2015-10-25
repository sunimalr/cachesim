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
	//Associativity = 8
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