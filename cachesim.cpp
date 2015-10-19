#include <cstdio>
#include <iostream>
using namespace std;

int main()
{
	FILE *file;
	unsigned long long *buffer;
	long size;
	size_t readstatus;

	file=fopen("ls.trace","rb");
	if(file==NULL)
	{
		cout<<"File Open Error"<<endl;
		//exit(1);
	}

	fseek(file, 0, SEEK_END);
    size=ftell(file);
    rewind(file);
    int itr;
    for(itr=1;itr<5;itr++)
    {
    	readstatus=fread(buffer,8,1,file);
    	cout<<"read :"<<buffer<<endl;	
    }
    
    fclose(file);


	return 0;
}

