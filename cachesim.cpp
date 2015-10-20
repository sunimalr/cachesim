#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <bitset>

using namespace std;

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
	    }
	    fclose(rm);
	    cout<<"END"<<endl;
    }
    else
        printf("File Not Found.\r\n");
	

	return 0;
}


