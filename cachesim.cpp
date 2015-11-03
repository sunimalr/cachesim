#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <bitset>
#include "cache.hpp"



using namespace std;
int process(unsigned long long address, int instruction);
int detect_instr_type(unsigned long long buffer);
unsigned long long remove_encoded_bits(unsigned long long buffer, int type);

int main()
{
    //Init cache
    init_l1i_cache();
    init_l1d_cache();
	
    FILE *rm;
    unsigned long long buf[1];
    int i;
    long fSize;
    
    //Open trace file
    rm = fopen("ls.trace", "rb");
    buf[0]=0;

    // obtain file size:
  	fseek (rm , 0 , SEEK_END);
  	fSize = ftell (rm);
  	rewind (rm);
  	long ctr=fSize/8;
    if (rm != NULL) {
    	for(i=0;i<100;i++)
    	{
        	fread(buf,8 , 1, rm);
        	switch(detect_instr_type(buf[0]))
    		{
    			case FETCH :
                    cout<<"FETCH"<<endl;
    				process(buf[0],FETCH);
    				l1i_check(buf[0]);
    				break;
    			case LOAD :
                    cout<<"LOAD"<<endl;
    				process(remove_encoded_bits(buf[0],LOAD),LOAD);
                    l1d_check(buf[0],LOAD);
    				break;
    			case STORE :
                    cout<<"STORE"<<endl;
    				process(remove_encoded_bits(buf[0],STORE),STORE);
                    l1d_check(buf[0],STORE);
    				break;
    		}
	    }
	    fclose(rm);
	    cout<<"END"<<endl;
    }
    else
        printf("File Not Found.\r\n");
	

	return 0;
}

int process(unsigned long long address, int instruction)
{
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