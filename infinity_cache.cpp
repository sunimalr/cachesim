#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "cache.hpp"
#include <map>

using namespace std;

//int ct_infinity_hit_l1i=0;
int ct_infinity_miss_l1i=0;
int ct_infinity_miss_l1d=0;
int ct_infinity_miss_l2=0;

map<unsigned long long,bool> infinity_cache_l1i;
map<unsigned long long,bool>::iterator itr_l1i;

map<unsigned long long,bool> infinity_cache_l1d;
map<unsigned long long,bool>::iterator itr_l1d;

map<unsigned long long,bool> infinity_cache_l2;
map<unsigned long long,bool>::iterator itr_l2;

void check_infinity_cache_l1i(unsigned long long address)
{

	itr_l1i = infinity_cache_l1i.find(address);
  	if (itr_l1i != infinity_cache_l1i.end())//No entry in table
  	{
  		//Hit
  	}
  	else
  	{
  		ct_infinity_miss_l1i++;
  		infinity_cache_l1i[address]=true;
  	}	

}

void check_infinity_cache_l1d(unsigned long long address)
{

	itr_l1d = infinity_cache_l1d.find(address);
  	if (itr_l1d != infinity_cache_l1d.end())//No entry in table
  	{
  		//Hit
  	}
  	else
  	{
  		ct_infinity_miss_l1d++;
  		infinity_cache_l1d[address]=true;
  	}	

}

void check_infinity_cache_l2(unsigned long long address)
{

	itr_l2 = infinity_cache_l2.find(address);
  	if (itr_l2 != infinity_cache_l2.end())//No entry in table
  	{
  		//Hit
  	}
  	else
  	{
  		ct_infinity_miss_l2++;
  		infinity_cache_l2[address]=true;
  	}	

}