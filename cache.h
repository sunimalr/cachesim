#ifndef CACHE_H
#define CACHE_H

struct cache_line_block{
	int valid;
	unsigned long long tag;
};
typedef struct cache_line_block cache_line_block;

struct l1i_cache_set{
	cache_line_block block[8];
};
typedef l1i_cache_set l1i_cache_set;

l1i_cache_set cache_l1i[64];

int l1i_fetch(unsigned long long address);
