#ifndef CACHE_H
#define CACHE_H
#endif

#define FETCH 100
#define LOAD 200
#define STORE 300

struct cache_line_block{
	int valid;
	unsigned long long tag;
	int mru_bit;
};
typedef struct cache_line_block cache_line_block;

struct cache_set{
	cache_line_block set[8];
};
typedef cache_set cache_set;

//l1i_cache_set cache_l1i[64];

int l1i_check(unsigned long long address);
int l1d_check(unsigned long long address, int type);
int l2_check(unsigned long long address, int type);

unsigned long long get_offset(unsigned long long address);
unsigned long long get_set_index(unsigned long long address);
unsigned long long get_tag(unsigned long long address);

unsigned long long get_l2_set_index(unsigned long long address);
unsigned long long get_l2_tag(unsigned long long address);

void init_l1i_cache();
void init_l1d_cache();
void init_l2_cache();


int get_8_way_bit_lru_position(cache_line_block *cache_8_block);
int get_8_way_tree_lru_position(int *lru_bits);
void flip_if_all_mru_bits_are_1(cache_line_block *cb);
void flip_bits(int *lru_bits, int pos1, int pos2, int pos3);
