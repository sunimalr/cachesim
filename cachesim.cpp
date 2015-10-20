#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <bitset>

#define FETCH 100
#define LOAD 200
#define STORE 300

using namespace std;
int process(unsigned long long address, string instruction);
int detect_instr_type(unsigned long long buffer);

int main()
{
	FILE *rm;
    unsigned long long buf[1];
    char b[9];
    int i;
    long fSize;
    
    rm = fopen("ls.trace", "rb");
    //rm = fopen("test.txt", "r");
    buf[0]=0;

    // obtain file size:
  	fseek (rm , 0 , SEEK_END);
  	fSize = ftell (rm);
  	rewind (rm);
  	//cout<<fSize<<endl;
  	long ctr=fSize/8;
    if (rm != NULL) {
    	for(i=0;i<ctr;i++)
    	{
        	fread(buf,8 , 1, rm);
        	//cout<<bitset<64>(buf[0])<<endl
        	//cout<<hex<<buf[0]<<endl;
        	//cout<<(buf[0]&0x8000000000000000)<<endl;
        	//cout<<(buf[0]&0x4000000000000000)<<endl;
        		switch(detect_instr_type(buf[0]))
        		{
        			case FETCH :
        				process(buf[0],FETCH);
        				break;
        			case LOAD :
        				process(buf[0],LOAD);
        				break :
        			case STORE :
        				process(buf[0],STORE);
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
	if(address)
	return 0;
}

int detect_instr_type(unsigned long long buffer)
{
	return 0;
}