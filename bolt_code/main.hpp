#include "meshDef.cpp"
#include "graphics.cpp"
#include "pngsupport.cpp"
#include <stdlib.h>
#include <array>
#include <unistd.h>
#include <time.h>
#include <chrono>


bool checkArguments(int argc, char *argv[], float &sliceSize){
	
	sliceSize = 0.1;

	switch(argc){
	
		case 1: 
			cout<<"\nParameter missing : filename\nExiting ";
			return false;
			
		case 2:
			cout<<"\nSlicing "<<argv[1]<<" with default slice size of "<<sliceSize;
			break;		
	
		case 3: 
			sliceSize = atof(argv[2]);	
			if(sliceSize>1 || sliceSize<0.001 || sliceSize==NAN){
			
				cout<<"\nPlease specify a slice size between 0.001 and 1 ";
				return false;
			}
			cout<<"\nSlicing "<<argv[1]<<" with supplied slice size of "<<sliceSize;
			break;

		default: 
			cout << "\nExtraneous parameters supplied, exiting. ";
			return false;
	}
	
	return true;

}
