#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <bitset>
#include "cache.hpp"
#include <iostream>
#include <fstream>



using namespace std;
int process(unsigned long long address, int instruction);
int detect_instr_type(unsigned long long buffer);
unsigned long long remove_encoded_bits(unsigned long long buffer, int type);
//For prefetcher
 unsigned long long program_counter=0; //Used for prefetcher

int ct_l1i_hit=0;
int ct_l1i_miss=0;

int ct_l1d_w_hit=0;
int ct_l1d_w_miss=0;
int ct_l1d_r_hit=0;
int ct_l1d_r_miss=0;

int ct_l2_w_hit=0;
int ct_l2_w_miss=0;
int ct_l2_r_hit=0;
int ct_l2_r_miss=0;

int ct_victim_hit=0;
int ct_victim_miss=0;

bool d;

int main(int argc, char* argv[])
{
    //Init cache

    if (argc < 2) {
        
        std::cerr << "Usage: " << argv[0] << " COMPILER_OPTION FILENAME" << std::endl;
        
        return 1;
    }
    

    #ifdef DEBUG
        d=true;
    #endif    
    init_l1i_cache();
    init_l1d_cache();
    init_l2_cache();
    init_victim_cache();
	
    FILE *rm;
    unsigned long long buf[1];
    int i;
    long fSize;
    
    //Open trace file
    rm = fopen(argv[1], "rb");
    buf[0]=0;

    // obtain file size:
  	fseek (rm , 0 , SEEK_END);
  	fSize = ftell (rm);
  	rewind (rm);
  	long ctr=fSize/8;
    if (rm != NULL) {
    	for(i=0;i<ctr;i++)
    	{
        	fread(buf,8 , 1, rm);
        	switch(detect_instr_type(buf[0]))
    		{
                
    			case FETCH :
                    if(d)
                    cout<<"FETCH"<<endl;
                    program_counter=buf[0];
    				process(buf[0],FETCH);
    				l1i_check(buf[0]);
    				break;
    			case LOAD :
                    if(d)
                    cout<<"LOAD"<<endl;
    				process(remove_encoded_bits(buf[0],LOAD),LOAD);
                    l1d_check(buf[0],LOAD);
    				break;
    			case STORE :
                    if(d)
                    cout<<"STORE"<<endl;
    				process(remove_encoded_bits(buf[0],STORE),STORE);
                    l1d_check(buf[0],STORE);
    				break;
    		}
	    }
	    fclose(rm);
        if(d)
	    cout<<"END"<<endl;
        calculate_stats();
    }
    else
        printf("File Not Found.\r\n");
	

	return 0;
}

int process(unsigned long long address, int instruction)
{
    if(d)
    cout<<"Address : "<<hex<<address<<endl;
	return 0;
}

int detect_instr_type(unsigned long long buffer)
{
    int bit63,bit62;
    if((buffer & 0x8000000000000000) > 0)
    {
        bit63=1;
    }
    else
    {
        bit63=0;
    }

    if((buffer & 0x4000000000000000) > 0)
    {
        bit62=1;
    }
    else
    {
        bit62=0;
    }

    if((bit63==0)&&(bit62==0))
    {
        return FETCH;
    }

    if((bit63==0)&&(bit62==1))
    {
        return LOAD;
    }

    if((bit63==1)&&(bit62==0))
    {
        return STORE;
    }
    if(d)
    cout<<"Corrupted Address"<<endl;
	return 0;
}

unsigned long long remove_encoded_bits(unsigned long long buffer, int type)
{
    if(type==STORE)
    {
        return (buffer^0x8000000000000000);
    }
    if(type==LOAD)
    {
        return (buffer^0x4000000000000000);
    }
    return 0;
}

