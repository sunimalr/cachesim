#ifndef CACHE_H
#define CACHE_H
#endif

#define FETCH 100
#define LOAD 200
#define STORE 300

#define INIT 1000
#define STEADY 2000
#define TRANSIENT 3000
#define NOPRED 4000

struct cache_line_block{
	int valid;
	unsigned long long tag;
	int mru_bit;
	int dirty_bit;
};
typedef struct cache_line_block cache_line_block;

struct cache_set{
	cache_line_block set[8];
};
typedef cache_set cache_set;

//l1i_cache_set cache_l1i[64];
extern unsigned long long program_counter;

extern int ct_l1i_hit;
extern int ct_l1i_miss;

extern int ct_l1d_w_hit;
extern int ct_l1d_w_miss;
extern int ct_l1d_r_hit;
extern int ct_l1d_r_miss;

extern int ct_l2_w_hit;
extern int ct_l2_w_miss;
extern int ct_l2_r_hit;
extern int ct_l2_r_miss;

extern int ct_victim_hit;
extern int ct_victim_miss;

extern int ct_infinity_miss_l1i;
extern int ct_infinity_miss_l1d;
extern int ct_infinity_miss_l2;

extern bool d;

int calculate_stats();

int l1i_check(unsigned long long address);
int l1d_check(unsigned long long address, int type);
int l2_check(unsigned long long address, int type);
int l2_write(unsigned long long address);

unsigned long long get_offset(unsigned long long address);
unsigned long long get_set_index(unsigned long long address);
unsigned long long get_tag(unsigned long long address);

unsigned long long get_l2_set_index(unsigned long long address);
unsigned long long get_l2_tag(unsigned long long address);

unsigned long long get_victim_cache_tag(unsigned long long address);
void write_to_victim_cache(unsigned long long address);
int victim_cache_check(unsigned long long address, int type);

void init_l1i_cache();
void init_l1d_cache();
void init_l2_cache();
void init_victim_cache();
void check_infinity_cache_l1i(unsigned long long address);
void check_infinity_cache_l1d(unsigned long long address);
void check_infinity_cache_l2(unsigned long long address);


int get_8_way_bit_lru_position(cache_line_block *cache_8_block);
int get_8_way_tree_lru_position(int *lru_bits);
void flip_if_all_mru_bits_are_1(cache_line_block *cb);
void flip_bits(int *lru_bits, int pos1, int pos2, int pos3);
int get_tree_lru_position(int way, int *lru_bits);
void flip_bits_lru(int way, int *lru_bits, int line);

unsigned long long get_prefetch_address(unsigned long long pc, unsigned long long add);
int l2_prefetch(unsigned long long address);
