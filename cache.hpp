#ifndef CACHE_H
#define CACHE_H
#endif

struct cache_line_block{
	int valid;
	unsigned long long tag;
};
typedef struct cache_line_block cache_line_block;

struct l1i_cache_set{
	cache_line_block set[8];
};
typedef l1i_cache_set l1i_cache_set;

int l1i_check(unsigned long long address);
unsigned long long get_offset(unsigned long long address);
unsigned long long get_set_index(unsigned long long address);
unsigned long long get_tag(unsigned long long address);
void init_l1i_cache();