int calculate_stats()
{
    
    ofstream myfile;
    myfile.open ("cache_stats.txt");
    myfile << "Cache statistics.\n";
    myfile <<"L1 -i hits   : "<<ct_l1i_hit<<endl;
    myfile <<"L1 -i misses : "<<ct_l1i_miss<<endl;
    //myfile <<"L1 -i miss rate : "<<ct_l1i_miss/(ct_l1i_hit+ct_l1i_miss)*1.0<<endl;
    myfile <<"L1 -i hit rate : "<<ct_l1i_hit/((ct_l1i_hit+ct_l1i_miss)*1.0)<<endl;

    myfile <<"L1 -d read hits   : "<<ct_l1d_r_hit<<endl;
    myfile <<"L1 -d read misses : "<<ct_l1d_r_miss<<endl;
    myfile <<"L1 -d write hits   : "<<ct_l1d_w_hit<<endl;
    myfile <<"L1 -d write misses : "<<ct_l1d_w_miss<<endl;
    float tmp=(ct_l1d_w_hit+ct_l1d_r_hit)/((ct_l1d_w_miss+ct_l1d_r_miss+ct_l1d_w_hit+ct_l1d_r_hit)*1.0);
    myfile <<"L1 -d hit rate : "<<tmp<<endl;

    myfile <<"L2 read hits   : "<<ct_l2_r_hit<<endl;
    myfile <<"L2 read misses : "<<ct_l2_r_miss<<endl;
    myfile <<"L2 write hits   : "<<ct_l2_w_hit<<endl;
    myfile <<"L2 write misses : "<<ct_l2_w_miss<<endl;
    tmp=(ct_l2_w_hit+ct_l2_r_hit)/((ct_l2_w_miss+ct_l2_r_miss+ct_l2_w_hit+ct_l2_r_hit)*1.0);
    myfile <<"L2 hit rate : "<<tmp<<endl;
     myfile <<"Compulsory misses L1i: "<<ct_infinity_miss_l1i<<endl;
    myfile <<"Compulsory misses L1d: "<<ct_infinity_miss_l1d<<endl;
    myfile <<"Compulsory misses L2: "<<ct_infinity_miss_l2<<endl;
    myfile.close();    

    //myfile.open ("cache_stats.txt");
    cout << "Cache statistics.\n";
    cout <<"L1 -i hits   : "<<ct_l1i_hit<<endl;
    cout <<"L1 -i misses : "<<ct_l1i_miss<<endl;
    //cout <<"L1 -i miss rate : "<<ct_l1i_miss/(ct_l1i_hit+ct_l1i_miss)*1.0<<endl;
    cout <<"L1 -i hit rate : "<<ct_l1i_hit/((ct_l1i_hit+ct_l1i_miss)*1.0)<<endl;

    cout <<"L1 -d read hits   : "<<ct_l1d_r_hit<<endl;
    cout <<"L1 -d read misses : "<<ct_l1d_r_miss<<endl;
    cout <<"L1 -d write hits   : "<<ct_l1d_w_hit<<endl;
    cout <<"L1 -d write misses : "<<ct_l1d_w_miss<<endl;
    tmp=(ct_l1d_w_hit+ct_l1d_r_hit)/((ct_l1d_w_miss+ct_l1d_r_miss+ct_l1d_w_hit+ct_l1d_r_hit)*1.0);
    cout <<"L1 -d hit rate : "<<tmp<<endl;

    cout <<"L2 read hits   : "<<ct_l2_r_hit<<endl;
    cout <<"L2 read misses : "<<ct_l2_r_miss<<endl;
    cout <<"L2 write hits   : "<<ct_l2_w_hit<<endl;
    cout <<"L2 write misses : "<<ct_l2_w_miss<<endl;
    tmp=(ct_l2_w_hit+ct_l2_r_hit)/((ct_l2_w_miss+ct_l2_r_miss+ct_l2_w_hit+ct_l2_r_hit)*1.0);
    cout <<"L2 hit rate : "<<tmp<<endl;


    cout <<"victim hits   : "<<ct_victim_hit<<endl;
    cout <<"victim misses : "<<ct_victim_miss<<endl;
    cout <<"victim hit rate : "<<ct_victim_hit/((ct_victim_hit+ct_victim_miss)*1.0)<<endl;

     cout <<"Compulsory misses L1i: "<<ct_infinity_miss_l1i<<endl;
    cout <<"Compulsory misses L1d: "<<ct_infinity_miss_l1d<<endl;
    cout <<"Compulsory misses L2: "<<ct_infinity_miss_l2<<endl;
    
    //cout <<"Infinity cache hits   : "<<ct_infinity_hit<<endl;
    //cout <<"Infinity cache misses : "<<ct_infinity_miss<<endl;
    //cout <<"victim hit rate : "<<ct_victim_hit/((ct_victim_hit+ct_victim_miss)*1.0)<<endl;
    return 0;
}
